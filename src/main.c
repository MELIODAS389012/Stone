#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "sound.h"
#include "my.h"

void calculateDFT(int len)
{
    int xn[len];
    float Xr[len];
    float Xi[len];
    int i, k, n, N = 0;

    for (i = 0; i < len; i++) {
        printf("Enter the value of x[%d]: ", i);
        scanf("%d", &xn[i]);
    }
    printf("Enter the number of points in the DFT: ");
    scanf("%d", &N);
    for (k = 0; k < N; k++) {
        Xr[k] = 0;
        Xi[k] = 0;
        for (n = 0; n < len; n++) {
            Xr[k] = (Xr[k] + xn[n] * cos(2 * 3.141592 * k * n / N));
            Xi[k] = (Xi[k] - xn[n] * sin(2 * 3.141592 * k * n / N));
        }
        printf("(%f) + j(%f)\n", Xr[k], Xi[k]);
    }
}

short int compute_bytes_per_sample(short int bits_per_sample, short int num_channels) {
    return (bits_per_sample / 8) * num_channels;
}

void generate_wav_file(short int *buffer, int buffer_size_samples)
{
    strncpy(g_generator_header.riff, "RIFF", 4);
    strncpy(g_generator_header.wave, "WAVE", 4);
    strncpy(g_generator_header.fmt, "fmt ", 4);
    strncpy(g_generator_header.data, "data", 4);

    g_generator_header.chunk_size = 16;
    g_generator_header.format_tag = 1;
    g_generator_header.num_chans = 1;
    g_generator_header.srate = G_GENERATOR_SAMPLE_RATE;
    g_generator_header.bits_per_samp = 16;
    g_generator_header.bytes_per_sec = g_generator_header.srate * g_generator_header.bits_per_samp / 8 * g_generator_header.num_chans;
    g_generator_header.bytes_per_samp = compute_bytes_per_sample(g_generator_header.bits_per_samp, g_generator_header.num_chans);

    for (int i = 0; i < buffer_size_samples; i++) {
        buffer[i] = (short int)((cos((2 * M_PI * G_GENERATOR_MIDDLE_C * i) / (double)g_generator_header.srate) * 1000));
    }

    g_generator_header.dlength = buffer_size_samples * g_generator_header.bytes_per_samp;
    g_generator_header.flength = g_generator_header.dlength + 36;
    FILE *fp = fopen("test.wav", "wb");
    if (!fp) {
        perror("Failed to open file");
        exit(84);
    }
    fwrite(g_generator_header.riff, sizeof(char), 4, fp);
    fwrite(&g_generator_header.flength, sizeof(int32_t), 1, fp);
    fwrite(g_generator_header.wave, sizeof(char), 4, fp);
    fwrite(g_generator_header.fmt, sizeof(char), 4, fp);
    fwrite(&g_generator_header.chunk_size, sizeof(int32_t), 1, fp);
    fwrite(&g_generator_header.format_tag, sizeof(int16_t), 1, fp);
    fwrite(&g_generator_header.num_chans, sizeof(int16_t), 1, fp);
    fwrite(&g_generator_header.srate, sizeof(int32_t), 1, fp);
    fwrite(&g_generator_header.bytes_per_sec, sizeof(int32_t), 1, fp);
    fwrite(&g_generator_header.bytes_per_samp, sizeof(int16_t), 1, fp);
    fwrite(&g_generator_header.bits_per_samp, sizeof(int16_t), 1, fp);
    fwrite(g_generator_header.data, sizeof(char), 4, fp);
    fwrite(&g_generator_header.dlength, sizeof(int32_t), 1, fp);
    fwrite(buffer, sizeof(short int), buffer_size_samples, fp);
    fclose(fp);
}

void generator(char *name)
{
    short int *buffer = malloc(G_GENERATOR_BUFFER_SIZE * sizeof(short int));
    if (!buffer) {
        perror("Failed to allocate buffer");
        exit(84);
    }
    memset(buffer, 0, G_GENERATOR_BUFFER_SIZE * sizeof(short int));
    generate_wav_file(buffer, G_GENERATOR_BUFFER_SIZE);
    free(buffer);
}



int load_wav_file_data(const char *filename, struct wav_header *header_out, short int **audio_data_out, int *num_samples_out);
void perform_dft_analysis_and_print(short int *audio_data, int num_samples, int sample_rate, int top_n_to_display);


int main(int ac, char **av)
{
    if (ac == 2 && strcmp(av[1], "--help") == 0) {
        usage();
        return 0;
    }
    if (ac == 4 && strcmp(av[1], "--analyze") == 0) {
        const char *input_filename = av[2];
        int num_top_frequencies = atoi(av[3]);

        if (num_top_frequencies <= 0) {
            fprintf(stderr, "Le nombre N de fréquences à afficher doit être un entier positif.\n");
            return 84;
        }
        struct wav_header audio_header;
        short int *audio_data_buffer = NULL;
        int total_samples_read;

        if (load_wav_file_data(input_filename, &audio_header, &audio_data_buffer, &total_samples_read) != 0) {
            return 84;
        }
        perform_dft_analysis_and_print(audio_data_buffer, total_samples_read, audio_header.srate, num_top_frequencies);
        free(audio_data_buffer);
        audio_data_buffer = NULL;
        return 0;
    }
    if (ac == 5 && strcmp(av[1], "--cypher") == 0) {
        return 0;
    }
    if (ac == 3 && strcmp(av[1], "--decypher") == 0) {
        return 0; 
    }
    usage();
    return 84;
}