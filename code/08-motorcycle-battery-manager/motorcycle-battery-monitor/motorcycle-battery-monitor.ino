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
const int SPREAD_FACTOR = 8;

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
// Statistics
//**********************************************************
long unsigned numberOfSendMessage = 0;
long unsigned numberOfFailedMessages = 0;

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

  // Alle led's uit
  digitalWrite(LED_GREEN, HIGH); 
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH); 
  
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
  delay(5000);
}

//**********************************************************
// De loop van Arduino, deze blijft telkens herhalen
//    - Hier bepalen we de sensorwaarden en versturen
//      we die vervolgens via LoRaWAN
//**********************************************************
void loop()
{
  debugSerial.println("---------------------------------------------");
  debugSerial.println("Taking measurements ...");

  // Periodiek temperatuur sensor uitlezen
  ready_controller_stats();
  ready_temperature_for_sending();
  ready_battery_for_sending();
  ready_cooling_water_level();
  ready_motorcycle_state();

  // Verzenden met LoRaWAN
  digitalWrite(LED_BUILTIN, HIGH);
  send_with_lorawan(18);     // 6 bytes in buffer

  // Tijd om te wachten (milliseconden)
  delay(20000);
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
      sum += analogRead(VOLTAGE_PIN) * REF_VOLTAGE / ((1 << ADC_BITS)) * VOLTAGE_DIVIDER;
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

void ready_cooling_water_level() {
  buffer[6] = 94;
}

void ready_motorcycle_state() {
  const static unsigned int RANDOM_TIME_MIN = 3;
  const static unsigned int RANDOM_TIME_MAX = 5;
  static bool isMotorCycleMoving = false;
  static long movingTimeLeft = 0;
  static long stoppingTimeLeft = random(RANDOM_TIME_MIN, RANDOM_TIME_MAX);
  unsigned int speed = 0;

  debugSerial.println("Motorcycle status:");
  if (!isMotorCycleMoving) {
    debugSerial.print("    => Standing still. Time left = ");
    debugSerial.println(stoppingTimeLeft);
    stoppingTimeLeft--;
    if (stoppingTimeLeft == 0) {
      movingTimeLeft = random(RANDOM_TIME_MIN, RANDOM_TIME_MAX);
      isMotorCycleMoving = true;
    }
  } else {
    debugSerial.print("    => On the move. Time left = ");
    debugSerial.println(movingTimeLeft);
    speed = random(0, 140);
    movingTimeLeft--;
    if (movingTimeLeft == 0) {
      stoppingTimeLeft = random(RANDOM_TIME_MIN, RANDOM_TIME_MAX);
      isMotorCycleMoving = false;
      speed = 0;
    }
  }

  debugSerial.print("    => Current speed = ");
  debugSerial.print(speed);
  debugSerial.println(" km/h");

  buffer[7] = (isMotorCycleMoving ? 0xFF : 0x00);
  buffer[8] = (speed >> 8) & 0xFF;
  buffer[9] = (speed >> 0) & 0xFF;
}

void ready_controller_stats() {
  debugSerial.println("Stats:");
  debugSerial.print("   # verzonden berichten: ");
  debugSerial.println(numberOfSendMessage);
  debugSerial.print("   # gefaalde berichten: ");
  debugSerial.println(numberOfFailedMessages);


  buffer[10] = (numberOfSendMessage >> 24) & 0xFF;
  buffer[11] = (numberOfSendMessage >> 16) & 0xFF;
  buffer[12] = (numberOfSendMessage >> 8) & 0xFF;
  buffer[13] = (numberOfSendMessage >> 0) & 0xFF;

  buffer[14] = (numberOfFailedMessages >> 24) & 0xFF;
  buffer[15] = (numberOfFailedMessages >> 16) & 0xFF;
  buffer[16] = (numberOfFailedMessages >> 8) & 0xFF;
  buffer[17] = (numberOfFailedMessages >> 0) & 0xFF;
}

//**********************************************************
// WARNING:   Vanaf hier dien je niets meer aan te passen.
//            Dit zijn de functies die de LoRa data verzenden.
//**********************************************************
void indicate_lora_status(bool statusOk) {
  if (statusOk) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  } else {
    digitalWrite(LED_GREEN, HIGH); 
    digitalWrite(LED_RED, LOW);       // Rood aan
  }
}

void setup_lorawan()
{
  indicate_lora_status(false);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  LoRaBee.init(loraSerial, LORA_RESET);
  if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, true)) {
    debugSerial.println("Verbonden met het LoRaWAN netwerk");
    indicate_lora_status(true);
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
      numberOfSendMessage++;
      break;
    case NoResponse:
      debugSerial.println("Er was geen response van de transceiver.");
      numberOfFailedMessages++;
      break;
    case Timeout:
      debugSerial.println("Timed-out. Is de seriele connectie met de transceiver in orde? Er wordt voor 20sec gewacht.");
      numberOfFailedMessages++;
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("Je probeert teveel data te versturen. Verzenden faalt hierdoor.");
      numberOfFailedMessages++;
      break;
    case InternalError:
      debugSerial.println("Interne Error - Dit is ernstig probleem. De LoRaWAN transceiver moet worden gereset.");
      numberOfFailedMessages++;
      setup_lorawan();
      break;
    case Busy:
      debugSerial.println("De transceiver is bezet. Er wordt 10 seconden gewacht.");
      numberOfFailedMessages++;
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("Er doet zich een netwerk error voor. De LoRaWAN transceiver moet worden gereset.");
      numberOfFailedMessages++;
      setup_lorawan();
      break;
    case NotConnected:
      debugSerial.println("Het device is niet verbonden met het netwerk. De applicatie zal de transceiver resetten.");
      numberOfFailedMessages++;
      setup_lorawan();
      break;
    case NoAcknowledgment:
      debugSerial.println("Het bericht werd niet bevestigd.");
      numberOfFailedMessages++;
      break;
    default:
      break;
  }
}
