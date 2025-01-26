#ifndef DEF_H
#define DEF_H

#include <stdint.h>


enum instructionType {TYPE_I=0, TYPE_S=1, TYPE_B=2, TYPE_J=3, TYPE_R, TYPE_NONE};
enum argtype {ARG_NONE=0, ARG_RS1=1, ARG_RS2=2, ARG_IMM=3, ARG_IMMOFFSET=4 ,ARG_RD=5};

struct instruction {
	
	enum instructionType type;
	uint32_t checkval; //value before hash to check false positive
	
	uint8_t opcode;
	uint8_t funct3;
	uint8_t funct7;
	uint8_t rd, rs1, rs2; //as register number (x..)
	int32_t imm;

	enum argtype argument_types[3];	

};

#endif
