/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/

/* user includes */
#include "OASensor.h"

using namespace std;

/****************************************/
/* SensorAI */
/****************************************/

/****************************************/
SensorAI::SensorAI(std::string sName, unsigned int mType, unsigned int mPin, std::string sUnits, std::string sDescription) :
  OASensor(sName, mType, sUnits, sDescription) {
/****************************************/
  SetPin(mPin);
}


/****************************************/
/* TODO - Really need to think about how to setup the Process fuction
 * To support to different sensor types, like DO, AI, TCP, etc */
float SensorAI::ProcessSensor(void) {
/****************************************/
  switch(GetPin()) {
    case 10:
      return(70.0 + (rand() % 50) / 10.0);
      break;
    case 13:
      return(85.0 + (rand() % 50) / 10.0 );
      break;
    case 17:
      return(150.0 + (rand() % 200) / 10.0 );
      break;
  }
}


/****************************************/
/* SensorDO */
/****************************************/

/****************************************/
SensorDO::SensorDO(std::string sName, unsigned int mType, unsigned int mPin, std::string sUnits, std::string sDescription) :
  OASensor(sName, mType, sUnits, sDescription) {
/****************************************/
  SetPin(mPin);
}



