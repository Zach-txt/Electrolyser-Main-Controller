#include <Arduino.h>


// put function declarations here:
int myFunction(int, int);
bool manometerVerifications(float current);
bool flowmeterVerifications(float current);
void electrolyserProtection(bool sensor1, bool sensor2);
void displaySensorValues(float voltage, float current);
void setRelayState(bool currentState);
bool getRelayState(void);

//Constantes et variables globales
const int manoPin = A0;
const int flowmeterPin = A1;
const int relayCommandPin = 23; //Digital Pin de commande 

bool manoVerif = false;
bool flowVerif = false;
bool relayState = false;
void setup() {
  //Relay Set up
  pinMode(relayCommandPin, OUTPUT);
  setRelayState(false);
  Serial.begin(9600);
}

void loop() {
  // Lecture du manomèetre
  int sensor_Mano_Value = analogRead(manoPin);
  float manoVoltage = (sensor_Mano_Value*5.0)/1023.0;
  float manoCurrent = manoVoltage/250.0;

  //Lecture de Flowmeter
  int sensor_Flowmeter_Value = analogRead(flowmeterPin);
  float flowmeterVoltage = (sensor_Flowmeter_Value*5.0)/1023.0;
  float flowmeterCurrent = flowmeterVoltage/250.0;

  //Relay manipulations
  manoVerif = manometerVerifications(manoCurrent); //Look at Manometer sensor state
  flowVerif = flowmeterVerifications(flowmeterCurrent);

  electrolyserProtection(manoVerif,flowVerif);
 
  Serial.print("Lecture de manometre : ");
  displaySensorValues(manoVoltage,manoCurrent);

  Serial.print("Lecture de flowmeter : ");
  displaySensorValues(flowmeterVoltage,flowmeterCurrent);


  delay(5000);
}

// put function definitions here:
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
    digitalWrite(relayCommandPin, HIGH);
  }
  else
  {
    //Close the relay
    digitalWrite(relayCommandPin, LOW);
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
 if((manoVerif == true || flowVerif == true) && relayState == false)
    {
      setRelayState(true);
    }
  if((manoVerif == false && flowVerif == false) && relayState == true)
    {
      setRelayState(false);
    }   
}
