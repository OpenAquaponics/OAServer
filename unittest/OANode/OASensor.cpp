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
/* SensorDO */
/****************************************/

/****************************************/
SensorDO::SensorDO(std::string sName, unsigned int mType, unsigned int mPin, std::string sUnits, std::string sDescription) :
  OASensor(sName, mType, sUnits, sDescription) {
/****************************************/
  SetPin(mPin);
}



