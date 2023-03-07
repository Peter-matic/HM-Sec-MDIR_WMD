//- -----------------------------------------------------------------------------------------------------------------------
// AskSin++
// 2016-10-31 papa Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
// 2023-02-21 modified by Peter.matic Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
// ci-test=yes board=328p aes=no
//- -----------------------------------------------------------------------------------------------------------------------

// define this to read the device id, serial and device type from bootloader section
// #define USE_OTA_BOOTLOADER

// define this if you have a TSL2561 connected at address 0x29
// #define USE_TSL2561

// define this if you have a BH1750 connected at address 0x23
// #define USE_BH1750
// define this if you have a LDR or analougue sensor like LMSS-101 at analog input 
#define USE_LMSS

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
#include <AskSinPP.h>
#include <LowPower.h>

#if defined(USE_TSL2561)
#include <sensors/Tsl2561.h>
#elif defined(USE_BH1750)
#include <sensors/Bh1750.h>
#elif defined(USE_LMSS)
#include <sensors/LMSS.h>
// pin to measure LMSS-101
#define LMSS_SENSE_PIN A6
// pin to power LMSS-101 (or 0 if connected to vcc)
#define LMSS_ACTIVATOR_PIN 4
// curve steepness
#define LMSS_PARAM_M -129830
// axis intercept +40000
#define LMSS_PARAM_B 735690
// min value of measuring range
#define LMSS_PARAM_MIN 33
// max value of meauring range
#define LMSS_PARAM_MAX 500
#endif

#include <MultiChannelDevice.h>
#include <Motion.h>

// we use a Pro Mini
// Arduino pin for the LED
// D4 == PIN 4 on Pro Mini
#define LED_PIN 8
// Arduino pin for the config button
// B0 == PIN 8 on Pro Mini
#define CONFIG_BUTTON_PIN 0
// Arduino pins for the PIRs (2 PIRs * 2 channels each)
#define PIR_PIN1 14
#define PIR_PIN2 15
#define PIR_PIN3 16
#define PIR_PIN4 17

// === Battery measurement ===
#define BAT_VOLT_LOW        33  // 3.3V low voltage threshold
#define BAT_VOLT_CRITICAL   30  // 3.0V critical voltage threshold, puts AVR into sleep-forever mode
// Internal measuring: AVR voltage
// #define BAT_SENSOR BatterySensor
// External measuring: Potential devider on GPIO; required if a StepUp converter is used
// one can consider lower thresholds (low=20; cri=13)
// Devider Factor is in BatterySensor.h preset to 57. In WMD it is 20. Therefore RefVcc has been changed
// to 3000 * 20 / 57 = 1053
#define BAT_SENSOR BatterySensorUni<A3,0xFF,1053> // <SensPIN, ActivationPIN, RefVcc>

// number of available peers per channel
#define PEERS_PER_CHANNEL 6

// all library classes are placed in the namespace 'as'
using namespace as;

// define all device properties
const struct DeviceInfo PROGMEM devinfo = {
    {0x56,0x78,0x91},       // Device ID
    "PET1005433",           // Device Serial
    {0x00,0x4a},            // Device Model
    0x16,                   // Firmware Version
    as::DeviceType::MotionDetector, // Device Type
    {0x01,0x00}             // Info Bytes
};

/**
 * Configure the used hardware
 */
typedef AvrSPI<10,11,12,13> SPIType;
typedef Radio<SPIType,2> RadioType;
typedef StatusLed<LED_PIN> LedType;
typedef AskSin<LedType,BAT_SENSOR,RadioType> Hal;

#if defined(USE_TSL2561)
typedef MotionChannel<Hal,PEERS_PER_CHANNEL,List0,Tsl2561<TSL2561_ADDR_LOW> > MChannel;
#elif defined(USE_BH1750)
typedef MotionChannel<Hal,PEERS_PER_CHANNEL,List0,Bh1750<0x23> > MChannel;
#elif defined(USE_LMSS)
typedef MotionChannel<Hal,PEERS_PER_CHANNEL,List0,Lmss<LMSS_SENSE_PIN,LMSS_ACTIVATOR_PIN,LMSS_PARAM_M,LMSS_PARAM_B,LMSS_PARAM_MIN,LMSS_PARAM_MAX> > MChannel;
#else	
typedef MotionChannel<Hal,PEERS_PER_CHANNEL,List0> MChannel;
#endif

typedef MultiChannelDevice<Hal,MChannel,1> MotionType;

Hal hal;
MotionType sdev(devinfo,0x20);
ConfigButton<MotionType> cfgBtn(sdev);


void setup () {
  DINIT(57600,ASKSIN_PLUS_PLUS_IDENTIFIER);
  sdev.init(hal);
  buttonISR(cfgBtn,CONFIG_BUTTON_PIN);
  motionISR(sdev,1,PIR_PIN1);
  motionISR(sdev,2,PIR_PIN2);
  motionISR(sdev,3,PIR_PIN3);
  motionISR(sdev,4,PIR_PIN4);
  
  hal.initBattery(60UL*60,BAT_VOLT_LOW,BAT_VOLT_CRITICAL); // Measure Battery every 1h
  sdev.initDone();
}

void loop() {
  bool worked = hal.runready();
  bool poll = sdev.pollRadio();
  if( worked == false && poll == false ) {
    // deep discharge protection
    // if we drop below critical battery level - switch off all and sleep forever
    if( hal.battery.critical() ) {
      // this call will never return
      //hal.activity.sleepForever(hal);
    }
    // if nothing to do - go sleep
    hal.activity.savePower<Sleep<>>(hal);
  }
}