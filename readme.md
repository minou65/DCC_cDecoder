# cDecoder
## Inhalt
- [cDecoder](#cdecoder)
  - [Inhalt](#inhalt)
  - [Funktionen](#funktionen)
  - [Programmierung](#programmierung)
  - [Analog Betrieb](#analog-betrieb)
  - [Immer ein](#immer-ein)
  - [Powerpack](#powerpack)
  - [Dimmen der Ausgänge](#dimmen-der-ausgänge)
  - [Anschlüsse](#anschlüsse)
    - [fDecoder](#fdecoder)
    - [cDecoder](#cdecoder-1)
    - [PowerPack](#powerpack-1)
    - [Farben](#farben)
  - [Lange DCC Adresse](#lange-dcc-adresse)
  - [CV Tabelle](#cv-tabelle)
    - [Zuordnung Ausgänge zu Funktionstasten](#zuordnung-ausgänge-zu-funktionstasten)
    - [Funktion aktiv bei](#funktion-aktiv-bei)
    - [Modus der Ausgänge](#modus-der-ausgänge)
      - [Blinker On Time](#blinker-on-time)
      - [Blinker off Time](#blinker-off-time)
      - [Dimmen der Ausgänge](#dimmen-der-ausgänge-1)
      - [Automatisches abschalten](#automatisches-abschalten)
  - [Beispiele](#beispiele)
  - [Programmieren des prozessors](#programmieren-des-prozessors)

## Funktionen
- Ein/Aus
- Impuls
- Blinker

Der Attiny85 untersützt nur 5 Ausgänge. Die Funktionen sind auf die Ausgänge verteilt. Die Funktionen können über die CV's 66 - 70 eingestellt werden. 
Die Blinkerfunktion kann über die CV's 71 - 75 eingestellt werden. Die Zeit für das Blinken kann über die CV's 76 - 80 eingestellt werden. 
Die Zeit wird in 10ms Schritten angegeben. Die Helligkeit der Ausgänge kann über die CV's 81 und 82 eingestellt werden. 
Die Zeit für das automatische abschalten kann über die CV's 86 - 90 eingestellt werden. Die Zeit wird in 10ms Schritten angegeben.

## Programmierung
Um den Decoder auf dem Programmiergleis programmieren zu können muss eine Last an AUX 1 angeschlossen werden, die mindestens einen Strom von 60mA erzeugt.

## Analog Betrieb
Wird während 100ms kein DCC Signal erkannt, werden die Funktionen gemäss CV 13 aktiviert

## Immer ein
Ab Version 3 untersützt die Firmware CV 123, damit können Ausgänge definiert werden, welche beim anlagen einer Spannung sofort eingeschaltet werden.
Eingeführt für die Rheinbrücke beim Modell Eisenbahn Club Seebezirk (MECS), Fräschels (CH)

## Powerpack
Ein externes Powerpack kann an den Decoder angeschlossen werden. Dazu können die Pad's VCC und GND auf der Rückseite des Decoders verwendet werden.

## Dimmen der Ausgänge
Die Ausgänge AUX 1 und AUX 2 können gedimmt werden. Die Helligkeit kann über CV 81 und 82 eingestellt werden.

## Anschlüsse
### fDecoder
| oben | unten |
|---|---|
| ![Alt text](img/fDecoder_oben.png) | ![Alt text](img/fDecoder_unten.png) |
| __v2__ <br> ![Alt text](img/fDecoder_oben_v2.png) | |

### cDecoder
| oben | unten |
|---|---|
|![Alt text](img/cDecoder.png) ||

### PowerPack
| oben | unten |
|---|---|
| ![Alt text](img/PowerPack.png) ||

### Farben
| Draht | Verwendung |
|--------|--------------|
| Rot | Rail |
| Braun | Rail |
| Blau | Decoder + |
| Weiss | Aux 1 / Licht vorne |
| Gelb | Aux 2 / Licht hinten |
| Violett | Aux 3 |
| Grün | Aux 4 |
| Schwarz | GND |

## Lange DCC Adresse
Um die lange DCC-Adresse in die Register 17 und 18 umzurechnen, kann folgende Formel verwendet werden.<br><br>
CV 17 = Adresse / 256 (nur die Stellen vor dem Komma) + 192<br>
CV 18 = Adresse - ((CV 17 - 192) * 256)<br>

Die lange Adresse wird nur verwendet, wenn das Bit 5 in CV 29 gesetzt ist

Hier gibt es einen CV Rechner
https://www.opendcc.de/info/decoder/dcc_cv.html

## CV Tabelle
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| Basisadresse | 1 | 1 - 255 (3) | Wertebereich im DCC-Format 1 - 127 |
| | 2 | 120 | |
| | 5 | 255 | |
| Version | 7 | | |
| Hersteller | 8 | | |
| Reset | 8 |  | Durch Eingeben eines Wertes 8, werden alle Einstellungen auf Default gesetzt |
| Im Analogbetrieb aktive Ausgänge | 13 | 0 - 255 (0) | AUX 1 ein = 1<br>AUX 2 ein = 2<br>AUX 3 ein = 4<br>AUX 4 ein = 8<br>AUX 5 ein = 16 |
| Decoderlock | 15 | 0 - 255 (0) | Decoder-Lock Funktion laut NMRA. <br>Details siehe http://www.nmra.org/standards/DCC/WGpublic/0305051/0305051.html |
| Decoderlock | 16 | 0 - 255 (0) | Decoder-Lock Funktion laut NMRA. <br>Details siehe http://www.nmra.org/standards/DCC/WGpublic/0305051/0305051.html |
| Erweiterte Adresse | 17 | 192 - 255 (0) | Nur für DCC-Format. Bei den meisten Zentralen ist es möglich, <br> erweiterte Adressendirekt einzugeben. Die CVs 17, 18 und 29 <br> werden von der Zentrale automatisch richtig eingestellt. |
| Erweiterte Adresse | 18 | 0 - 255 (3) |
| Konfigurationsdaten | 29 | 0 - 64 (2) | Fahrtrichtung Standard = 0 <br>Fahrtrichtung invertiert = 1 <br>14 Fahrstufen = 0 <br>28 oder 128 Fahrstufen 2 <br>Basisadressen = 0 <br>Erweiterte Adressen = 32 |
| Betriebszustand | 122 | 0/1 (0) | Den Betriebszustand der Ausgänge speichern. <br>0 = Aus <br>1 = Ein |
| Immer eingeschaltet<br>(ab Version 3) | 123 | 0 - 255 (0) | AUX 1 ein = 1<br>AUX 2 ein = 2<br>AUX 3 ein = 4<br>AUX 4 ein = 8<br>AUX 5 ein = 16 |

### Zuordnung Ausgänge zu Funktionstasten
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| F0 | 30 | 0 - 31 (3) | AUX 1 = 1 <br> AUX 2 = 2 <br> AUX 3 = 4 <br> AUX 4 = 8 <br> AUX 5 = 16 |
| F1 | 31 | 0 - 31 (4) | |
| F2 | 32 | 0 - 31 (8) | |
| ... | ... | 0 - 31 (0) | |
| F28 | 58 | 0 - 31 (0) | |

### Funktion aktiv bei
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| AUX 1 | 61 | 0 - 31 (1) | Vorwärts = 0 <br> Rückwärts = 1 <br> Beide = 3 <br> Default bei cDecoder ist 3 |
| AUX 2 | 62 | 0 - 31 (2) | Default bei cDecoder ist 3 |
| AUX 3 | 63 | 0 - 31 (3) | |
| AUX 4 | 64 | 0 - 31 (3) | |
| AUX 5 | 65 | 0 - 31 (3) | |

### Modus der Ausgänge
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| AUX 1 | 66 | 0 - 31 (1) | Ein / Aus = 1 <br> Blinker = 2 <br> Impuls = 3 |
| AUX 2 | 67 | 0 - 31 (1) | |
| AUX 3 | 68 | 0 - 31 (1) | |
| AUX 4 | 69 | 0 - 31 (2) | |
| AUX 5 | 70 | 0 - 31 (2) | |

#### Blinker On Time
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| AUX 1 | 71 | 0 - 255 (100) | Wert x 10 = Zeit in milli Sekunden |
| AUX 2 | 72 | 0 - 255 (100) | |
| AUX 3 | 73 | 0 - 255 (100) | |
| AUX 4 | 74 | 0 - 255 (100) | |
| AUX 5 | 75 | 0 - 255 (100) | |

#### Blinker off Time
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| AUX 1 | 76 | 0 - 255 (100) | Wert x 10 = Zeit in milli Sekunden |
| AUX 2 | 77 | 0 - 255 (100) | |
| AUX 3 | 78 | 0 - 255 (100) | |
| AUX 4 | 79 | 0 - 255 (100) | |
| AUX 5 | 80 | 0 - 255 (100) | |

#### Dimmen der Ausgänge
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| AUX 1 | 81 | 1 - 255 (255) | |
| AUX 2 | 82 | 1 - 255 (255) | |

#### Automatisches abschalten
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| AUX 1 | 86 | 0 - 255 (0) | Wert x 10 = Zeit in milli Sekunden. Ist der <br> Wert grösser als 0 wird der Ausgang nach der <br> eingestellten Zeit wieder abgeschaltet. <br> Ist der Wert 0, dann bleibt der Ausgang eingeschaltet. |
| AUX 2 | 87 | 0 - 255 (0) | |
| AUX 3 | 88 | 0 - 255 (0) | |
| AUX 4 | 89 | 0 - 255 (0) | |
| AUX 5 | 90 | 0 - 255 (0) | |

## Beispiele
|Beispiel | Einstellungen |
|---|---|
| SBB Schlusslicht an AUX 4 <br><br><br><br> | CV64 = 36<br>CV69 = 27 <br>CV74 = 1 (On Time)<br>CV79 = 70 (Off Time) |
| Rheinbrücke MESC<br>Rote LED soll immer blinken<br><br><br> | CV123 = 1<br>CV66 = 2<br>CV71 = 80<br>76 = 20 |


## Programmieren des prozessors

Um den Prozessor zu programmieren wird ein ISP Programmer benötigt. Der Prozessor kann mit der Arduino IDE programmiert werden. Dazu muss der Prozessor als Board hinzugefügt werden.
- Dazu unter Datei -> Voreinstellungen -> Zusätzliche Boardverwalter-URLs: https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json einfügen.
- Arduino AVR Boards installieren unter Werkzeuge -> Board -> Boardverwalter... -> Arduino AVR Boards installieren.
- In der Arduino IDE unter Werkzeuge -> Board -> Boardverwalter... -> attiny auswählen und installieren. 

Danach das Board wie folgt konfigurieren
    Board    : Attiny25/45/85
    Prozessor: Attiny85
    Clock    : Internal 16Mhz
    Programmer: Diamex USB ISP oder ATMEL STK500 (Für Windows 10 ist kein Treiber erforderlich)


Nun kann zuerst der Bootloader und danach die Firmware auf den Prozessor geschrieben werden. Unter visual Micro kommt folgende Fehlermeldung beim schreiben des Bootloaders

```
Error while burning bootloader.
Burn failed
avrdude: can't open input file aders/{bootloader.file}: No such file or directory
avrdude: read from file oaders/{bootloader.file}' failed
```

Dies ist normal, der Attiny hat keine Bootloader visual Micro sucht diesen aber und findet ihn nicht. Die Fuse und Clock settings werden aber geschrieben bevor der Bootloader geschrieben wird.
https://www.visualmicro.com/forums/YaBB.pl?num=1607630288

