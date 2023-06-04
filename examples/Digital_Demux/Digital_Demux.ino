// CD74HC4067 Demultiplexing
// https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf

const uint8_t controlPins[] = { 0, 1, 2, 3 };

void setup()
{
  for (int i = 0; i < 4; ++i)
  {
    pinMode(controlPins[i], OUTPUT);
  }
}

void loop()
{
  for(int i = 0; i < 10; i ++)
  {
    digitalWrite(controlPins[0], i & 1);
    digitalWrite(controlPins[1], 1 & i >> 1);
    digitalWrite(controlPins[2], 1 & i >> 2);
    digitalWrite(controlPins[3], 1 & i >> 3);
    delay(500);
  }
}