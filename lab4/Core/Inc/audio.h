/*
 * audio.h
 *
 *  Created on: Jan 20, 2023
 *      Author: Ivan
 */

#ifndef INC_AUDIO_H_
#define INC_AUDIO_H_



#endif /* INC_AUDIO_H_ */

#include <gpio.h>
#include "i2c.h"

#define AUDIO_RESET_PIN GPIO_PIN_4
#define AUDIO_I2C_ADDRESS	0x94

void init_AudioReset();
void configAudio();
