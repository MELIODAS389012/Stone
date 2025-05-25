/*
** EPITECH PROJECT, 2025
** G-CNA-400-COT-4-1-stoneanalysis-comble.sotinkon
** File description:
** usage.c
*/


#include "my.h"

void usage(void)
{
    printf("USAGE\n");
    printf("./stone_analysis [--analyze IN_FILE N | --cypher ");
    printf("IN_FILE OUT_FILE MESSAGE | --decypher IN_FILE]\n\n");
    printf("IN_FILE     An audio file to be analyzed\n");
    printf("OUT_FILE    Output audio file of the cypher mode\n");
    printf("MESSAGE     The message to hide in the audio file\n");
    printf("N         Number of top frequencies to display\n");
}
