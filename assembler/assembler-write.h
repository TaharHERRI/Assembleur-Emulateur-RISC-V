#ifndef ASSEMBLER_WRITE_H
#define ASSEMBLER_WRITE_H

#include "../include/def.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_LINE_SIZE 32

void line_to_hex(struct instruction* instr, char hex[8]);

uint32_t FUNCT7(struct instruction* instr);
uint32_t FUNCT3(struct instruction* instr);
uint32_t RS2(struct instruction* instr);
uint32_t RS1(struct instruction* instr);
uint32_t RD(struct instruction* instr);
uint32_t OPCODE(struct instruction* instr);
int32_t IMM_I(struct instruction* instr);
int32_t IMM_S(struct instruction* instr);
int32_t IMM_B(struct instruction* instr);
int32_t IMM_J(struct instruction* instr);
#endif
