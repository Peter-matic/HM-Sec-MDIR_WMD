# HM-Sec-MDIR_WMD
Hallo allerseits,

nun muss ich mich doch einmal in die Diskussion einschalten.
Ich beschäftige mich auch schon eine ganze Weile mit dem WMD.

Aber zunächst einmal meinen vielen Dank an die Experten, die diese hervorragende Vorabeit zur Asksin- Bibliothek und die anderen Sketches für  weiter Innogy - Geräte geleistet haben.

Eigentlich wollte ich einen eigenen Thread zum WMD erstellen, bin dabei aber auf diesen und den ursprünglichen Parallel- Thread 

 [RWE/Innogy/Livisi Bewegungsmelder WMD - Homematic/AskSinPP Firmware](https://homematic-forum.de/forum/viewtopic.php?f=76&t=64804&hilit=Wmd&start=20)
 
 von @re-vo-lution gestoßen.
 
 Aber nun zum Thema:
 
 # Schaltungsanalyse
 
 Hier nochmal die Fotos der beiden im WMD enthaltenen Platinen:
 
 Hauptplatine (Vorder- und Rückseite)
 ![](https://github.com/Peter-matic/HM-Sec-MDIR_WMD/blob/main/Pictures/Hauptplatine%20Vorder-%20und%20R%C3%BCckseite.jpg)
 
 Tochterplatine (Vorder- und Rückseite)
 ![](https://github.com/Peter-matic/HM-Sec-MDIR_WMD/blob/main/Pictures/Tochterplatine%20Vorder-%20und%20R%C3%BCckseite.jpg)
 
Als nächstes habe ich die Schaltung analysiert und ein Schaltbild des WMD erstellt:

![](https://github.com/Peter-matic/HM-Sec-MDIR_WMD/blob/main/Schematic/WMD_Schem.jpg)

[Hier](https://github.com/Peter-matic/HM-Sec-MDIR_WMD/blob/main/Schematic/WMD_Schem.pdf) nochmal als PDF mit besserer Auflösung.

Es handelt sich dabei um eine prinzipielle Ansicht, die die Funktion verdeutlichen soll. Es gibt auf der Platine z.B. viele parallel geschaltete Kondensatoren. Diese habe ich der Übersichtlichkeit halber zu einem Symbol zusammengefasst. Die Werte der einzelnen Bauelemente habe ich auch nicht ausgemessen, was im eingebauten Zustand wohl auch nicht sinnvoll ist. Lediglich das Teilerverhältnis des Spannungsteilers für die Batterieüberwachung habe ich durch Spannungsmessung ermittelt. Es beträgt 2:1.

# Bewegungsmelder

Über die Funktion der Schaltung um den PIR- Sensor wurde schon in einigen vorausgehenden Forenbeiträgen gerätselt. Leider hat der PIR keine Typenbezeichnung. Vom Aussehen her könnte  es sich um einen D203B handeln. Zumindest ist im zugehörigen [Datenblatt](https://github.com/Peter-matic/HM-Sec-MDIR_WMD/blob/main/Datasheets/D203B-e.pdf) auf Seite 4 ein sehr ähnliches Schaltbild angegeben, wie es hier verwendet ist. Die Schaltungen der beiden PIRs sind identisch.

Das Signal des PIR wird zunächst durch einen OpAmp verstärkt. Anschließend wird der Gleichstromanteil des Signals durch einen Koppelkondensator abgetrennt, sodass nur die Flanken ausgewertet werden. Die Flanken werden durch die zweite Hälfte des OpAmp nochmal vestärkt und dann einem nichtinvertierenden und invertierenden Komperator zugeführt. An deren Ausgängen steht dann jeweils ein Impuls für die positive und die neative Flanke zur Verfügung. Für beide PIRs werden diese dann auf vier Eingänge des Atmega gegeben.

Wie man das zugrunde liegenden Sketch auf 4 PIR- Eingänge aufbohrt, ist ja bereits [hier](https://homematic-forum.de/forum/viewtopic.php?f=76&t=44118&hilit=HM+SEC+MDIR) beschrieben.

# Lichtsensor

Wie schon in vorhergehenden Forenbeiträgen vermutet, scheint es sich bei dem Lichtsensor um einen LMSS-101 zu handeln. dafür spricht auch die logarithmichhe Charakteristik der aufgezeichneten Messwerte.

