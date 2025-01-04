# Arduino-Projekt-Florin

Projektplan

Hühnerklappe
Es soll eine Hühnerklappe automatisiert werden. Die Klappe wird über ein Seil, das von einem Schrittmotor aufgewickelt wird, bewegt. Die Position wird durch Sensoren erkannt. Über einen Handmodus soll die Klappe per Taster bewegt werden. Die Position, der Modus und Fehler sollen mit LEDs signalisiert werden.


## Muss Kriterien:

- Schrittmotor bewegt Klappe
- Zwei Positionen werden erkannt
- Schalter für Handmodus funktioniert
- Taster für Handmodus funktioniert
- Lichtsensor für Automodus funktioniert
- LED Signalisierung funktioniert

## Wunsch Kriterien:

Wie besprochen gibt es keine Wunschkriterien

## Aufgabenaufteilung

Da ich das Projekt alleine gemacht habe, müssen keine Aufgaben verteilt werden.

## Beschreibbung des Lösungsansatzes

### Verwendetes Material:

Generell habe ich versucht, das Material aus dem gelieferten Set zu verwenden. Bis auf die Sensoren und einige Verbrauchsmaterialien (Widerstände, Kabel) habe ich dies auch gemacht.

Microcontroller
- Arduino UNO R3

![image](https://github.com/user-attachments/assets/6d12e702-9cd9-459d-953d-860e141284f0)

Schrittmotor
- 28BYJ-48
 
 ![Motoranschluesse](https://github.com/user-attachments/assets/2b8889b9-5922-4b87-973b-dc400074144c)

Treiberplatine Schrittmotor
 - ULN2003
 
![Platinenanschluesse](https://github.com/user-attachments/assets/268f0aa4-8ffd-46ee-aaa8-9de1bf0a8755)

LED's

![image](https://github.com/user-attachments/assets/1ace0dfe-84a3-4789-b388-045add65c387)

Taster

![Screenshot 2025-01-03 155550](https://github.com/user-attachments/assets/dcc8cbbb-ffea-46b5-85c1-0ea80276b9ff)

Photoressistor

![Screenshot 2025-01-03 155725](https://github.com/user-attachments/assets/4587366a-98b7-4d29-8cee-7cd70cabea44)

Sensoren
- mk04-1a66b-500w

![Screenshot 2025-01-03 160841](https://github.com/user-attachments/assets/d6d70d0d-95da-40cb-bfa9-f6e6d0ce7469)

### Mein Ansatz:

Zuallererst war es wichtig, die benötigten Funktionen des Projektes zu definieren. Da mein Projekt eine Hühnerklappe für meine Eltern ist, war es wichtig, zuerst Absprache mit ihnen zu halten.
Die grundlegende Idee war, einen Motor einzusetzen, welcher durch Drehung im Uhrzeigersinn oder im Gegenuhrzeigersinn eine Schnur, welche an der Hühnerklappe befestigt ist, auf- und abwickelt. Dies soll das Öffnen und Schließen der Hühnerklappe ermöglichen.
Um den Motor zu stoppen, wenn die Klappe offen oder geschlossen ist, sollten zusätzlich Sensoren verwendet werden, welche die Position der Hühnerklappe detektieren. Diese Position wird jeweils mit einer weißen LED signalisiert.
Die Klappe soll sich in zwei unterschiedlichen Modi öffnen und schließen. Standardmäßig soll mittels eines Photoresistors erkannt werden, ob Tag oder Nacht ist. Bei Tag öffnet sich die Klappe und bei Nacht schließt sie sich automatisch. Dies ist der Automatikmodus, dessen Aktivierung mit einer blauen LED signalisiert wird.
Mit einer weiteren blauen LED wird signalisiert, dass der Handmodus aktiv ist, in welchem durch Drücken eines Tasters die Position der Klappe jeweils in die andere wechselt (offen zu geschlossen und umgekehrt). Zwischen den beiden Modi kann mit einem weiteren Taster hin- und hergewechselt werden. Falls die Klappe z. B. abreißt und für eine zu lange Zeit nicht die andere Position erreicht, wird der Motor gestoppt und die rote LED aktiviert sich. 


## Flussdiagramm

[Flussdiagramm Ardunino.pdf](https://github.com/user-attachments/files/18305349/Flussdiagramm.Ardunino.pdf)


## Testen
Muss Kriterien:

### Schrittmotor bewegt Klappe

Um zu testen, muss sich der Motor entweder durch Statusänderung des Photoresistors oder durch Betätigung des Positionstasters aktivieren. Die Bewegung muss entsprechend der Logik entweder im Uhrzeigersinn oder im Gegenuhrzeigersinn erfolgen. Zusätzlich muss sich der Motor nach Erreichen der zweiten Position stoppen.
Dies wurde getestet und ist in den Videos klar ersichtlich.

### Kriterien Start

**Automodus (Modus 1)**

**Drehung Uhrzeigersinn (Öffnen)**

- Modus 1 = True
- Licht = Tag
- Sensor unten = True
- Sensor uben = False

**Drehung Gegenuhrzeigersinn (Schliessen)**

- Modus 1 = True
- Licht = Nacht
- Sensor unten = False
- Sensor oben = True

**Handmodus (Modus 2)**

**Drehung Uhrzeigersinn (Öffnen)**
- Modus 2 = True
- Taster Position = True
- Sensor unten = True
- Sensor oben = False

**Drehung Gegenuhrzeigersinn (Schliessen)**

- Modus 2 = TrueTaster 
- Position = True
- Sensor Unten = False
- Sensor Oben = True

### Kriterien Stop

**Drehung Uhrzeigersinn (Öffnen)**
- Motor Status = Drehung Uhrzeigersinn
- Sensor oben = True

**Drehung Gegenuhrzeigersinn (Schliessen)**
- Motor Status = Drehung Gegenuhrzeigersinn
- Sensor unten = True


### Zwei Positionen werden erkannt
Erreicht die Klappe die beiden Sensoren, müssen diese die Position erkennen und das entsprechende Signal an die Steuerung weitergeben.
Dies wurde getestet und ist in den Videos klar ersichtlich.

Ursprünglich wurde besprochen, dass Induktivsensoren verwendet werden. Die von mir verwendeten Sensoren sind Magnetschließer, welche ich fälschlicherweise für Induktivsensoren gehalten habe. Dies wurde jedoch erst beim Durchlesen dieses Readme durch Zweitpersonen entdeckt. Da ich aus Zeitgründen keine Induktivsensoren mehr beschaffen konnte, habe ich mich entschieden, hier zu zeigen, wie ich es gelöst hätte.

- Zu erst müssen die Pins definiert werden

![image](https://github.com/user-attachments/assets/7cdc31eb-4bc0-4498-9f25-57239b113eb1)

- Es wird eine Konstante definiert, welche den Schwellenwert für beide Sensoren definiert. Dies könnte für beide Sensoren einzeln gemacht werden, ist jedoch hier nicht nötig.

![image](https://github.com/user-attachments/assets/5bdee1cc-cfc6-4c24-93aa-b5da1f2371d1)

- Es werden Variablen für beide Sensoren festgelegt, welche bei Erreichen des Schwellenwerts auf true gesetzt werden.

![image](https://github.com/user-attachments/assets/ae076d8d-97f8-4743-b57c-a931c313f741)

- Es werden die beiden Funktionen zustandInduktivSensoroben und zustandInduktivSensorunten deklariert. 

![image](https://github.com/user-attachments/assets/52d6b50c-281a-46fe-a465-a9648f1daaa7)

- Da es sich um analoge Eingänge handelt und später die Funktion analogRead verwendet wird, müssen die Eingänge nicht konfiguriert werden.

- Die beiden Funktionen werden abgerufen. 

![image](https://github.com/user-attachments/assets/1e825364-0007-4d37-8105-8dc2c49c4b96)

- Bei der Abfrage der Induktivsensoren wird mit der Funktion analogRead der analoge Wert des jeweiligen Eingangs in Variablen induktivWertoben und induktivWertunten überschrieben. Die Variablen induktiverkanntoben und induktiverkanntunten werden auf true gesetzt, wenn der ausgelesene Wert den Schwellenwert überschreitet.

![image](https://github.com/user-attachments/assets/e405ac01-5959-495b-824c-21ce598f3867)

- In den Funktionen zum Öffnen und Schließen der Klappe werden nun die Variablen induktiverkanntoben und induktiverkanntunten verwendet.

![image](https://github.com/user-attachments/assets/8fbb76d8-6b0f-472f-8bcb-df49cc73ee26)


### Schalter für Handmodus funktioniert
Statt wie anfangs angedacht, habe ich einen Taster verwendet, welcher zwischen den beiden Modi hin- und herwechselt. Der Taster muss bei Betätigung den Modus in den jeweils anderen wechseln.
Dies wurde getestet und ist in den Videos klar ersichtlich.


### Taster für Handmodus funktioniert
Im Handmodus muss der Taster für den Positionswechsel funktionieren und die Klappe öffnen und wieder schließen.
Dies wurde getestet und ist in den Videos klar ersichtlich.

### Lichtsensor für Automodus funktioniert
Der Photoresistor muss detektieren, ob Tag oder Nacht ist, und das entsprechende Signal an die Steuerung weitergeben.
Dies wurde getestet und ist in den Videos klar ersichtlich.

### LED Signalisierung funktioniert
Die entsprechenden Modi, Positionen und die Zeitüberschreitung müssen mittels LEDs signalisiert werden.
Dies wurde getestet und ist in den Videos klar ersichtlich.

## Videos
Sämtliche Videos sind unter diesem Link zu finden. 
https://www.dropbox.com/scl/fo/u0rw0w13u6se14yuvlmyl/AETGqyAwuBVfIsyqLIkZcvA?rlkey=89beq2ghppip8bfxroa89nqne&st=l7esdeb7&dl=0

- Das Video Mode Switch zeigt das Wechseln zwischen Hand- und Automodus durch Drücken eines Tasters und die Signalisierung mittels zweier LEDs.

- In den Videos Mod1 Cw und Mod2 Cw wird die Abschaltung nach Zeitüberschreitung und deren Signalisierung mittels LED gezeigt. Der Schwellenwert für das Timeout wurde hier auf 10 Sekunden heruntergesetzt. In der Praxis beträgt er 45 Sekunden.

- Die Videos Mode 1 Zu, Mode 1 Auf, Mode 2 Zu und Mode 1 Auf zeigen die Funktion im eingebauten Zustand. Hierbei wurde die rote LED zur Signalisierung des Licht-Statusses verwendet. Dies wurde im Nachhinein angepasst.

