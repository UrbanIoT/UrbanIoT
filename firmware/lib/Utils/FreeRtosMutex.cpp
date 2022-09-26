#ifdef SEEED_WIO_TERMINAL
#include "FreeRtosMutex.h"

void MobileEnvironmentalMonitoring::Utils::FreeRtosMutex::take(void)
{
    xSemaphoreTake(this->_mutex, portMAX_DELAY);
}

void MobileEnvironmentalMonitoring::Utils::FreeRtosMutex::give(void)
{
    xSemaphoreGive(this->_mutex);
}

#endif // SEEED_WIO_TERMINAL