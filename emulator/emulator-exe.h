#ifndef EMULATOR_EXE_H
#define EMULATOR_EXE_H

#include "../include/def.h"
#include "../include/emulator.h"
#include "emulator-load.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



void exe_add(struct instruction* instr, struct emulator_state* state);
void exe_sub(struct instruction* instr, struct emulator_state* state);
void exe_addi(struct instruction* instr, struct emulator_state* state);
void exe_ld(struct instruction* instr, struct emulator_state* state);
void exe_sd(struct instruction* instr, struct emulator_state* state);
void exe_beq(struct instruction* instr, struct emulator_state* state);
void exe_bne(struct instruction* instr, struct emulator_state* state);
void exe_blt(struct instruction* instr, struct emulator_state* state);
void exe_bge(struct instruction* instr, struct emulator_state* state);
void exe_jal(struct instruction* instr, struct emulator_state* state);

//returns 1 if exit, 0 otherwise
int instruction_cycle(struct emulator_state* state);

void execute_instruction(struct instruction* instr, struct emulator_state* state);


#endif
