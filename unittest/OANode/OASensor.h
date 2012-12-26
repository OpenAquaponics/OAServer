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


/* typedef */

/* namespace */

/* class */



class SensorAI {

  public:
    SensorAI(void);
   ~SensorAI(void);

};

class OASensor {

  public:
    OASensor(void);
   ~OASensor(void);
    OASensor(int mPin, std::string sUnits, std::string sDescription) {
      SetPin(mPin);
      SetUnits(sUnits);
      SetDescription(sDescription);
    };

  protected:
    void SetPin(int p) { this->mPin = p; };
    void SetDescription(std::string s) { this->sDescription = s; };
    void SetUnits(std::string u) { this->sUnits = u; };

    int GetPin(void) { return(mPin); };
    std::string GetDescription(void) { return(sDescription); };
    std::string GetUnits(void) { return(sUnits); };

  private:
    int mPin;
    std::string sDescription;
    std::string sUnits;
};

#endif /* _OASENSOR_H_ */

