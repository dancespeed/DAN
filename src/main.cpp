#include <Arduino.h>

#include "system/system.hpp"

void setup()
{
    System::Init();
}

void loop()
{
    System::Process();
}