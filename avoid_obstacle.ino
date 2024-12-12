  // H-Bridge motor control information
//from: https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/
// Motor A connections
// orig int enA = 9;
int enA = 12;
//orig int in1 = 8;
//orig int in2 = 7;
int in1 = 8;
int in2 = 9;
// Motor B connections
//orig int enB = 3;
//orig int in3 = 5;
//orig int in4 = 4;
int enB = 3;
int in3 = 10;
int in4 = 11;

// for ultrasonic sensor
int led_pin = 13;
const int TriggerPin = 6;
const int EchoPin = 5 ;
// orig const int TriggerPin = 8;
//orig const int EchoPin = 9 ;

// global variables for control
int CountStuck = 0;
long Duration = 0;
long Distance_cm = 1 ;
long Distance_left_cm = 1 ;
long Distance_right_cm = 1 ;
int DirectionState = 0; // 0 is forward, 1 is reverse

void setup() {
	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
  motor_off();

  // for ultrasonic sensor
  
  pinMode(led_pin, OUTPUT); // led_pin is an output signal
  pinMode(TriggerPin, OUTPUT); // ultrasonic sensor trigger is output
  pinMode(EchoPin, INPUT); // ultrasonic sensor echo is an input

  Serial.begin(9600); // serial output at 9600 baud rate

}

void loop() {
//	directionControl();
//	delay(1000);
//	speedControl();
//	delay(1000);
//  motor_forwards();

  // start with motors off
  motor_off();

  // call functions to find distance
  Distance_cm = Distance( ultrasonicMeasure() );

// to control choosing what program the mobile robot 
// will run, use the initial sensed distance to pick 
// function

if (Distance_cm < 10)
{ // run first code option}
Obstacle_avoid();
//Car_Box();
}
else
{ // if long distance, run second code option}
Obstacle_avoid();
}

} // end of loop


void Car_Box() {
// make your car drive a box
while (CountStuck <50)
{
// control robot with functions here
motor_off();
// can you make it travel in a rectangle? Octagon? Figure 8?

// add your code here


// next block of code is to stop robot if sensor 
// repeatedly says it is stuck
  // call functions to find distance
  Distance_cm = Distance( ultrasonicMeasure() );
  if(Distance_cm < 3)
  { // obstacle is here
  // turn around and drive backwards
	// Now change motor directions
  motor_backwards();
 	delay(100);
   // maybe robot is stuck
   CountStuck++ ;
  }
  else
  { // not stuck
    CountStuck =0;
  }
} // end of while CountStuck<100

if (CountStuck>49) {motor_off(); exit(0);} // stop robot if stuck

} // end of Car_box function

void Obstacle_avoid() {

while (CountStuck <10)
{
  // call functions to find distance
  Distance_cm = Distance( ultrasonicMeasure() );

if(Distance_cm < 4)
{ // obstacle is here
  // turn around and drive backwards
	// Now change motor directions
  motor_backwards();
 	delay(400);
   CountStuck++ ;
}
else if (Distance_cm > 10)
{ // go forward at speed
	// Turn on motor A & B
  CountStuck = 0;
  motor_forwards();
 	delay(200); // drive for a little time
}  else 
{ // obstacle approaching
  // check way to proceed
//  motor_off();
  CountStuck = 0;

  // rotate and check distance
  // rotate
  motor_left_rotate(200);
  
  // Now turn off motors
  motor_off();
  Serial.println("Obstacle: Checking Distance--"); // transmit print statement to computer console
  Serial.print("Obstacle: Left "); // transmit print statement to computer console

  // call functions to find distance
  Distance_left_cm = Distance( ultrasonicMeasure() );

  // rotate and check distance
  // rotate other way
  // enough time to rotate back to initial
  motor_right_rotate(400);
  Serial.print("Obstacle: Right "); // transmit print statement to computer console

  // call functions to find distance
  Distance_right_cm = Distance( ultrasonicMeasure() );

// if distance_right is more then go the way that we are pointing
if (Distance_right_cm > Distance_left_cm)
{ // go forward at speed
	// Turn on motor A & B
  motor_forwards();
 	delay(400); // drive for a little time
} else
{ // rotate left and start moving forward}
// rotate left
  motor_left_rotate(400);
 // now go forward
  DirectionState = 0; // forward
 motor_forwards();
 	delay(500); // drive for a little time
} // End distance left vs right

} // end of else (not forward or backward travel) 

  } // end of while CountStuck < 10

if (CountStuck>9)  
{ // terminate running if stuck too long
   motor_off();
   exit(0);
   }
} // end of obstacle_avoid

float ultrasonicMeasure() {
  // returns duration of sensor pulse
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);

  // measure duration of pulse from ECHO pin
  float duration_us = pulseIn(EchoPin, HIGH);

  // calculate the distance
  // float distance_cm = 0.017 * duration_us;

  return duration_us;
}

  // function to compute distance from ultrasonic sensor time delay
  long Distance(long time)
  { long DistanceCalc;  // local variable for distance calculation

  //DistanceCalc = ((time * 0.034) / 2.0 ); // calculation in centimeters
  DistanceCalc = ((time/74.0) /2.0); // calculation in inches

  Serial.print("Distance ="); // transmit print statement to computer console
  Serial.print(DistanceCalc);
  Serial.println(" cm"); // output with line-return
  
  return DistanceCalc ; // return calculated distance
  
  }


// This function  controls motors - set motors to off
void motor_off() {
	// Set motors tooff 
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

// This function  controls motors - set motors to backwards
void motor_backwards() {
	// Set motors go backwards 

  DirectionState = 1; // both backwards
  // left motor backwards
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
  // right motor backward
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
}
// This function  controls motors - set motors to backwards

void motor_forwards() {
	// Set motors go forwards 
  // Turn on motor A & B
  DirectionState = 0; // both forward
	//left motor forward
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
    // right motor forward
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);

}

void motor_right_rotate(unsigned int time) {
  // rotate
  // left motor backwards
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
  // right motor forward
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);

  //travel a little
//  delayMicroseconds(time); // delay time us while high
  delay(time); // delay time ms while high

  	// Now turn off motors
  motor_off();
  
}


void motor_left_rotate(unsigned int time) {
  	// Now turn off motors
  motor_off();

  // rotate
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);

  //travel a little
  delayMicroseconds(time); // delay time us while high
  delay(time); // delay time ms while high
	// Now turn off motors
  motor_off();
}


// This function lets you control spinning direction of motors
void directionControl() {
	// Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	analogWrite(enA, 255);
	analogWrite(enB, 255);

	// Turn on motor A & B
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	delay(200);
	
	// Now change motor directions
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	delay(200);
	
	// Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

// This function lets you control speed of the motors
void speedControl() {
	// Turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	
	// Accelerate from zero to maximum speed
	for (int i = 0; i < 256; i++) {
		analogWrite(enA, i);
		analogWrite(enB, i);
		delay(20);
	}
	
	// Decelerate from maximum speed to zero
	for (int i = 255; i >= 0; --i) {
		analogWrite(enA, i);
		analogWrite(enB, i);
		delay(20);
	}
	
	// Now turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

