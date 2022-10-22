#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

#define SPEC_ST          PA1
#define SPEC_CLK         PA2
#define SPEC_VIDEO       PA3


#define SPEC_CHANNELS    288 // New Spec Channel

int32_t data[SPEC_CHANNELS];
int32_t newdata[SPEC_CHANNELS];
int16_t adc1;
int cnt=0;

void setup(){
  //adc.setGain(GAIN_ONE);t
  //Set desired pins to OUTPUT
  pinMode(SPEC_CLK, OUTPUT);
  pinMode(SPEC_ST, OUTPUT);

  digitalWrite(SPEC_CLK, HIGH); // Set SPEC_CLK High
  digitalWrite(SPEC_ST, LOW); // Set SPEC_ST Low

  Serial.begin(115200); // Baud Rate set to 115200
  
    if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
}

void printData(){
  for (int i = 0; i < SPEC_CHANNELS; i++){
    
    Serial.print(newdata[i]/3);
    Serial.print(',');
    
  }
  
  Serial.print("\n");
  memset (newdata, 0, sizeof(newdata));
}

void avg(){
  for (int i = 0; i < SPEC_CHANNELS; i++){
    newdata[i] = data[i] + newdata[i];
    //Serial.print(data[i]);
    //Serial.print(',');
  }
  
}
/*
 * This functions reads spectrometer data from SPEC_VIDEO
 * Look at the Timing Chart in the Datasheet for more info
 */
void readSpectrometer(){

  int delayTime = 1; // delay time

  
  // Start clock cycle and set start pulse to signal start
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  digitalWrite(SPEC_ST, HIGH);
  delayMicroseconds(delayTime);

  //Sample for a period of time
  for(int i = 0; i < 15; i++){

      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime); 
 
  }
  
  //Set SPEC_ST to low
  digitalWrite(SPEC_ST, LOW);

  //Sample for a period of time
  for(int i = 0; i < 85; i++){

      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime); 
      
  }
  
  //One more clock pulse before the actual read
  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);

  //Read from SPEC_VIDEO
  
  for(int i = 0; i < SPEC_CHANNELS; i++){
      //data[i] = analogRead(SPEC_VIDEO);
      data[i] = ads.readADC_SingleEnded(0);
      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime);
        
  }
  cnt+=1;
  //Serial.println(cnt);
  if (cnt == 3) { 
    printData(); 
    cnt = cnt - 3;
    }
  
  avg();
  
  //Set SPEC_ST to high
  digitalWrite(SPEC_ST, HIGH);

  //Sample for a small amount of time
  for(int i = 0; i < 7; i++){
      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime);

  }

  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);

}

/*
 * The function below prints out data to the terminal or 
 * processing plot
 */

void loop(){
  readSpectrometer();
  //Serial.print("\n");
  //adc1 = ads.readADC_SingleEnded(1);
  //Serial.println(adc1); 
  delay(45); 

}
