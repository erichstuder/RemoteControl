#pragma once

#include "nrf.h"
#include <stdint.h>

// Analog pins
// -----------
//#define PIN_A0 (14u)
//#define PIN_A1 (15u)
//#define PIN_A2 (16u)
//#define PIN_A3 (17u)
//#define PIN_A4 (18u)
#define PIN_A5 (19u)
#define PIN_A6 (20u)
#define PIN_A7 (21u)
//static const uint8_t A0  = PIN_A0;
//static const uint8_t A1  = PIN_A1;
//static const uint8_t A2  = PIN_A2;
//static const uint8_t A3  = PIN_A3;
//static const uint8_t A4  = PIN_A4;
static const uint8_t A5  = PIN_A5;
static const uint8_t A6  = PIN_A6;
static const uint8_t A7  = PIN_A7;
//#define ADC_RESOLUTION 12

// Digital pins
// -----------
//#define D0  (0u)
//#define D1  (1u)
//#define D2  (2u)
#define D3  (3u)
#define D4  (4u)
//#define D5  (5u)
//#define D6  (6u)
//#define D7  (7u)
//#define D8  (8u)
//#define D9  (9u)
//#define D10 (10u)
//#define D11 (11u)
//#define D12 (12u)
//#define D13 (13u)

#define LED_BUILTIN (13u)
#define LED_PWR     (25u)
#define PIN_ENABLE_I2C_PULLUP      (32u)
#define PIN_ENABLE_SENSORS_3V3     (33u)

typedef enum {
  INPUT            = 0x0,
  OUTPUT           = 0x1,
  INPUT_PULLUP     = 0x2,
  INPUT_PULLDOWN   = 0x3,
  OUTPUT_OPENDRAIN = 0x4,
} PinMode;

typedef enum {
  LOW     = 0,
  HIGH    = 1,
  CHANGE  = 2,
  FALLING = 3,
  RISING  = 4,
} PinStatus;

typedef uint8_t pin_size_t;
typedef uint8_t byte;
typedef void (*voidFuncPtr)(void);

void init();
void initVariant();

void setup();
void loop();

void pinMode(pin_size_t pin, PinMode mode);
void digitalWrite(pin_size_t pin, PinStatus val);
PinStatus digitalRead(pin_size_t pin);

void delay(unsigned long ms);
unsigned long millis();

void interrupts();
void noInterrupts();
pin_size_t digitalPinToInterrupt(pin_size_t pin);
void attachInterrupt(pin_size_t interruptNum, voidFuncPtr func, PinStatus mode);
