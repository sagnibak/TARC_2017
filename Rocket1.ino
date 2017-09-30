/*
Made by Sagnik Bhattacharya on September 27, 2017, for the
Campolindo TARC team.
*/

const int RELEASE_TIME = ;
const int SERVO_PARA_CLOSED = ;
const int SERVO_PARA_RELEASED = ;
const int SERVO_PIN = ;

int altitude;
int servoPosition;
int elapsedTime;

void setup() {
    // altitude
    pinMode(SERVO_PIN, SERVO_PARA_CLOSED);
}

void loop() {
    if (elapsedTime >= RELEASE_TIME) {
      servoPosition = SERVO_PARA_RELEASED;
    }
}
