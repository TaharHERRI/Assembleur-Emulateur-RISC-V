#ifndef EMULATOR_LOAD_H
#define EMULATOR_LOAD_H
#include "../include/def.h"
#include "../include/emulator.h"
#include <stdio.h>


// all functions assume the .hex is valid since it always comes from our assembler.



int init_emulator(FILE* inf, struct emulator_state* state);

//
void decode(int32_t val, struct instruction *target);

//returns the instruction type of the given instruction hex
enum instructionType get_instruction_type (int32_t val);


void decode_R(int32_t val, struct instruction *target);
void decode_I(int32_t val, struct instruction *target);
void decode_S(int32_t val, struct instruction *target);
void decode_B(int32_t val, struct instruction *target);
void decode_J(int32_t val, struct instruction *target);

#endif
