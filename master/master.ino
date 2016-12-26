
const int numReadings = 150;

long readings[numReadings];      // the readings from the analog input
long readIndex = 0;              // the index of the current reading
long total = 0;                  // the running total
long average = 0;                // the average
boolean music_playing = 0;
int inputPin = A0;
int threshold = 250;

void setup() 

{
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  delay(5000);
}

void loop() 

{
  get_audio_reading();
  
  if(average > threshold)
  {
    digitalWrite(13,HIGH);
    
    if(music_playing == 0)
    {
      turn_on_power();
      music_playing = 1;
    }
  }
  if(average < threshold)
  {
    digitalWrite(13,LOW);
    
    if(music_playing == 1)
    {
      music_playing = 0;
    }
  }
  delay(10);        // delay in between reads for stability
}

void turn_on_power()
{
   //Input gets sent first, if unit is already on
   Serial.print("Z1INP3;");
   delay(10);
   Serial.write( '\r' ); 
   //Multiple power on commands are sent just to make sure they get there
   Serial.print("Z1POW1;");
   delay(10);
   Serial.print("Z1POW1;");
   Serial.write( '\r' ); 
   delay(100);
   Serial.print("Z1POW1;");
   delay(10);
   Serial.print("Z1POW1;");
   Serial.write( '\r' ); 
   delay(7000);
   //A delay of 7 seconds is added before the input command, if the unit was off
   Serial.print("Z1INP3;");
   delay(10);
   Serial.write( '\r' ); 
}

void get_audio_reading()
{
  
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;
}
