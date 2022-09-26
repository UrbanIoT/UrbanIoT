#pragma once

namespace MobileEnvironmentalMonitoring::Utils {
    class Mutex{
        public:
        virtual void take(void) = 0;
        virtual void give(void) = 0;
    };
}