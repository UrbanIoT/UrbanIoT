#ifndef SEEED_PM2_5_SENSOR_HM3301_HM330X_ERROR_CODE_H
#define SEEED_PM2_5_SENSOR_HM3301_HM330X_ERROR_CODE_H


typedef enum {
    NO_ERROR = 0,
    ERROR_PARAM = -1,
    ERROR_COMM = -2,
    ERROR_OTHERS = -128,
} HM330XErrorCode;


#endif //SEEED_PM2_5_SENSOR_HM3301_HM330X_ERROR_CODE_H