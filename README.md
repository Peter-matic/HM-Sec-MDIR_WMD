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

Es handelt sich dabei um eine prinzipielle Ansicht, die die Funktion verdeutlichen soll. Es gibt auf der Platine z.B. viele parallel geschaltete Kondensatoren. Dese habe ich der Übersichtlichkeit halber zu einem Symbol zusammengefasst. Die Werte der einzelnen Bauelementa habe ich auch nicht ausgemessen, was im eingebauten Zustand wohl auch nicht sinnvoll ist. Lediglich das Teilerverhältnis des Spannungsteilers für die Batterieüberwachung habe ich durch Spannungsmessung ermittelt. Es beträgt 2:1.
