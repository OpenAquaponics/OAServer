/*****************************************************************************/
/*
**  pkt_types.h - Header file containing the definitions of the ethernet packets
**
**  Author: Joshua Eliser
**
**
*/
/*****************************************************************************/
#ifndef _PKT_TYPES_H_
#define _PKT_TYPES_H_


/* system includes */

/* user includes */
#include "common_types.h"

/* defines */

/* enums */
typedef enum {
  ADD_MEASUREMENT = 1,
  ADD_WATER,
  ADD_FISH,
  ADD_PLANT
} PACKET_TYPE_e;


/* typedefs */
typedef struct {
  int mSync;        /* 0xCABE */
  int mNumBytes;    /* Number of bytes in the payload */
  int mTimeTagSec;  /* NTP(?) time in secs */
  int mDeviceId;
  int mMsgType;
  int mCheckSum;    /* Checksum of just payload */
} PacketHeader_t;

typedef struct {
  PacketHeader_t mHdr;
  float mTempAirIndoor;
  float mTempAirOutdoor;
  float mTempWater;
  float mHumidityIndoor;
  float mHumidityOutdoor;
  float mWaterLevel;
  float mBattaryVoltage;
  float mSolarPanelVoltage;
} PktStatistics_t;

typedef struct {
  uint32_t  year;
  uint32_t  month;
  uint32_t  day;
} rpi_pkt_t;

typedef struct {
  rpi_pkt_t rpiHdr;
  uint32_t  measType;
  float     measData;
  char      comment[256];
} meas_data_t;

typedef struct {
  rpi_pkt_t rpiHdr;
  float     waterData;
  char      comment[256];
} water_data_t;

typedef struct {
  rpi_pkt_t rpiHdr;
  uint32_t  fishType;
  uint32_t  fishSize;
  char      comment[256];
} fish_data_t;

typedef struct {
  rpi_pkt_t rpiHdr;
  uint32_t  plantType;
  uint32_t  plantSize;
  char      comment[256];
} plant_data_t;


/* const string */
typedef struct {
  uint32_t idx;
  char abbr[16];
  char suffix[16];
  char comment[64];
} elem_data_t;


const elem_data_t meas_type[] = {
  {0, "PH", "pH", "Water pH"},
  {1, "PH+", "pH", "Water pH+"},
  {2, "NH3", "ppm", "Ammonia (NH3)"},
  {3, "NO2-", "ppm", "Nitrite (N02-)"},
  {4, "NO3-", "ppm", "Nitrate (NO3-)"}
};
#define NUM_MEAS_TYPES   (sizeof(meas_type) / sizeof(elem_data_t))


const elem_data_t fish_type[] = {
  {0, "GOLD", "", "Goldfish"},
  {1, "TILA", "", "Tilapia"},
  {2, "CAT", "", "Catfish"},
  {3, "PERCH", "", "Perch"}
};
#define NUM_FISH_TYPES    (sizeof(fish_type) / sizeof(elem_data_t))


const elem_data_t fish_size[] = {
  {0, "VS", "", "Very Small"},
  {1, "S", "", "Small"},
  {2, "M", "", "Medium"},
  {3, "L", "", "Large"},
  {4, "VL", "", "Very Large"}
};
#define NUM_FISH_SIZES    (sizeof(fish_size) / sizeof(elem_data_t))


const elem_data_t plant_type[] = {
  {0, "BAS", "", "Basil"},
  {1, "CIL", "", "Cilantro"},
  {2, "CUC", "", "Cucumber"},
  {3, "GO", "", "Green Onion"},
  {4, "LET", "", "Lettuce"},
  {5, "PAR", "", "Parsley"},
  {6, "CAR", "", "Carrot"},
  {7, "SPN", "", "Spinach"},
  {8, "TOM", "", "Tomato"},
  {9, "DIL", "", "Dill"},
  {10, "ROS", "", "Rosemary"},
  {11, "ORA", "", "Orageno"},
  {12, "GB", "", "Green Bean"}
};
#define NUM_PLANT_TYPES   (sizeof(plant_type) / sizeof(elem_data_t))


const elem_data_t plant_size[] = {
  {0, "SEED", "", "Seed"},
  {1, "SDLG", "", "Seedling"},
  {2, "SPL", "", "Small Plant"},
  {3, "FPL", "", "Full Plant"},
  {4, "HARV", "", "Harvest"},
  {5, "DEAD", "", "Dead Plant"}
};
#define NUM_PLANT_SIZES   (sizeof(plant_size) / sizeof(elem_data_t))



#endif /* _PKT_TYPES_H_ */


