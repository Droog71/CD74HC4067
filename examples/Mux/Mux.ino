// CD74HC4067
// 16-Channel Analog Multiplexer
// Demultiplexing not implemented for this simulation.
// https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf

#include <Adafruit_NeoPixel.h>

#define COM 34
#define NP_PIN 4
#define NUMPIXELS 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NP_PIN, NEO_GRB + NEO_KHZ800);

const uint8_t controlPins[] = { 25, 26, 19, 18 };

const uint8_t muxChannels [16][4] = {
  {0,0,0,0},
  {1,0,0,0},
  {0,1,0,0},
  {1,1,0,0},
  {0,0,1,0},
  {1,0,1,0},
  {0,1,1,0},
  {1,1,1,0},
  {0,0,0,1},
  {1,0,0,1},
  {0,1,0,1},
  {1,1,0,1},
  {0,0,1,1},
  {1,0,1,1},
  {0,1,1,1},
  {1,1,1,1}
};

float readMux(int channel)
{
  for(int i = 0; i < 4; i ++)
  {
    digitalWrite(controlPins[i], muxChannels[channel][i]);
  }
  delay(10);
  return analogRead(COM);
}

void setup()
{
  for (int i = 0; i < 4; ++i)
  {
    pinMode(controlPins[i], OUTPUT);
  }
  pixels.begin();
}

void loop()
{
  pixels.clear();
  for(int i = 0; i < 16; ++i)
  {
    if (readMux(i) > 1500)
    {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    }
  }
  pixels.show();
}