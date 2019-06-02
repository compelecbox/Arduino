// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
  Name:       RGB_LED_MultiProgrammer.ino
  Created:    26/05/2019 
  Author:     CompElecBox ralph
  Multi function RGB LED controller
  This can directly drive one LED per 3 channel via resistor of 100 ohms - 2.2k ohms 1/4 watt
  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On most Arduino, the PWM pins
  are identified with a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.
  These pins can also function as Digital write pins too.
  */

// let's set some variables to be used in all the functions (Global Variables)
const int led[] = { 5, 6, 11 };   // an Array to store the ~ PWM outout pins the LED is attached to
const int numLeds = 3;   // the number of LEDs
int incTime = 2500;

// used by analogFader
int brightness[] = { 4, 7, 0 }; // start the LEDs off at 1/3 of cycle 
int fadeAmount[] = { -1, 1, 1 };
float adjustLevel[] = { 1.0, 0.6, 0.4 }; // alternative weighted values for my LEDs, try experimenting with these
int maxLevel[] = {15, 15, 15}; // comment if using values above

// set up button inputs and variables
const int programButtonPin = 2;
const int speedButtonPin = 3;
int programMode = 0;
int speedButtonState = 0;
int programButtonState = 0;
int const progMax = 5; // the maximum number of program modes

                             
// The setup() function runs once each time the micro-controller starts
void setup()
{
  // this reset will ensure that the LEDs will start off unlit, or use for calibrating
  int i = 0;
  do {
    pinMode(led[i], OUTPUT);
    // analogWrite(led[i], 255  * adjustLevel[i]); used for calibrating the white hue
    analogWrite(led[i], 0); // use this line when not calibrating
    i++;
  } while (i < numLeds);
  delay(100);// use this line when not calibrating
  // delay(5000);// use this for calibrating white hue
  
  // set up switches input pins
  pinMode(programButtonPin, INPUT);
  pinMode(speedButtonPin, INPUT);
  // Attach 2 interrupts to the ISR vectors
  attachInterrupt(digitalPinToInterrupt(programButtonPin), program_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(speedButtonPin), speed_ISR, CHANGE);
  Serial.println("reset ran!");
}

// the main program loop routine runs over and over again forever
void loop() {
  // here we will select which effect(s) to apply
  switch (programMode) {
  case 1:
    // statements
    analogFader();
    break;

  case 2:
    // statements
    justRed();
    break;

  case 3:
    // statements
    justGreen();
    break;

  case 4:
    // statements
    justBlue();
    break;

    // add new options here and change progMax to that number

  default: // case 0:
    // statements
    digitalSequencer();
    break;
  }
}

// handles the program button interrupt
void program_ISR()
{
  // get button states
  programButtonState = digitalRead(programButtonPin);
  // what to do when program button pressed?
  if (programButtonState == 1) {
    programMode += 1;
    if (programMode >= progMax) { programMode = 0; }
    delay(100); // prevent double registration
  }
  // debugging
  Serial.print("programMode=");
  Serial.println(programMode);
}

// handles the speed button interrupt
void speed_ISR()
{
  // get button states
  speedButtonState = digitalRead(speedButtonPin);
  // what to do when speed button pressed?
  if (speedButtonState == 1) {
    incTime = incTime / 2; // speed the effect up 
    if (incTime <= 100) { incTime = 5000; }// then slow it right down
    delay(100); // prevent double registration
  }
  // debug
  Serial.print("incTime=");
  Serial.println(incTime);
}

// analog fader, rather than just on/off, let's vary the brightness of each LED to get a pleasing fade effect
void analogFader()
{
  // slide the fade across the leds
  // set each LEDs brightness
  int n = 0;

  do {
    // send the values to the LEDs, brightness squared makes it appear linear to our eyes
    analogWrite(led[n], brightness[n] * brightness[n] * adjustLevel[n]);

    // incrememt or decrement the values
    brightness[n] = brightness[n] + fadeAmount[n];

    // if values max out, reverse the increment/decrement
    if (brightness[n] >= maxLevel[n]) {
      fadeAmount[n] = -fadeAmount[n];
      //brightness[n] = maxLevel[n];
    }
    else if (brightness[n] <= 0) {
      fadeAmount[n] = -fadeAmount[n];
      //brightness[n] = 0;
    }

    // move to the next LED 
    n++;
  }
  // loop around until all LEDs updated 
  while (n < numLeds);

  // debugging
  Serial.print("LED0 ");
  Serial.print(brightness[0]);
  Serial.print(" LED1 ");
  Serial.print(brightness[1]);
  Serial.print(" LED2 ");
  Serial.println(brightness[2]);

  // have a little sleep
  delay(incTime);
}

// digital sequencer
void digitalSequencer()
{
  // slide the fade across the leds by rewriting, have a delay after each step
  // set each LEDs status to LOW or HIGH in CAPITALS
  digitalWrite(led[0], HIGH);//red
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], LOW);
  delay(incTime);
  Serial.println("sequencer0");

  digitalWrite(led[0], HIGH);//yellow (red + green)
  digitalWrite(led[1], HIGH);
  digitalWrite(led[2], LOW);
  delay(incTime);
  Serial.println("sequencer1");

  digitalWrite(led[0], LOW);
  digitalWrite(led[1], HIGH);//green
  digitalWrite(led[2], LOW);
  delay(incTime);
  Serial.println("sequencer2");

  digitalWrite(led[0], LOW);
  digitalWrite(led[1], HIGH);//cyan (green + blue)
  digitalWrite(led[2], HIGH);
  delay(incTime);
  Serial.println("sequencer3");

  digitalWrite(led[0], LOW);
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], HIGH);//blue
  delay(incTime);
  Serial.println("sequencer4");

  digitalWrite(led[0], HIGH);// purple (red + blue) 
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], HIGH);
  delay(incTime);
  Serial.println("sequencer5");

  digitalWrite(led[0], HIGH);// white (red + green + blue)
  digitalWrite(led[1], HIGH);
  digitalWrite(led[2], HIGH);
  delay(incTime);
  Serial.println("sequencer6");
}

// just single LED colours
void justRed()
{
  digitalWrite(led[0], HIGH);// Red
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], LOW);
  delay(incTime);
}
void justGreen()
{
  digitalWrite(led[0], LOW);
  digitalWrite(led[1], HIGH);// Green
  digitalWrite(led[2], LOW);
  delay(incTime);
}
void justBlue()
{
  digitalWrite(led[0], LOW);
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], HIGH);// Blue
  delay(incTime);
}
