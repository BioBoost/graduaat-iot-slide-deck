#include <Sodaq_RN2483.h>

#define debugSerial SerialUSB
#define loraSerial Serial2

//**********************************************************
// TODO: verander de waarden van DevEUI, AppEUI en APPkey
//**********************************************************
static uint8_t DevEUI[8] = { 0x00, 0x80, 0xEA, 0x8B, 0xEC, 0x6C, 0x68, 0xB9 };
const uint8_t AppEUI[8] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x02, 0xAF, 0xBD };
const uint8_t AppKey[16] = { 0x86, 0x1D, 0xBD, 0xF2, 0x5F, 0x50, 0xA2, 0xF4, 0x76, 0xDC, 0xDE, 0x8B, 0xD4, 0x88, 0x4D, 0xAD };

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
const int SPREAD_FACTOR = 6;

//**********************************************************
// Configuratie spanninsmeting
//**********************************************************
const int VOLTAGE_PIN = A0;
const double REF_VOLTAGE = 3300;
const double VOLTAGE_DIVIDER = 11.0;
const unsigned int ADC_BITS = 10;
const unsigned int NUMBER_OF_SAMPLES = 20;
const double FULL_VOLTAGE = 10500;      // 10.5V
const double DEPLETED_VOLTAGE = 6300;   // 6.3V
const double VOLTAGE_RANGE = FULL_VOLTAGE - DEPLETED_VOLTAGE;

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
  
  // Configuratie van de spanningsmeting
  pinMode(VOLTAGE_PIN, INPUT);

  // Even wachten op de seriele monitor
  SerialUSB.begin(115200);
  while ((!SerialUSB) && (millis() < 5000));
  debugSerial.println("Starten van LoRaWAN Temperatuur Sensor");

  // Configuratie van LoRaWAN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  setup_lorawan();
}

//**********************************************************
// De loop van Arduino, deze blijft telkens herhalen
//    - Hier bepalen we de sensorwaarden en versturen
//      we die vervolgens via LoRaWAN
//**********************************************************
void loop()
{
  debugSerial.println("Taking measurements ...");

  // Periodiek temperatuur sensor uitlezen
  ready_temperature_for_sending();
  ready_battery_for_sending();

  // Verzenden met LoRaWAN
  digitalWrite(LED_BUILTIN, HIGH);
  send_with_lorawan(5);     // 5 bytes in buffer

  // Tijd om te wachten (milliseconden)
  delay(15000);
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
    debugSerial.print(temperature);
    debugSerial.println("°C");

    // Buffer vullen met onze data (temperatuur)
    buffer[0] = ((int)(temperature * 100) >> 8) & 0xFF;
    buffer[1] = ((int)(temperature * 100) >> 0) & 0xFF;
}

//**********************************************************
// Uitlezen van de battery sensor
//**********************************************************
void ready_battery_for_sending()
{
    double sum = 0;
    for (unsigned int i = 0; i < NUMBER_OF_SAMPLES; i++) {
      sum += analogRead(VOLTAGE_PIN) * REF_VOLTAGE / ((1 << ADC_BITS) - 1) * VOLTAGE_DIVIDER;
      delay(10);
    }
    double voltage = sum / NUMBER_OF_SAMPLES;
    debugSerial.print("Batterij spanning: ");
    debugSerial.print(voltage/1000.0);
    debugSerial.println("V");

    // Batterij percentage berekenen
    int percentage = ((voltage - DEPLETED_VOLTAGE) / VOLTAGE_RANGE) * 100;
    if (percentage < 0) percentage = 0;
    debugSerial.print("Batterij percentage: ");
    debugSerial.print(percentage);
    debugSerial.println("%");

    // Batterij percentage
    buffer[2] = percentage;

    // Batterij toestand
    if (voltage < DEPLETED_VOLTAGE) {
      debugSerial.println("Batterij spanning te laag. Mogelijks gevaar!!");
      buffer[3] = false;  // Ongezonde toestand
    } else {
      debugSerial.println("Batterij spanning ligt binnen de grenzen.");
      buffer[3] = true;  // GEzonde toestand
    }

    // Batterij spanning in buffer
    buffer[4] = ((int)voltage >> 8) & 0xFF;
    buffer[5] = ((int)voltage >> 0) & 0xFF;
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
      digitalWrite(LED_BUILTIN, LOW);
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
