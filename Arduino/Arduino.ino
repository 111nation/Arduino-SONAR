#include <Servo.h>

// CONNECTIONS
// SERVO 
const int SERVO_PIN = 8;
// PROXIMITY SENSOR
const int PROX_ECHO = 10;
const int PROX_TRIG = 9;

const int DELAY = 5;
const int NORESPONSE = 1;
const int TIMEOUT = 1;
const int MAX_READ = 26;

const int MAX_PROX = 300;

const double SPEED_OF_SOUND = 0.034;

class SONAR { 
  private:
  Servo motor;
  bool confirmation = false;

  // TEXT STRUCTURE
    // Arduino -> Program
    static const char cBEGIN = '#';
    static const char cDELIM = ':';
    
    // Program -> Arduino
    static const char cMSG = '!';
    static const char cEND = '\n';

  void UpdateDisplay();
  bool Read();
  bool Parse();

  // Proximity sensor logic
  void Init_Prox();
  void GetProx();

  public: 
  String msg = "";
  unsigned int deg = 0;
  unsigned int prox = 0;
  
  bool clockwise = true;  

  void Init();
  void Update();
};

SONAR SONAR;

//===================================
//              MAIN
//===================================
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(TIMEOUT);

  SONAR.Init();
}

void loop() {
  SONAR.Update();

  delay(DELAY);
}

//===================================
//           DECLARATIONS
//===================================
void SONAR::Init() {
  // Initilization
  motor.attach(SERVO_PIN);
  pinMode(PROX_ECHO, INPUT);
  pinMode(PROX_TRIG, OUTPUT);
  
  Init_Prox();

  deg = 0;
  prox = 0;
  clockwise = true;
  motor.write(deg); // Initialize to zero degrees
  UpdateDisplay();
}

void SONAR::Init_Prox() {
  digitalWrite(PROX_TRIG, LOW); 
  delayMicroseconds(2);
}

void SONAR::GetProx() {
  // SEND TRIGGER
  Init_Prox();
  digitalWrite(PROX_TRIG, HIGH);
  delayMicroseconds(10); // Keep on for atleast 10 microseconds to register trigger
  digitalWrite(PROX_TRIG, LOW);

  // ECHO RESULT
  // Gets how long trigger pin was on for
  double duration = pulseIn(PROX_ECHO, HIGH, 10000);
  int distance = ((duration * SPEED_OF_SOUND) / 2.0);
  
  prox = constrain(distance, 0, MAX_PROX); // Constrain to maximum and minimum expected proximity reading
}

void SONAR::Update() {
  // Sees if main program responeded
  if (Read()) {
    // Tries retelling program of current degree
    delay(NORESPONSE);
    UpdateDisplay();
    return;
  }

  // Gets proximity reading
  GetProx();

  // Checks if direction needs to be changed
  deg = constrain(deg, 0, 180);
  if (deg <= 0) {
    clockwise = true;
  }

  if(deg >= 180) {
    clockwise = false;
  }   

  // CLOCKWISE
  if (clockwise == true) {
    ++deg;
  } else {
    --deg;
  }

  // Moves the servo
  motor.write(deg);

  UpdateDisplay();
}


//=======================
//     I/O OPERATIONS 
//=======================
void SONAR::UpdateDisplay() {
  String msg = String(cBEGIN) + String(deg) + String(cDELIM) + String(prox) + String(cEND);

  Serial.print(msg); 
}

bool SONAR::Read() {
  // Reads serial port for data coming in & get
  msg = "";
  if (Serial.available()) {
    msg = Serial.readString();
  } else {
    return true; // No message
  }

  if (msg.length() > MAX_READ) {
    return true; // Too long
  }

  // Parses message
  if (Parse()) {
    return true; // unsuccessful
  } else {
    return false; // successful
  }
}

bool SONAR::Parse() {
  // Only copies core of message (remove cMSG and cEND)
  String temp = msg;
  msg = "";

  bool begin = false;
  bool end = false;
  bool delim = false;
  for (unsigned int i = 0; i < temp.length(); i++) {
    switch (temp[i]) {
      case cMSG:
        begin = true;
        break;

      case cEND:
        if (begin) {
          end = true;
        } 
        break;
      
      case cDELIM:
        if (begin && !end) {
          msg += temp[i];
          delim = true;
        }
        break;

      default: 
        if (begin && !end) {
          msg += temp[i];
        }
        break;

    }
    
    if (end) {
      break;
    }
  }
  
  msg.trim();

  // Checks if message completed fully
  if (begin && end && !(msg == "") && delim) {
    // Extracts degrees and proximity   
    String sDeg = "";
    String sProx = "";

    const size_t delim_pos = msg.indexOf(cDELIM);

    sDeg = msg.substring(0, delim_pos);
    sProx = msg.substring(delim_pos+1);

    deg = sDeg.toInt();
    prox = sProx.toInt();

    deg = constrain(deg, 0, 180);
    prox = constrain(prox, 0, MAX_PROX);

    return false; // Successful conversion
  } else  {
    return true; // Unsuccessful conversion
  }
}