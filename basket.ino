// This code was designed for a project called “Basketcase” by Caitlin Woodcock, 
// who was assisted by Matt Frazer of the Critical Media Lab.
#include <LiquidCrystal.h>
LiquidCrystal lcd ( 7, 8, 9, 10, 11, 12); //Setup the LCD interface pins

int sensorReading; //A variable to store the value of the sensor reading
int sensorPin = 0; //Force-sensing resistor (FSR) is plugged into pin A0

//The sensor value above which the basket considers a phone to be present:
//Adjust between 0 (most sensitive) and 1023 (least sensitive);
int sensorThreshold = 200; 

unsigned long start; //Stores the time the phone was rested
unsigned long finish; //Stores the time the phone was lifted
unsigned long elapsed; //Stores the difference between start and finish
float percentComplete; //Stores the percentage of basket completion

int resultDisplayTime = 5000; //Time in ms to display result

//"Basket Case" is a state machine - it has different behaviours in each
//of its three states:
//1. Waiting    - Promt user to rest phone, read sensor
//2. Counting   - Count the number of seconds the phone has been resting,
//                read sensor
//3. Concluding - Show the basket percentage for a set amount of time
//This variable keeps track of what state it is in
int state = 1;


//The code that runs once:
void setup() {
  lcd.begin(16,2); //Set the LCD's number of columns and rows 
}

//The code that runs on repeat:
void loop() {

  if (state == 1) { //Do state 1 behaviour (waiting)
     
     //Prompt the user:
     lcd.setCursor(0,0);
     lcd.print("Rest Your Phone");
     lcd.setCursor(0,1);
     lcd.print("                ");

    sensorReading = analogRead(sensorPin); //Read the value of the FSR

    //Check if a phone is detected:
    if (sensorReading > sensorThreshold) {
      //Store the time (in # of ms since power on) phone was detected:
      start = millis();
      state = 2; //Move to "Counting" state
    }
     
  } else if (state == 2) { //Do state 2 behavior (counting)
    
    elapsed = millis() - start; //Subtract start time from current time
    elapsed = int(elapsed/1000); //Convert from milliseconds to seconds

    //Display count:
    lcd.setCursor(0,0);
    lcd.print("Resting   ");
    lcd.print(elapsed, 0);
    lcd.setCursor(0,1);
    lcd.print("                ");

    sensorReading = analogRead(sensorPin); //Read the value of the FSR

    //Check if the phone is removed:
    if (sensorReading <= sensorThreshold) {
      //Store the time (in # of ms since power on) phone was removed:
      finish = millis();
      elapsed = finish - start; //Find the duration phone was present
      state = 3; //Move to "Concluding" state
    }    
    
  } else if (state == 3) { //Do state 3 behavior (concluding)

   //The basket took 20h to complete
   //20h = 72,000,000 ms
   //To find the percentage:
   //%Complete = 100m* (elapsed/72,000,000).
   //Simplified:
   percentComplete = elapsed/720000;

   //Display result
   lcd.setCursor(0,0);
   lcd.print(percentComplete,2);
   lcd.print("% woven.");
   lcd.setCursor(0,1);
   lcd.print("                ");

   //Check to see if it has been longer than the preset display time:
   if (millis() - finish > resultDisplayTime) {
      state = 1;
   }

  }
  
  lcd.display(); //update the LCD display
  delay(100); //wait 100ms 
}

