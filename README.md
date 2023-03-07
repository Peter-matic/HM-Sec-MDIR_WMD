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

Wie schon in vorhergehenden Forenbeiträgen vermutet, scheint es sich bei dem Lichtsensor um einen LMSS-101 zu handeln. Dafür spricht auch die logarithmische Charakteristik der aufgezeichneten Messwerte. Zur Analyse wurde bei steigender Beleuchtungsstärke mit dem Historian die Kurve eines originalen HM-Sec-MDIR und die Rohwerte des AD- Wandlers am Lichtsensor- Eingang des Atmega aufgezeichnet. Die Werte des WMD sind logarithmisch während der HM-Sec-MDIR weitestgehend linear ist.

![](https://github.com/Peter-matic/HM-Sec-MDIR_WMD/blob/main/Pictures/Graph1.jpg)

Es gilt nun, die Kurve des WMD zu linearisiern und auf einen Wertebereich von 8 Bit zu bringen, da im Telegramm nur 8 Bit zur Übertragung zur Verfügung stehen.

Im Datenblatt des [LMSS-101](https://github.com/Peter-matic/HM-Sec-MDIR_WMD/blob/main/Datasheets/LMSS-101.pdf) sind Referenzwerte und eine Kurve (Light Source Dependence) angegeben.

Im logarithmischen Maßstab handelt es sich dabei um eine Gerade, die sich mit der Gleichung y = mx+b beschreiben lässt. Wegen des logarithnischen Maßstabes muss es hier aber heißen: y = m*log(x)+b
Die Steigung m lässt sich mit m = (y2-y1)/(log(x2)-log(x1)) berechnen.
Die Referenzwerte VRef1 und VRef3 aus dem Datenblatt eingesetzt ergibt das:

m = (2,13V - 1,3V) / log(0,27 lux - log(25 lux) = -0,422 V/log(lux)

Die Steigung ist negativ. D.H. große Ausgangsspannung bei Dunkelheit, kleine Spannung bei Helligkei. Des wurde ja in vorhergehenden Beiträgen schon festgestellt.

Um den Achsenabschnitt b zu berechnen können wir die Formel nach b umstellen und den driten Referenzwert VRef2 einsetzen:

b = y - m*log(x) = 1,69V - (-0,422 V/log(lux)*log(3,0 lux) = 1,89
(Stimmt nicht ganz exakt mit der Kurve überein, aber da sind ja auch Toleranzen angegeben.)

Die Formel zur Beschreibung des Lichtsensors lautet also

U = -0,422*log(L)+1,89  ( U in Volt und L in lux)

Nun messen wir aber den y- wert als Spannung und wollen x in lux wissen. Daher müssen wir die Formel nochmal nach x umstellen:

x = 10 hoch ((y-b)/m)) oder L = 10 hoch ((U-1,89V)/-0,422))  ( U in Volt und L in lux)

Nachdem wir am AD- Wandler eine Referenzspannung vom 3,0V haben, können wir mit dem Faktor 3V/1024Digits auch gleich die Rohwerte des AD- Wandlers in die Formel einsetzen um diese Berechnung im Code zu sparen.

Damit ergibt sich L = 10 hoch ((A-646)/-144))  ( A in Digits und L in lux)

Der LMSS-101 kann zwar von 0,1 bis 3000 lux messen, allerdings ist im originalen HM-Sec-MDIR der Messbereich nach oben bei ca. 500 Lux begrenzt und bei 0 Lux gibt es einen Offset von 33 Digits. Dies ist bei der codierung dieses Mathematik- Exkurses ebenfalls zu berücksichtigen.

