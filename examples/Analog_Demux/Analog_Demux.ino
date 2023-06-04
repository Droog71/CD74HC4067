// CD74HC4067 Demultiplexing
// https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf

const uint8_t adcPins [] = { 33, 32, 35, 34 };
const uint8_t controlPins[] = { 18, 19, 22, 23 };

void setup()
{
  Serial.begin(115200);
  for (int i = 0; i < 4; ++i)
  {
    pinMode(controlPins[i], OUTPUT);
  }
}

void loop()
{
  for (int i = 0; i < 4; ++i)
  {
    digitalWrite(controlPins[0], i & 1);
    digitalWrite(controlPins[1], 1 & i >> 1);
    digitalWrite(controlPins[2], 1 & i >> 2);
    digitalWrite(controlPins[3], 1 & i >> 3);
    delay(500);

    for(int j = 0; j < 3; j++)
    {
      Serial.print('I');
      Serial.print(j);
      Serial.print(": ");
      Serial.print(analogRead(adcPins[j]));
      Serial.print(", ");
    }

    Serial.print("I3: ");
    Serial.print(analogRead(adcPins[3]));
    Serial.println();
    delay(500);
  }
}