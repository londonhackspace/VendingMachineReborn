/*
 _     _   _ ____   __     _______ _   _ ____ ___ _   _  ____   __  __    _    ____ _   _ ___ _   _ _____
| |   | | | / ___|  \ \   / / ____| \ | |  _ \_ _| \ | |/ ___| |  \/  |  / \  / ___| | | |_ _| \ | | ____|
| |   | |_| \___ \   \ \ / /|  _| |  \| | | | | ||  \| | |  _  | |\/| | / _ \| |   | |_| || ||  \| |  _|
| |___|  _  |___) |   \ V / | |___| |\  | |_| | || |\  | |_| | | |  | |/ ___ \ |___|  _  || || |\  | |___
|_____|_| |_|____/     \_/  |_____|_| \_|____/___|_| \_|\____| |_|  |_/_/   \_\____|_| |_|___|_| \_|_____|
Written to stop tgreer from throwing the vending machine away
*/

#include "VendingMachineConfig.h"

//if we are cross compiling, pull in Arduino.h
#if CMAKE_CROSSCOMPILING
#include <Arduino.h>
#else
// dummy main for desktop use
int main(int argc, char** argv)
{
    return 0;
}
#endif

void setup()
{
  
}

void loop()
{
                        // wait for a second
}
