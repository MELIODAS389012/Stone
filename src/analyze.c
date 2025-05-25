#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "sound.h" 
#include "my.h"    
#include <stdbool.h>

typedef struct result{
    double frequency_hz;
    double magnitude;
}result;

int compare_frequency_results_desc(const void *a, const void *b) {
    result *res_a = (result *)a;
    result *res_b = (result *)b;

    if (res_a->magnitude < res_b->magnitude) return 1;
    if (res_a->magnitude > res_b->magnitude) return -1;
    return 0;
}

void perform_dft_analysis_and_print(short int *audio_data, int num_samples, int sample_rate, int top_n_to_display)
{
    int N_dft = num_samples;

    if (N_dft <= 0) {
        perror("Nombre d'échantillons pour la DFT est invalide\n");
        return;
    }
    double *Xr = (double *)malloc(N_dft * sizeof(double));
    double *Xi = (double *)malloc(N_dft * sizeof(double));
    if (Xr == NULL || Xi == NULL) {
        perror("Échec de l'allocation mémoire pour Xr/Xi.\n");
        free(Xr);
        free(Xi);
        exit(84);
    }
    for (int k = 0; k < N_dft; k++) {
        Xr[k] = 0.0;
        Xi[k] = 0.0;
        for (int n = 0; n < N_dft; n++) {
            double angle = 2.0 * M_PI * (double)k * (double)n / (double)N_dft;
            Xr[k] += (double)audio_data[n] * cos(angle);
            Xi[k] -= (double)audio_data[n] * sin(angle);        
        }
    }
    int num_useful_freqs = N_dft / 2;
    if (num_useful_freqs == 0 && N_dft > 0) num_useful_freqs = 1;

    result *results = (result *)malloc(num_useful_freqs * sizeof(result));
    if (results == NULL) {
        perror("Échec de l'allocation mémoire pour les résultats de fréquence.\n");
        free(Xr);
        free(Xi);
        exit(84);
    }
    for (int k = 0; k < num_useful_freqs; k++) {
        results[k].magnitude = sqrt(Xr[k] * Xr[k] + Xi[k] * Xi[k]);
        results[k].frequency_hz = (double)k * (double)sample_rate / (double)N_dft;
    }
    free(Xr);
    free(Xi);
    qsort(results, num_useful_freqs, sizeof(result), compare_frequency_results_desc);
    printf("Top %d frequencies:\n", top_n_to_display);
    for (int i = 0; i < top_n_to_display && i < num_useful_freqs; i++)
        printf("%.1f Hz\n", results[i].frequency_hz);
    
    free(results);
    results = NULL;
}

bool check_info(struct wav_header info)
{
    if (strncmp(info.riff, "RIFF", 4) != 0) return false;
    if (strncmp(info.wave, "WAVE", 4) != 0) return false;
    if (strncmp(info.fmt, "fmt ", 4) != 0) return false;
    if (strncmp(info.data, "data", 4) != 0) return false;
    if (info.format_tag != 1) return false;
    if (info.num_chans != 1) return false;
    if (info.bits_per_samp != 16) return false;
    if (info.srate != 48000) return false; 
    if (info.chunk_size != 16) return false;
    short int expected_bytes_per_samp = info.num_chans * (info.bits_per_samp / 8);
    if (info.bytes_per_samp != expected_bytes_per_samp) return false;
    int32_t expected_bytes_per_sec = info.srate * info.bytes_per_samp;
    if (info.bytes_per_sec != expected_bytes_per_sec) return false;
    return true; 
}

int load_wav_file_data(const char *filename, struct wav_header *header_out, short int **audio_data_out, int *num_samples_out) {
    FILE *file_ptr = fopen(filename, "rb");
    if (file_ptr == NULL) {
        perror("Error while openning file\n");
        return 1;
    }
    if (fread(header_out, sizeof(struct wav_header), 1, file_ptr) != 1) {
        perror("Error while reading header\n");
        fclose(file_ptr);
        return 1;
    }
    if (!check_info(*header_out)) {
        perror("Error while checking header \"must\"\n");
        fclose(file_ptr);
        return 1;
    }
    int bytes_per_sample = header_out->bits_per_samp / 8;
    *num_samples_out = header_out->dlength / bytes_per_sample;
    if (*num_samples_out <= 0) {
        perror("Error, had nothing to read\n");
        fclose(file_ptr);
        return 1;
    }
    *audio_data_out = (short int *)malloc(*num_samples_out * sizeof(short int));
    if (*audio_data_out == NULL) {
        perror("Error while allocating memory for audio data\n");
        fclose(file_ptr);
        return 1;
    }
    size_t samples_actually_read = fread(*audio_data_out, sizeof(short int), *num_samples_out, file_ptr);
    if (samples_actually_read != (size_t)*num_samples_out) {
        perror("Error while reading audio data\n");
        free(*audio_data_out);
        *audio_data_out = NULL;
        fclose(file_ptr);
        return 1;
    }
    if (fclose(file_ptr) != 0) {
        perror("Error while closing file\n");
    }
    return 0;
}

