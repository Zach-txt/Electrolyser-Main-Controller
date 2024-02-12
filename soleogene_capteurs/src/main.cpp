#include <Arduino.h>


// put function declarations here:
bool manometerVerifications(float current);
bool flowmeterVerifications(float current);
void electrolyserProtection(bool sensor1, bool sensor2);
void displaySensorValues(float voltage, float current);
void setRelayState(bool currentState);
bool getRelayState(void);
float calculateAnalog2DigitalVoltage(int analogPin,int numberOfBits);
float calculateSensorCurrentOuput(float voltageOutput, float resistance);

//Constantes et variables globales
const int MANOMETERPIN = A0;
const int FLOWMETERPIN = A1;
const int RELAYCOMMANDPIN = 23; //Digital Pin de commande
const float MANOMETERRESISTANCE = 250.0; 
const float FLOWMETERRESISTANCE = 250.0; 

bool relayState = false;

void setup() {
  //Relay Set up
  pinMode(RELAYCOMMANDPIN, OUTPUT);
  setRelayState(false);
  Serial.begin(9600);
}

void loop() {
  //Variables volatiles
  bool manoVerif = false;
  bool flowVerif = false;

  // Lecture du manomètre
  float manometerVoltage = calculateAnalog2DigitalVoltage(MANOMETERPIN,10);
  float manoCurrent = calculateSensorCurrentOuput(manometerVoltage, MANOMETERRESISTANCE);

  //Lecture de Flowmeter
  float flowmeterVoltage = calculateAnalog2DigitalVoltage(FLOWMETERPIN,10);
  float flowmeterCurrent = calculateSensorCurrentOuput(flowmeterVoltage, FLOWMETERRESISTANCE);

  //Relay manipulations
  manoVerif = manometerVerifications(manoCurrent); //Look at Manometer sensor state
  flowVerif = flowmeterVerifications(flowmeterCurrent);

  electrolyserProtection(manoVerif,flowVerif);
 
  Serial.print("Lecture de manometre : ");
  displaySensorValues(manometerVoltage,manoCurrent);

  Serial.print("Lecture de flowmeter : ");
  displaySensorValues(flowmeterVoltage,flowmeterCurrent);


  delay(5000);
}

// put function definitions here:
float calculateAnalog2DigitalVoltage(int analogPin,int numberofBits)
{
  float total = 2^numberofBits;
  float analogValue = analogRead(analogPin);

  return ((analogValue*5.0)/total);
}

float calculateSensorCurrentOuput(float voltageOutput, float resistance)
{
  return (voltageOutput/resistance);
}
bool manometerVerifications(float voltage) {
  //1V = 0PcurrentSI
  //5V = 250PSI
float pressure = (voltage-1)*250.0/4;
float buffer = 5.0;//PSI

//Valider la mesure de pression et faire l'opération voulue
  if(voltage <= 1 || pressure > (250.0 + buffer))
  {
    return true;
  }
  else{
    return false;
  }
}
bool flowmeterVerifications(float current)
{
  //Put flowmeter verifications when clarified
  return false;
}

void setRelayState(bool state)
{
  relayState = state;
  if(relayState == true)
  {
    //Opens the realy
    digitalWrite(RELAYCOMMANDPIN, HIGH);
  }
  else
  {
    //Close the relay
    digitalWrite(RELAYCOMMANDPIN, LOW);
  }
}
bool getRelayState(void)
{
  return relayState;
}
void displaySensorValues(float voltage, float current)
{
  Serial.print("Voltage (V) : ");
  Serial.print(voltage);
  Serial.print(" ");

  Serial.print("Current (mA) : ");
  Serial.print(current);
  Serial.print("\n");
}

void electrolyserProtection(bool sensor1, bool sensor2)
{
  //Minimise le nombre d'ouverture/fermreture du relai.
 //if((manoVerif == true || flowVerif == true) && relayState == false)
 //   {
 //     setRelayState(true);
 //   }
 // if((manoVerif == false && flowVerif == false) && relayState == true)
 //   {
 //     setRelayState(false);
 //   }  
}