/*
** EPITECH PROJECT, 2024
** savers.c
** File description:
** allocate str to word array
*/

#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "my.h"

char **savers(int columns, int lignes)
{
    int j;
    char **tableau;

    tableau = malloc(lignes * sizeof(char *));
    if (tableau == NULL) {
    perror("allocation failed");
    exit(EXIT_FAILURE);
    }
    for (j = 0; j < lignes; j++) {
    tableau[j] = malloc(columns * sizeof(char));
    }
    return tableau;
}

