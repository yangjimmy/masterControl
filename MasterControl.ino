// subtract 4 from each pin number of respective IO
#define MISO 46 //50
#define MOSI 47 //51
#define SCK 48 //52
#define SS 49 //53
#define LDAC 37 // define LDAC digital pin as pin 37

// initialize variables
float voltage = 0;
String inString = "";


// begin the program
void setup() {
  Serial.begin(9600);

  // set up pins
  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(LDAC, OUTPUT);
  //pinMode(RESET, OUTPUT);

  digitalWrite(MOSI, LOW);
  digitalWrite(SCK, LOW);
  // end setup of pins


  // set up registers
  initialize();
  // end setup of registers
}

void loop() {
  readCommand();
}

void setVoltage(float voltage) {
  uint16_t data = (uint16_t) (voltage * 65535 / 10);
  data = data & 0xffff;
  uint8_t first = 0b00010000 | (data >> 12);
  uint16_t second = (data << 4) & (0xffff);

  digitalWrite(SS, LOW);
  sendByte(first);
  sendByte((uint8_t)((second >> 8) & 0xff));
  sendByte((uint8_t)((second) & 0xf0));
  digitalWrite(SS, HIGH);
}

void initialize() {
  digitalWrite(SS, LOW);
  uint8_t high = 0b01000000;
  uint8_t mid = 0b00000000;
  uint8_t low = 0b00000000;
  sendByte(high);
  sendByte(mid);
  sendByte(low);
  digitalWrite(SS, HIGH);

  digitalWrite(SS, LOW);
  uint8_t c_high = 0b00100000;
  uint8_t c_mid = 0b00000000;
  uint8_t c_low = 0b00010010;
  sendByte(c_high);
  sendByte(c_mid);
  sendByte(c_low);
  digitalWrite(SS, HIGH);
}

void sendByte(uint8_t value) {
  //ok
  int i = 0;
  for (i; i < 8; i++) {
    digitalWrite(SCK, HIGH);
    if ((value & 0x80) == 0x80) {
      digitalWrite(MOSI, HIGH);
    } else {
      digitalWrite(MOSI, LOW);
    }
    value = value << 1;
    digitalWrite(SCK, LOW);
  }
}

void readCommand() {
  float voltage;
  float pressure;

  while (Serial.available() > 0) {
    int inChar = Serial.read();

    if (inChar != (int)'\n') {
      // As long as the incoming byte
      // is not a newline,
      // convert the incoming byte to a char
      // and add it to the string
      Serial.println("incoming char");
      Serial.println((char)inChar);
      inString += (char)inChar;
      Serial.println("current string");
      Serial.println(inString);
    }
    // if you get a newline, break
    else {
      switch (inString.charAt(0))
      {
        // voltage command
        case ('v'):
        case ('V'):
          Serial.println("End of command");
          Serial.println("Entire string");
          Serial.println(inString);
          voltage = (inString.substring(1)).toFloat();
          Serial.println("first char");
          Serial.println(inString.substring(0));
          Serial.println("result");
          Serial.println(voltage);
          setVoltage(voltage);
          break;
        // pressure command
        case ('p'):
        case ('P'):
          Serial.println("End of command");
          Serial.println("Entire string");
          Serial.println(inString);
          pressure = (inString.substring(1)).toFloat();
          Serial.println("first char");
          Serial.println(inString.substring(0));
          Serial.println("result");
          Serial.println(pressure);
          setPressure(pressure);
          break;
      }
      inString = "";
    }
  }
}

void setPressure(float pressure) {
  float voltage;
  // error check
  if (pressure > 35) {
    Serial.println("Error, resetting to 35");
    pressure = 35;
  } else if (pressure < 0) {
    Serial.println("Error, resetting to 0");
    pressure = 0;
  } else {
    // no error
  }
  uint16_t data = (pressure + 0.3)/35*65535;
  data = data & 0xffff;
  uint8_t first = 0b00010000 | (data >> 12);
  uint16_t second = (data << 4) & (0xffff);

  digitalWrite(SS, LOW);
  sendByte(first);
  sendByte((uint8_t)((second >> 8) & 0xff));
  sendByte((uint8_t)((second) & 0xf0));
  digitalWrite(SS, HIGH);
}
