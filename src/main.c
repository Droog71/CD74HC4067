// CD74HC4067
// 16-Channel Analog Multiplexer
// https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

//! Channel pin names.
const char* pin_names_I [16] = { 
  "I0", "I1", "I2", "I3",
  "I4", "I5", "I6", "I7", 
  "I8", "I9", "I10", "I11",
  "I12", "I13", "I14","I15"
};

//! Chip data.
typedef struct {
  pin_t  I[16];
  pin_t  S[4];
  pin_t  VCC;
  pin_t  GND;
  pin_t  COM;
  pin_t  EN;
  uint32_t floatingSig;
  uint32_t analogDemux;
  uint32_t digitalMode;
} chip_data_t;

//! Returns true if VCC and GND are properly connected.
bool power_connected(void *data)
{
  chip_data_t *chip = (chip_data_t*)data;
  return pin_read(chip->VCC) && !pin_read(chip->GND);
}

//! Called when E pin is high or the chip is not powered correctly.
void off_state(void *data)
{
  chip_data_t *chip = (chip_data_t*)data;

  if (attr_read(chip->floatingSig))
  {
    pin_mode(chip->COM, INPUT);
  }
  else
  {
    pin_mode(chip->COM, ANALOG);
    pin_dac_write(chip->COM, 0);
  }
    
  for (int i = 0; i < 16; ++i)
  {
    pin_mode(chip->I[i], INPUT);
  }
}

//! Returns the currently selected channel.
uint8_t selected_channel(void *data)
{
    chip_data_t *chip = (chip_data_t*)data;
    bool S0 = pin_read(chip->S[0]);
    bool S1 = pin_read(chip->S[1]);
    bool S2 = pin_read(chip->S[2]);
    bool S3 = pin_read(chip->S[3]);
    return S3 << 3 | S2 << 2 | S1 << 1 | S0;
}

//! Sets COM and all of the channel pins to analog mode.
void analog_mode(void *data)
{
  chip_data_t *chip = (chip_data_t*)data;
  pin_mode(chip->COM, ANALOG);
      
  for (int i = 0; i < 16; ++i)
  {
    pin_mode(chip->I[i], ANALOG);
  }
}

//! Sets COM and all of the channel pins to digital mode.
void digital_mode(void *data)
{
  chip_data_t *chip = (chip_data_t*)data;
  pin_mode(chip->COM, OUTPUT);
      
  for (int i = 0; i < 16; ++i)
  {
    pin_mode(chip->I[i], INPUT);
  }
}

//! Sets the selected channel to the same voltage read on the COM pin.
void analog_demux(void *data)
{
  chip_data_t *chip = (chip_data_t*)data;

  for (int i = 0; i < 16; ++i)
  {
    pin_dac_write(chip->I[i], 0);
  }
  
  uint8_t channel = selected_channel(chip);
  pin_dac_write(chip->I[channel], pin_adc_read(chip->COM));
}

//! Drives the selected channel HIGH. Called when COM is also HIGH.
void digital_demux(void *data)
{
  chip_data_t *chip = (chip_data_t*)data;

  for (int i = 0; i < 16; ++i)
  {
    pin_mode(chip->I[i], OUTPUT);
    pin_write(chip->I[i], LOW);
  }
  
  uint8_t channel = selected_channel(chip);
  pin_write(chip->I[channel], HIGH);
}

//! Sets the COM pin to the same voltage read on the selected channel.
void mux(void *data)
{
  chip_data_t *chip = (chip_data_t*)data;
  uint8_t channel = selected_channel(chip);
  if (attr_read(chip->digitalMode))
  {
    pin_write(chip->COM, pin_read(chip->I[channel]));
  }
  else
  {
    pin_dac_write(chip->COM, pin_adc_read(chip->I[channel]));
  }
}

//! Main loop.
void chip_timer_callback(void *data) 
{
  chip_data_t *chip = (chip_data_t*)data;

  if (power_connected(chip) && !pin_read(chip->EN))
  {
    pin_mode(chip->COM, INPUT_PULLDOWN);

    if (pin_read(chip->COM))
    {
      digital_demux(chip);
    }
    else
    {
      if (attr_read(chip->digitalMode))
      {
        digital_mode(chip);
      }
      else
      {
        analog_mode(chip);  
      }
      
      if (attr_read(chip->analogDemux))
      {
        analog_demux(chip);
      }
      else
      {
        mux(chip);
      }
    }
  }
  else
  {
    off_state(chip);
  }
}

//! Chip initialization.
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
  chip->EN = pin_init("EN", INPUT);

  chip->floatingSig = attr_init("floatingSig", 0);
  chip->analogDemux = attr_init("analogDemux", 0);
  chip->digitalMode = attr_init("digitalMode", 0);

  const timer_config_t config = 
  {
    .callback = chip_timer_callback,
    .user_data = chip,
  };

  timer_t timer_id = timer_init(&config);
  timer_start(timer_id, 10000, true);
}
