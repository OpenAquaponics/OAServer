/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#ifndef _OASENSOR_H_
#define _OASENSOR_H_

/* system includes */
#include <iostream>

/* user includes */


/* defines */


/* enums */
typedef enum {
  OASENSOR_AI = 1,
  OASENSOR_AO,
  OASENSOR_DI,
  OASENSOR_DO,
  OASENSOR_SER,
  OASENSOR_TCP,
  OASENSOR_UDP,
} SESNOR_TYPE_e;

typedef enum {
  OATASK_ON = 1,
  OATASK_OFF,
  OATASK_SMS,
  OATASK_TWITTER,
  OATASK_EMAIL,
} TASK_TYPE_e;

/* typedef */


/* namespace */

/* class */



class OASensor {

  public:
    OASensor(void) {};
   ~OASensor(void) {};
    OASensor(std::string sName, unsigned int mType, std::string sUnits, std::string sDescription) {
      SetName(sName);
      SetType(mType);
      SetUnits(sUnits);
      SetDescription(sDescription);
    };

    virtual int ProcessSensor(void) = 0;

    void SetName(std::string s) { this->sName = s; };
    void SetType(unsigned int t) { this->mType = t; };
    void SetDescription(std::string s) { this->sDescription = s; };
    void SetUnits(std::string u) { this->sUnits = u; };

    std::string GetName(void) { return(sName); };
    unsigned int GetType(void) { return(mType); };
    std::string GetDescription(void) { return(sDescription); };
    std::string GetUnits(void) { return(sUnits); };

  private:
    std::string sName;
    unsigned int mType;
    std::string sDescription;
    std::string sUnits;
};


class SensorAI : public OASensor {

  public:
    SensorAI(std::string sName, unsigned int mType, unsigned int mPin, std::string sUnits, std::string sDescription);
   ~SensorAI(void);

    virtual int ProcessSensor(void) { std::cout << OASensor::GetDescription() << std::endl; };

    void SetPin(unsigned int p) { this->mPin = p; };

    unsigned int GetPin(void) { return(mPin); };

  private:
    unsigned int mPin;
};


class SensorDO : public OASensor {

  public:
    SensorDO(std::string sName, unsigned int mType, unsigned int mPin, std::string sUnits, std::string sDescription);
   ~SensorDO(void);

    virtual int ProcessSensor(void) { std::cout << OASensor::GetDescription() << std::endl; };

    void SetPin(unsigned int p) { this->mPin = p; };

    unsigned int GetPin(void) { return(mPin); };

  private:
    unsigned int mPin;
};



#endif /* _OASENSOR_H_ */

