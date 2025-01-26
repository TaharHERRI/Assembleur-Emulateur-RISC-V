#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "assembler-read.h"
#include "assembler-write.h"

//gets line from read_buffer starting from index into line_buffer, updating index
//returns index of last char if OK
//	  -1 if no more \n
int get_line(char read_buffer[MAX_READ_BUFFER], char line_buffer[MAX_LINE_SIZE], int *index, int index_max);

int assemble(FILE* inf, FILE* outf);

#endif
