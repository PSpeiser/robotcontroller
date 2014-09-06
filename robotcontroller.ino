int FLA = 8;
int FLB = 9;
int FLE = 10;

int FRA = 6;
int FRB = 5;
int FRE = 4;

int BLA = 16;
int BLB = 17;
int BLE = 15;

int BRA = 14;
int BRB = 13;
int BRE = 12;


const int MINDUTYCYCLE = 100;
const int MAXDUTYCYCLE = 255;

const int FL = 0;
const int FR = 1;
const int BL = 2;
const int BR = 3;

unsigned long lastCommandTime = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello");
  pinMode(FLA,OUTPUT);
  pinMode(FLB,OUTPUT);
  pinMode(FLE,OUTPUT);
  pinMode(FRA,OUTPUT);
  pinMode(FRB,OUTPUT);
  pinMode(FRE,OUTPUT);
  pinMode(BLA,OUTPUT);
  pinMode(BLB,OUTPUT);
  pinMode(BLE,OUTPUT);
  pinMode(BRA,OUTPUT);
  pinMode(BRB,OUTPUT);
  pinMode(BRE,OUTPUT);
}
int motorSelect = -1;
int motorPower = -1;
void loop()
{
  if (millis() - lastCommandTime > 1000)
  {
    StopMotors();
  }
  
  if(Serial.available() > 0)
  {
    if(motorSelect != -1)
    {
      motorPower = Serial.read();
      motorPower -= 128;
      float mp = motorPower / 128.0;
      switch (motorSelect)
      {  
        case FL:
            FLSpin(mp);
            break;
        case FR:
            FRSpin(mp);
            break;
        case BL:
            BLSpin(mp);
            break;
        case BR:
            BRSpin(mp);
            break;
      }
      motorSelect = -1;
      lastCommandTime = millis();
    }
    else
    {
      motorSelect = Serial.read();
    }
    
  }
}
int getDutyCycle(float power)
{
  int dutyCycle = 0;
  if(power > 0.0)
  {
    dutyCycle = min(MINDUTYCYCLE + (MAXDUTYCYCLE-MINDUTYCYCLE)*power,MAXDUTYCYCLE);    
  }
  return dutyCycle;
}
void StopMotors()
{
  FLSpin(0.0);
  FRSpin(0.0);
  BLSpin(0.0);
  BRSpin(0.0);
}
void FLSpin(float power)
{
  int dutyCycle = 0;
  //spin counter-clockwise on positive values
  if(power >= 0)
  {
    digitalWrite(FLA,LOW);
    digitalWrite(FLB,HIGH);
    dutyCycle = getDutyCycle(power);
  }
  //spin clockwise on negative values
  else
  {
    digitalWrite(FLA,HIGH);
    digitalWrite(FLB,LOW);
    dutyCycle = getDutyCycle(power*-1);
  }
  analogWrite(FLE,dutyCycle);
}
void FRSpin(float power)
{
  int dutyCycle = 0;
  //spin clockwise on positive values
  if(power >= 0)
  {
    digitalWrite(FRA,HIGH);
    digitalWrite(FRB,LOW);
    dutyCycle = getDutyCycle(power);
  }
  //spin counter-clockwise on negative values
  else
  {
    digitalWrite(FRA,LOW);
    digitalWrite(FRB,HIGH);
    dutyCycle = getDutyCycle(power*-1);
  }
  analogWrite(FRE,dutyCycle);
}
void BLSpin(float power)
{
  int dutyCycle = 0;
  //spin counter-clockwise on positive values
  if(power >= 0)
  {
    digitalWrite(BLA,LOW);
    digitalWrite(BLB,HIGH);
    dutyCycle = getDutyCycle(power);
  }
  //spin clockwise on negative values
  else
  {
    digitalWrite(BLA,HIGH);
    digitalWrite(BLB,LOW);
    dutyCycle = getDutyCycle(power*-1);
  }
  analogWrite(BLE,dutyCycle);
}
void BRSpin(float power)
{
  int dutyCycle = 0;
  //spin clockwise on positive values
  if(power >= 0)
  {
    digitalWrite(BRA,HIGH);
    digitalWrite(BRB,LOW);
    dutyCycle = getDutyCycle(power);
  }
  //spin counter-clockwise on negative values
  else
  {
    digitalWrite(BRA,LOW);
    digitalWrite(BRB,HIGH);
    dutyCycle = getDutyCycle(power*-1);
  }
  analogWrite(BRE,dutyCycle);
}  
