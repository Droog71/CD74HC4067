// CD74HC4067
// 16-Channel Analog Multiplexer
// https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

const char* pin_names_I [16] = { 
  "I0", "I1", "I2", "I3",
  "I4", "I5", "I6", "I7", 
  "I8", "I9", "I10", "I11",
  "I12", "I13", "I14","I15"
};

typedef struct {
  pin_t  I[16];
  pin_t  S[4];
  pin_t  VCC;
  pin_t  GND;
  pin_t  COM;
  pin_t  E;
  uint32_t analog_demux;
} chip_data_t;

bool power_connected(void *data)
{
  chip_data_t *chip = (chip_data_t*)data;
  return pin_read(chip->VCC) && !pin_read(chip->GND);
}

void chip_timer_callback(void *data) 
{
  chip_data_t *chip = (chip_data_t*)data;

  if (power_connected(chip) && !pin_read(chip->E))
  {
    bool S0 = pin_read(chip->S[0]);
    bool S1 = pin_read(chip->S[1]);
    bool S2 = pin_read(chip->S[2]);
    bool S3 = pin_read(chip->S[3]);

    int pinIndex = S3 << 3 | S2 << 2 | S1 << 1 | S0;
    chip->COM = pin_init("COM", INPUT_PULLDOWN);

    if (pin_read(chip->COM))
    {
      for (int i = 0; i < 16; ++i)
      {
        chip->I[i] = pin_init(pin_names_I[i], OUTPUT);
        pin_write(chip->I[i], LOW);
      }
      
      pin_write(chip->I[pinIndex], HIGH);
    }
    else
    {
      chip->COM = pin_init("COM", ANALOG);
      
      for (int i = 0; i < 16; ++i)
      {
        chip->I[i] = pin_init(pin_names_I[i], ANALOG);
      }

      if (attr_read(chip->analog_demux))
      {
        for (int i = 0; i < 16; ++i)
        {
          pin_dac_write(chip->I[i], 0);
        }
        
        pin_dac_write(chip->I[pinIndex], pin_adc_read(chip->COM));
      }
      else
      {
        pin_dac_write(chip->COM, pin_adc_read(chip->I[pinIndex]));
      }
    }
  }
  else
  {
    chip->COM = pin_init("COM", ANALOG);
    pin_dac_write(chip->COM, 0);
      
    for (int i = 0; i < 16; ++i)
    {
      chip->I[i] = pin_init(pin_names_I[i], ANALOG);
    }

    for (int i = 0; i < 16; ++i)
    {
      pin_dac_write(chip->I[i], 0);
    }
  }
}

void chip_init() 
{
  chip_data_t *chip = (chip_data_t*)malloc(sizeof(chip_data_t));

  for (int i = 0; i < 16; ++i)
  {
    chip->I[i] = pin_init(pin_names_I[i], ANALOG);
  }

  chip->S[0] = pin_init("S0", INPUT);
  chip->S[1] = pin_init("S1", INPUT);
  chip->S[2] = pin_init("S2", INPUT);
  chip->S[3] = pin_init("S3", INPUT);

  chip->COM = pin_init("COM", INPUT);
  chip->VCC = pin_init("VCC", INPUT);
  chip->GND = pin_init("GND", INPUT);
  chip->E = pin_init("E", INPUT);

  chip->analog_demux = attr_init("analog_demux", 0);

  const timer_config_t config = 
  {
    .callback = chip_timer_callback,
    .user_data = chip,
  };

  timer_t timer_id = timer_init(&config);
  timer_start(timer_id, 10000, true);
}