#include <Arduino.h>


// put function declarations here:
bool manometerPressureIsOK(float current);
bool flowmeterFlowIsOK(float current);
void electrolyserProtectionActions(bool sensor1, bool sensor2);
void displayVoltageCurrentSensorValues(float voltage, float current);
void displaySensorValues(float pressure, float flow);
void setRelayState(bool currentState);
bool getRelayState(void);
float calculateAnalog2DigitalVoltage(int analogPin,int numberOfBits);
float calculateSensorCurrentOuput(float voltageOutput, float resistance);

//Constantes
//ANALOG PINS
const int MANOMETERPIN = A0;
const int FLOWMETERPIN = A1;

//DIGITAL PINS
const int RELAYCOMMANDPIN = 23; 

//Valeurs de mesure
const float MANOMETERRESISTANCE = 250.0; 
const float FLOWMETERRESISTANCE = 250.0; 

//Variables globales
bool RELAYSTATEFLAG= false;

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
  manoVerif = manometerPressureIsOK(manoCurrent); //Look at Manometer sensor state
  flowVerif = flowmeterFlowIsOK(flowmeterCurrent);

  electrolyserProtectionActions(manoVerif,flowVerif);
  
  Serial.print("Lecture de manometre : ");
  displayVoltageCurrentSensorValues(manometerVoltage,manoCurrent);

  Serial.print("Lecture de flowmeter : ");
  displayVoltageCurrentSensorValues(flowmeterVoltage,flowmeterCurrent);


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

bool manometerPressureIsOK(float voltage) {
  //1V = 0 PSI
  //5V = 250 PSI
float pressurePSI = (voltage-1)*250.0/4; 
float bufferPressure = 5.0;

//Valider la mesure de pression et faire l'opération voulue
  if(voltage <= 1 || pressurePSI > (200.0 + bufferPressure))
  {
    return true;
  }
  else{
    return false;
  }
}

bool flowmeterFlowIsOK(float current)
{
  //Put flowmeter verifications when clarified
  return false;
}

void setRelayState(bool state)
{
  RELAYSTATEFLAG= state;
  if(RELAYSTATEFLAG== true)
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
  return RELAYSTATEFLAG;
}
void displayVoltageCurrentSensorValues(float voltage, float current)
{
  Serial.print("Voltage (V) : ");
  Serial.print(voltage);
  Serial.print(" ");

  Serial.print("Current (mA) : ");
  Serial.print(current);
  Serial.print("\n");
}
void displaySensorValues(float pressure, float flow)
{
    Serial.print("Pressure (PSI) : ");
  Serial.print(pressure);
  Serial.print(" ");

  Serial.print("Current (mA) : ");
  Serial.print(flow);
  Serial.print("\n");
}

void electrolyserProtectionActions(bool sensor1, bool sensor2)
{
  //Minimise le nombre d'ouverture/fermreture du relai.
 if((sensor1 == true || sensor2 == true) && RELAYSTATEFLAG== false)
    {
      setRelayState(true);
    }
  if((sensor1 == false && sensor2 == false) && RELAYSTATEFLAG== true)
    {
      setRelayState(false);
    }  
}