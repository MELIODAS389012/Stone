/*
** EPITECH PROJECT, 2025
** G-CNA-400-COT-4-1-stoneanalysis-comble.sotinkon
** File description:
** generator_globals
*/
#include "sound.h"

struct wav_header g_generator_header;
const float G_GENERATOR_MIDDLE_C = 900.00f;
const int G_GENERATOR_SAMPLE_RATE = 100000;
const int G_GENERATOR_DURATION_SECONDS = 10;
const int G_GENERATOR_BUFFER_SIZE = G_GENERATOR_SAMPLE_RATE * G_GENERATOR_DURATION_SECONDS;