/*
 * LCD RS pin 8
 * LCD En pin 6
 * LCD D4 pin 5
 * LCD D5 pin 4
 * LCD D6 pin 3
 * LCD D7 pin 2
 * LCD R/W gnd
 *
 * Ingresso sensore pin 7
 * Ingresso reset pin 9
 */

#include <LiquidCrystal.h>

#define SENS_IN 7
#define RESET 9
#define TIMEOUT 20

LiquidCrystal lcd(8, 6, 5, 4, 3, 2);

volatile uint32_t counter = 0;

void count()
{
    // debouncing delay
    delayMicroseconds(16383);
    if (!digitalRead(SENS_IN))
    {
        counter++;
    }
}

void setup()
{

    lcd.begin(8, 2);
    lcd.print("persone:");
    lcd.setCursor(0, 1);
    lcd.print(counter);

    pinMode(SENS_IN, INPUT_PULLUP);
    pinMode(RESET, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(SENS_IN), count, FALLING);

    Serial1.begin(9600);
}

void loop()
{

    if (Serial1.available() == 2)
    {
        uint8_t first  = Serial1.read();
        uint8_t second = Serial1.read();

        if (first == 0x55 && second == 0xDD)
        {
            // HACK: cast the pointer to an uint32_t to a pointer to
            // an uint8_t and thread this as an array. Since receiver
            // and transmitter have the same endianness this is an easy
            // way to send a 32 bit value over a serial line
            uint8_t *ptr = (uint8_t *)(&counter);
            for (uint8_t i = 0; i < 4; i++)
            {
                Serial1.write(ptr[i]);
                delay(50);
            }
        }
        Serial1.flush();
    }

    // This to ensure reset has effect only once,
    // otherwise problems with the serial communication
    // may be observed
    if (!digitalRead(RESET) && counter != 0)
    {
        // 65535us of debouncing delay
        delayMicroseconds(0xFFFF);
        if (!digitalRead(RESET))
        {
            counter = 0;
            Serial1.flush();
        }

        lcd.clear();
        lcd.print("persone:");
    }

    if (counter > 99999999)
    {
        counter = 0;
    }

    lcd.setCursor(0, 1);
    lcd.print(counter);
}
