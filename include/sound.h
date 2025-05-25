/*
** EPITECH PROJECT, 2025
** G-CNA-400-COT-4-1-stoneanalysis-comble.sotinkon
** File description:
** sound
*/

#ifndef INCLUDED_SOUND_H
#define INCLUDED_SOUND_H

#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct wav_header {
    char riff[4];
    int32_t flength;
    char wave[4];
    char fmt[4];
    int32_t chunk_size;
    int16_t format_tag;
    int16_t num_chans;
    int32_t srate;
    int32_t bytes_per_sec;
    int16_t bytes_per_samp;
    int16_t bits_per_samp;
    char data[4];
    int32_t dlength;
};

extern struct wav_header g_generator_header;
extern const float G_GENERATOR_MIDDLE_C;
extern const int G_GENERATOR_SAMPLE_RATE;
extern const int G_GENERATOR_DURATION_SECONDS;
extern const int G_GENERATOR_BUFFER_SIZE;

short int compute_bytes_per_sample(short int bits_per_sample, short int num_channels);

#endif