#pragma once

#ifdef SEEED_WIO_TERMINAL

#include "Mutex.h"
#include <FreeRTOS.h>
#include <semphr.h>

namespace MobileEnvironmentalMonitoring::Utils
{
    class FreeRtosMutex : public MobileEnvironmentalMonitoring::Utils::Mutex
    {
    public:
        void take(void);
        void give(void);

    private:
        const SemaphoreHandle_t _mutex = xSemaphoreCreateMutex();
    };
}

#endif // SEEED_WIO_TERMINAL