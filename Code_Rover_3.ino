#include <Wire.h>

#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.

#define sensor0 A3 //gauche 
#define sensor1 A2 //droite
#define sensor2 A4 //avant
#include <Servo.h>

Servo Lidar;
Servo MoteurAvance;
Servo MoteurTourne;

double pos = 0;         // Position of the servo (degress, [0, 180])
double distance = 0;    // Distance measured
int distanceIR0=0;
int distanceIR1=0;
int distanceIR2=0;
int distanceParcouru=0;


const int MoteurAvancePin = 8;  // avance
const int MoteurTournePin = 9; // tourne


void setup()
{
  Serial.begin(9600);
  Serial.println("< START >");
  Lidar.attach(7); //Servo du LIDAR
  MoteurAvance.attach(MoteurAvancePin,1000,2000); //Servo avancer
  MoteurTourne.attach(MoteurTournePin,1000,2000); //Servo tourner
  // LIDAR control
  Wire.begin(); 
}


int lidarGetRange(void)
{
  int val = -1;
  
  Wire.beginTransmission((int)LIDARLite_ADDRESS); 
  Wire.write((int)RegisterMeasure); 
  Wire.write((int)MeasureValue);  
  Wire.endTransmission();

  delay(20); 

  Wire.beginTransmission((int)LIDARLite_ADDRESS);
  Wire.write((int)RegisterHighLowB);
  Wire.endTransmission(); 

  delay(20); 
  
  Wire.requestFrom((int)LIDARLite_ADDRESS, 2); 

  if(2 <= Wire.available()) 
  {
    val = Wire.read(); 
    val = val << 8; 
    val |= Wire.read(); 
  }
  
  return val;
}

int iRGetRange(void)
{
  float volts0=analogRead(sensor0)*0.0048828125;
  distanceIR0=13*pow(volts0,-1)*2.54;
  float volts1=analogRead(sensor1)*0.0048828125;
  distanceIR1=13*pow(volts1,-1)*2.54;
  float volts2=analogRead(sensor2)*0.0048828125;
  distanceIR2=13*pow(volts2,-1)*2.54;

  //Serial.println(distanceIR0);
  //Serial.print("gauche \n");
  //Serial.println(distanceIR1);
  //Serial.print("droite \n");
  //Serial.println(distanceIR2);
  //Serial.print("arriere");
}

void serialPrintRange(int pos, int distance)
{
    //Serial.print("Position (deg): ");
    //Serial.print(pos);
    //Serial.print("\t\tDistance (cm): ");
    //Serial.println(distance);
}

void LIDAR(void)
{
   
  for(pos = 40; pos <= 140; pos += 5)
  {
    iRGetRange();
    avance();
    Lidar.write(pos);
    distance = lidarGetRange();
    serialPrintRange(pos, distance);
  }

  for(pos = 140; pos>=40; pos -= 5)
  {
    iRGetRange();
    avance();
    Lidar.write(pos);
    distance = lidarGetRange();
    serialPrintRange(pos, distance);
  }
}
void avance(void)
{ 
   if(distanceIR1<20 && distanceIR1>1 ) 
   {
    MoteurAvance.write(90);
    delay(50); 
    MoteurAvance.write(40);
    delay(500);
    MoteurAvance.write(90);
    delay(50);
   }
   else
   {
    if(distance > (-0.0112*(pos)*(pos) + 2.0162*(pos) - 39.25))
    {
     //MoteurAvance.write(148.73); vitesse theorique
     MoteurAvance.write(110); //vitesse test
    }   
    else //if(pos>=70 && pos<=110)
    {
     MoteurAvance.write(90);
     delay(1); 
      if(pos<=90)
      {
       MoteurTourne.write(140);
       delay(8*pos-220);
       MoteurTourne.write(90);
       delay(1);
      } 
      else
      {
       MoteurTourne.write(40);
       delay(-8*pos+1220);
       MoteurTourne.write(90);
       delay(1);
       } 
     }
    }
}

void loop()
{
  LIDAR();
}
