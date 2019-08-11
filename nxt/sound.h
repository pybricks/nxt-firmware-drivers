#ifndef SOUND_H_
#define SOUND_H_

#include <stdint.h>

void sound_init();

void sound_freq(uint32_t freq, uint32_t ms, int vol);
void sound_play_sample(uint8_t *data, uint32_t length, uint32_t freq, int vol);
int sound_add_sample(uint8_t *data, uint32_t length, uint32_t freq, int vol);
void sound_set_volume(int vol);
int sound_get_volume();
int sound_get_time();
void sound_wait();
void sound_reset();

#define MAXVOL 100

#endif /*SOUND_H_*/
