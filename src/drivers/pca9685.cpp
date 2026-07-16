#include "pca9685.hpp"

namespace PCA9685
{
    bool Init()
    {
        // Реализацию добавим после проверки I2C.
        return true;
    }

    bool Set(uint8_t channel,
             uint16_t brightness)
    {
        if (channel >= 16)
        {
            return false;
        }

        if (brightness > 4095)
        {
            brightness = 4095;
        }

        // Реализацию записи в PCA9685 добавим позже.
        return true;
    }

    bool SetAll(uint16_t brightness)
    {
        if (brightness > 4095)
        {
            brightness = 4095;
        }

        // Реализацию записи во все каналы добавим позже.
        return true;
    }
}