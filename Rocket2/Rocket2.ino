#include <Servo.h>
//#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

/*
Made by Sagnik Bhattacharya on September 27, 2017, for the
Campolindo TARC team.
*/

//const int RX_PIN = 5;                                    // Arduino RX, connects to bluetooth TX, for software serial ONLY
//const int TX_PIN = 6;                                    // Arduino TX, connects to bluetooth RX, may need a voltage divider or level shifter, for software serial ONLY
const int RELEASE_ALTD = ;                                 // Altitude at which parachute should be released, in units metres
const int SERVO_PARA_CLOSED = ;                            // servo position at which the parachute is locked
const int SERVO_PARA_RELEASED = ;                          // servo position at which the parachute is released
const int SERVO_PIN = 3;                                   // pin to which the servo signal is attahced
const float AIR_MASS_DENSITY;                              //mass density of the air
const float MASS;                                          //mass of the rocket

float altitude;                                            // current altitude, read from the sensor
int servoPosition;                                         // position of the servo
bool isParaReleased = false;                               // boolean to keep track of the parachute's status
float currentCdA;                                          // the current CdA the rocket experiences(coefficient of drag * area)
float usingCdA;                                            // the CdA the rocket is actually using for its calcutions (needs to be entered based on experimental results)

float velocity;                                            // current vertical velocity TODO: make actually work
float acceleration;                                        // current vertical accelion TODO: make actually work
//int elapsedTime;                                         // if we want to keep track of time
float timeToGoalTime;                                      // the time until the goal time

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

    // calculate descent velocity when parachute is deployed
    
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

    // if the time it will take to hit the ground with the parachute deployed is less than the time we need to be in the air, release the parachute
    if ((altitude / descentVelocity) > timeToGoalTime) {
      // do the parachute release stuff, like in the other if statement
    }

    // if the altitude is greater than the required altitude, release the parachute
    if (altitude >= RELEASE_ALTD || isParaReleased == true) {
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
    currentCdA = 2 * abs(MASS * acceleration) / (AIR_MASS_DENSITY * velocity * velocity);

    delay(50);                                              // repeat 20 times every second (feel free to tune this)
}
