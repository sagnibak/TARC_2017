#include <Servo.h>
//#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

/*
Made by Sagnik Bhattacharya on September 27, 2017, for the
Campolindo TARC team.
*/
//***************************************
// Many constants have placeholder values
//***************************************
//const int RX_PIN = 5;                                    // Arduino RX, connects to bluetooth TX, for software serial ONLY
//const int TX_PIN = 6;                                    // Arduino TX, connects to bluetooth RX, may need a voltage divider or level shifter, for software serial ONLY
const int RELEASE_ALTD = 1000;                                 // Altitude at which parachute should be released, in units metres
const int BACKUP_RELEASE_ALTD = 10;                        // Altitude at which parachute will be released as a backup
const int SERVO_PARA_CLOSED = 1;                            // servo position at which the parachute is locked
const int SERVO_PARA_RELEASED = 1;                          // servo position at which the parachute is released
const int SERVO_PIN = 3;
const float TIME_INTERVAL = 100;
const float AIR_MASS_DENSITY = 100; //mass density of the air
const float WEIGHT = 420;                                        // weight of the rocket NOT MASS THIS SHOULD BE IN NEWTONS REEEEEEEEEEE
const float CdA = .69;                                           // CdA that is used to calculate ejection
const float VELOCITY_TERMINAL = sqrt(2 * WEIGHT / (CdA * AIR_MASS_DENSITY)); // I realize this might be bad practice, but this is how I'm doing it for now
const float VELOCITY_THRESHOLD = 1;// pin to which the servo signal is attahced

float altitude;                                            // current altitude, read from the sensor
float prevAltitude;                                            
float currAltitude;
unsigned long startTime;
unsigned long endTime;
unsigned long elapsedTime = 41000;
unsigned long time1 = 0;
unsigned long time2;
int servoPosition;                                         // position of the servo
bool timerStart;
bool timerEnd;
bool apogee;
bool isParaReleased = false;                               // boolean to keep track of the parachute's status
float currentCdA; //the current CdA (coefficient of drag * area)
float velocity; //current vertical velocity TODO: make actually work
float acceleration; //current vertical accelion TODO: make actually work
unsigned long timeUntilGoal;                              // time IN MILLISECONDS until the goal time to reach the ground


Adafruit_MPL3115A2 altimeter = Adafruit_MPL3115A2();       // altimeter object
Servo paraServo;                                           // Servo object to control the servo holding the parachute
//SoftwareSerial bt(RX_PIN, TX_PIN);                       // for software serial, if needed

void setup() {
    Serial.begin(115200);                                  // start bluetooth serial communication

    paraServo.attach(SERVO_PIN);                           // connect the servo to SERVO_PIN
    paraServo.write(SERVO_PARA_CLOSED);                    // make sure that the parachute is initially closed
                                                                
    // debug messages                                                                   
    Serial.println("Rocket initialized.");                           
    Serial.print("Parachute released: ");                       
    Serial.println(isParaReleased);                        // should say `false`

    // if we need to use software serial for bluetooth
//    bt.begin(115200); 
//    bt.println("Rocket initialized.");
altimeter.setSeaPressure(altimeter.getPressure());
prevAltitude = altimeter.getAltitude();
}

void loop() {
    // check if the altimeter is connected
    if (! altimeter.begin()) {
        Serial.println("Couldnt altimeter");
        return;
    }

    altitude = altimeter.getAltitude();                     // read the altimeter
    Serial.print("The current altitude is: ");
    Serial.print(altitude);                                 // print the current altitude
    Serial.println("metres");

time2 = millis()*1000;
                                                      
    if(time2 - time1 > TIME_INTERVAL){
      currAltitude = altimeter.getAltitude();
      velocity = (currAltitude - prevAltitude)/(time2 - time1);
      time1 = time2;
      prevAltitude = currAltitude;
    }  
    if(abs(velocity) > VELOCITY_THRESHOLD){
          if(!timerStart) {
              startTime = millis()*1000 - time2;
              timerStart = true;
           }
      apogee = velocity < 0;
    }
    if((apogee && velocity == 0) && !timerEnd) {
        endTime = millis()*1000 - startTime;
        timerEnd = true;
      }

    // if the altitude is less or equal than to the calculated release altitude, release the parachute
    if (altitude <= VELOCITY_TERMINAL * timeUntilGoal || altitude <= BACKUP_RELEASE_ALTD) {
        servoPosition = SERVO_PARA_RELEASED;                // release the parachute
        isParaReleased = true;                              // tell the code that the parachute is released
        
        Serial.print("It is ");                             // a crafty debug message
        Serial.print(isParaReleased);                       // true or false
        Serial.print(" that the parachute has been");
        Serial.print(" released at an altitude of ");
        Serial.print(altitude);
        Serial.println(" metres.");
    }

    //Calculate current CdA
    currentCdA = 2 * WEIGHT / (velocity * velocity * AIR_MASS_DENSITY);

    delay(50);                                              // repeat 20 times every second (feel free to tune this)
}
