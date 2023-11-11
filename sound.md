# Sound Decoder

Der Sounddecoder besitzt nur 3 Ausgänge, davon sind 2 mit PWM.

## Inhalt
- [Sound Decoder](#sound-decoder)
  - [Inhalt](#inhalt)
  - [Verzeichnis MP3](#verzeichnis-mp3)
  - [Verzeichnis ADVERT](#verzeichnis-advert)
  - [Erweiterte CV Tabelle](#erweiterte-cv-tabelle)
    - [Einstellungen](#einstellungen)
    - [Zuordnung Sound zu Funktionstasten](#zuordnung-sound-zu-funktionstasten)


## Verzeichnis MP3

Dieses Verzeichnis muss exakt als __MP3__ benannt werden. Hier passen bis zu 3000 Dateien mit dem Namensschema 0001.mp3 (bzw. wav), 0002.mp3, ….. 3000.mp3 hinein.

Möglich ist es auch die Dateien 0001-Lokpiff.mp3 zu benennen. Auch Leerzeichen sind erlaubt.

## Verzeichnis ADVERT

Es gelten dieselben Regeln wie für das MP3 Verzeichnis. Das ADVERT Verzeichnis hat eine besondere Funktion. Spielt man eine Datei aus einem der anderen Verzeichnisse ab und ruft währenddessen eine Datei aus dem ADVERT Verzeichnis auf, so wird das Abspielen der ursprünglichen Datei lediglich unterbrochen. Nach dem Beenden der ADVERT Einspielung wird das ursprüngliche Stück fortgesetzt. Es ist wie eine Werbeeinblendung und so deutet es der Name des Verzeichnisses ja auch schon an.

## Erweiterte CV Tabelle

### Einstellungen
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| Volume | 125 | 0 - 30 (10) | Lautstärke |
| EQ | 126 | 0 - 5 (0) | 0 = Normal <br> 1 = POP <br> 2 = Rock <br> 3 = Jazz <br> 4 = Classic <br> 5 = Bass |

### Zuordnung Sound zu Funktionstasten
| Bezeichnung | CV | Wertbereich <br> (default) | Beschreibung |
|---|:--------:|---|---|
| F0 | 130 | 0 - 255 (0) | 0 = kein Sound <br> 1 - 255 = Nummer des Sounds der gespielt werden soll. <br><br> Dabei gilt: <br> 1 - 100, dieser Sound wir aus dem Verzeichnis MP3 abgespielt. <br> 101 - 200, dieser Sound wird aus dem Verzeichnis ADVERT abgespielt. <br> 201 - 240, dieser Sound wird aus dem Verzeichnis MP3 in einem Loop abgespielt. <br> 250 = Mute / Unmute <br> 251 = Volume up <br> 252 = Volume down <br><br> Die Dateien müssen auch jeweils die entsprechende Nummer besitzen. |
| ... | ... | 0 - 255 (0) | |
| F10 | 140 | 0 - 255 (250) | Mute / Unmute |
| F11 | 141 | 0 - 255 (251) | Volume up |
| F12 | 142 | 0 - 255 (252) | Volume down |
| F28 | 158 | 0 - 255 (0) | |
