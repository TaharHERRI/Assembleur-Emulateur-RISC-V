#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

#define REG_SP 2
#define REG_RA 1
#define REG_PC 32


struct emulator_state{

	int64_t registers[33]; //32 registers + PC
	int32_t memory[4096];

};


#endif
