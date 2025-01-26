#ifndef ASSEMBLER_READ_H
#define ASSEMBLER_READ_H

#include "../include/def.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 32
#define MAX_READ_BUFFER 1024

#define HASH_MASK 1601420374

// read ----------------------

//reads and returns up to 4 chars as an int from index to next delim char. 
//index is updated to point at the char succeeding delim. burning spaces before and after delim
//returns -1 on error 
//	   0 on success
int read_text_to_delim(char* line, char delim, int* index, uint32_t* target);

//reads linebuffer to fill instruction. assumes line is no commentary
//returns -1 on error
//	   0 on success
int read_text_instruction(char buffer[MAX_LINE_SIZE], int read_index, struct instruction* instr);

//reads until space (index in n) and gets template.
//returns -1 if invalid syntax
//	   0 on success
int get_instruction_template(char* line, int *n, struct instruction* target);

//reads from index to delim and converts to reg number.
//index is updated as char succeeding delim, burning spaces
int read_text_register(char* line, int* index, char delim, uint8_t* target);
//reads from index to first non-numerical character (except potential first '-')
//delim is updated as said character
//index is updated as char succeeding delim, burning spaces
int read_text_integer(char* line, int* index, char delim, int32_t* target);

int read_arg_rs1(char* line, int* index, struct instruction* instr);
int read_arg_rs2(char* line, int* index, struct instruction* instr);
int read_arg_rd(char* line, int* index, struct instruction* instr);
int read_arg_imm(char* line, int* index, struct instruction* instr);
int read_arg_immoffset(char* line, int* index, struct instruction* instr);


//returns the next non-space char and updates index
char burn_spaces(char* line, int* index);

//ugly switch until I find a good hash function
int fake_register_hash(int val);

unsigned int hash_reg (unsigned int in);
#endif
