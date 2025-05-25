/*
** EPITECH PROJECT, 2024
** str_space.c
** File description:
** str_to_word_array espace
*/

#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "my.h"
#include "sound.h"

int space_lines(char *filename, char c)
{
    int lignes = 0;
    int i = 0;

    while (filename[i] != 0) {
    if (filename[i] == c) {
    lignes++;
    }
    i++;
    }
    return lignes + 1;
}

int space_columns(char *filename)
{
    int j = 0;

    while (filename[j] != '\0') {
    j++;
    }
    return j;
}

char **str_to_array_space(char *filename, char c)
{
    int a = 0;
    int i = space_lines(filename, c);
    int j = space_columns(filename);
    char **tab = savers(j, i);

    i = 0;
    j = 0;
    while (filename[a] != '\0' && tab[i] != NULL) {
        if (filename[a] != c) {
        tab[i][j] = filename[a];
        j++;
        }
        if (filename[a] == c) {
        tab[i][j] = '\0';
        j = 0;
        i++;
        }
        a++;
    }
    tab[i + 1] = NULL;
    return tab;
}
