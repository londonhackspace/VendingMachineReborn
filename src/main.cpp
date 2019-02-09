/*
 _     _   _ ____   __     _______ _   _ ____ ___ _   _  ____   __  __    _    ____ _   _ ___ _   _ _____
| |   | | | / ___|  \ \   / / ____| \ | |  _ \_ _| \ | |/ ___| |  \/  |  / \  / ___| | | |_ _| \ | | ____|
| |   | |_| \___ \   \ \ / /|  _| |  \| | | | | ||  \| | |  _  | |\/| | / _ \| |   | |_| || ||  \| |  _|
| |___|  _  |___) |   \ V / | |___| |\  | |_| | || |\  | |_| | | |  | |/ ___ \ |___|  _  || || |\  | |___
|_____|_| |_|____/     \_/  |_____|_| \_|____/___|_| \_|\____| |_|  |_/_/   \_\____|_| |_|___|_| \_|_____|
Written to stop tgreer from throwing the vending machine away
*/

#include <Arduino.h>

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
