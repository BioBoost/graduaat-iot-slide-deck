#include <Sodaq_RN2483.h>

#define debugSerial SerialUSB
#define loraSerial Serial2

//**********************************************************
// TODO: verander de waarden van DevEUI, AppEUI en APPkey
//**********************************************************
static uint8_t DevEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t AppEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t AppKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

//**********************************************************
// TODO: De poort waarop de data wordt verzonden
//       Andere poort per type sensor
//**********************************************************
const int LORAWAN_PORT = 1;

//**********************************************************
// WARNING:   Niet aanpassen. Maakt de buffer voor data.
//            De grootte mag worden aangepast als je meer
//            data wil verzenden.
//**********************************************************
const int SIZE_OF_BUFFER = 32;
uint8_t buffer[SIZE_OF_BUFFER];

//**********************************************************
// De spread factor waarmee we zenden
//**********************************************************
const int SPREAD_FACTOR = 7;

//**********************************************************
// De setup van Arduino, wordt in het begin 1x uitgevoerd.
//    - Hier initialiseren we alles (LEDs, Seriele poort,
//      LoRa transceiver)
//**********************************************************
void setup()
{
  // Configuratie van de gekleurde LEDs
  pinMode(LED_RED, OUTPUT);         // Rode LED als uitgang
  pinMode(LED_GREEN, OUTPUT);       // Groene LED als uitgang

  digitalWrite(LED_GREEN, HIGH);    // Groen uit
  digitalWrite(LED_RED, LOW);       // Rood aan
  
  // Even wachten op de seriele monitor
  SerialUSB.begin(115200);
  while ((!SerialUSB) && (millis() < 5000));
  debugSerial.println("Starten van LoRaWAN Temperatuur Sensor");

  // Configuratie van LoRaWAN
  setup_lorawan();
}

//**********************************************************
// De loop van Arduino, deze blijft telkens herhalen
//    - Hier bepalen we de sensorwaarden en versturen
//      we die vervolgens via LoRaWAN
//**********************************************************
void loop()
{
  // Periodiek temperatuur sensor uitlezen
  ready_temperature_for_sending();

  // Verzenden met LoRaWAN
  send_with_lorawan(2);     // 2 bytes in buffer

  // Tijd om te wachten (milliseconden)
  delay(10000);
}

//**********************************************************
// Uitlezen van de temperatuur sensor
//**********************************************************
void ready_temperature_for_sending()
{
    //10mV per C, 0C is 500mV
    float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.0 / 1023.0;
    float temperature = (mVolts - 500.0) / 10.0;

    // Uitschrijven in console
    debugSerial.print("Huidige temperatuur: ");
    debugSerial.println(temperature);

    // Buffer vullen met onze data (temperatuur)
    buffer[0] = ((int)(temperature * 100) >> 8) & 0xFF;
    buffer[1] = ((int)(temperature * 100) >> 0) & 0xFF;
}

//**********************************************************
// WARNING:   Vanaf hier dien je niets meer aan te passen.
//            Dit zijn de functies die de LoRa data verzenden.
//**********************************************************
void setup_lorawan()
{
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  LoRaBee.init(loraSerial, LORA_RESET);
  if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, true)) {
    debugSerial.println("Verbonden met het LoRaWAN netwerk");
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  }
  else {
    debugSerial.println("Connectie met LoRaWAN netwerk gefaald. Zijn de keys ok?");
  }
  LoRaBee.setSpreadingFactor(SPREAD_FACTOR);
}

void send_with_lorawan(unsigned int size) {
  if (size > SIZE_OF_BUFFER) {
    debugSerial.println("Buffer is kleiner dan het aantal bytes dat je wil verzenden");
    return;
  }

  switch (LoRaBee.send(LORAWAN_PORT, buffer, size))
  {
    case NoError:
      debugSerial.println("Packet succesvol verzonden");
      break;
    case NoResponse:
      debugSerial.println("Er was geen response van de transceiver.");
      break;
    case Timeout:
      debugSerial.println("Timed-out. Is de seriele connectie met de transceiver in orde? Er wordt voor 20sec gewacht.");
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("Je probeert teveel data te versturen. Verzenden faalt hierdoor.");
      break;
    case InternalError:
      debugSerial.println("Interne Error - Dit is ernstig probleem. De LoRaWAN transceiver moet worden gereset.");
      setup_lorawan();
      break;
    case Busy:
      debugSerial.println("De transceiver is bezet. Er wordt 10 seconden gewacht.");
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("Er doet zich een netwerk error voor. De LoRaWAN transceiver moet worden gereset.");
      setup_lorawan();
      break;
    case NotConnected:
      debugSerial.println("The device is not connected to the network. The program will reset the RN module.");
      setup_lorawan();
      break;
    case NoAcknowledgment:
      debugSerial.println("Het bericht werd niet bevestigd door.");
      break;
    default:
      break;
  }
}
