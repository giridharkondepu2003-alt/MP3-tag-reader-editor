#ifndef MAIN_H
#define MAIN_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void view(char *filename);

void edit(char *frame_id, char *new_value, char *filename);

void error();

void help();

#endif