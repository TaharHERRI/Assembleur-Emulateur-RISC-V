#include "emulator-load.h"

int init_emulator(FILE* inf, struct emulator_state *state){
	
	state->registers[REG_SP] = 16384;
	state->registers[REG_PC] = 0;

	int32_t buffer;
	
	//TODO no scanf ?
	uint32_t program_index = 0;
	while((fscanf(inf, "%08x", &buffer)) == 1){
		
		state->memory[program_index] = buffer;
		program_index ++;
	}

	return 0;
}


enum instructionType get_instruction_type (int32_t val){
	if(val & 4){
		return TYPE_J;
	}
	if((val & 32) == 0){
		return TYPE_I;
	}
	if((val & 96) == 96){
		return TYPE_B;
	}
	if((val & 48) == 48){
		return TYPE_R;
	}
	return TYPE_S;


}


void decode(int32_t val, struct instruction *target){

	static void (* const decode_routines[]) (int32_t val, struct instruction *target) = 
		{
			&decode_I,
			&decode_S,
			&decode_B,
			&decode_J,
			&decode_R
		};
	
	target->type = get_instruction_type(val);
	target->opcode = val & 0x7F;

	decode_routines[target->type] (val, target);

}


void decode_R(int32_t val, struct instruction *target){
	val = val >> 7;
	target->rd = val & 0x1F;
	val = val >> 5;
	target->funct3 = val & 0x7;
	val = val >> 3;
	target->rs1 = val & 0x1F;
	val = val >> 5;
	target->rs2 = val & 0x1F;
	val = val >> 5;
	target->funct7 = val & 0x7F;
	val = val >> 5;
	
}
void decode_I(int32_t val, struct instruction *target){
	val = val >> 7;
	target->rd = val & 0x1F;
	val = val >> 5;
	target->funct3 = val & 0x7;
	val = val >> 3;
	target->rs1 = val & 0x1F;
	val = val >> 5;
	target->imm = val & 0x7FF;
	if(val & 0x800)
		target->imm |= 0xFFFFF800;
}

void decode_S(int32_t val, struct instruction *target){
	val = val >> 7;
	target->imm = val & 0x1F;
	val = val >> 5;
	target->funct3 = val & 0x7;
	val = val >> 3;
	target->rs1 = val & 0x1F;
	val = val >> 5;
	target->rs2 = val & 0x1F;
	val = val >> 5;
	target->imm |= ((val & 0x3F) << 5);
	if(val & 0x40)
		target->imm |= 0xFFFFF800;

}
void decode_B(int32_t val, struct instruction *target){
	val = val >> 7;
	target->imm = ((val & 0x1) << 11);
	val = val >> 1;
	target->imm |= ((val & 0xF) << 1);
	val = val >> 4;
	target->funct3 = val & 0x7;
	val = val >> 3;
	target->rs1 = val & 0x1F;
	val = val >> 5;
	target->rs2 = val & 0x1F;
	val = val >> 5;
	target->imm |= ((val & 0x3F) << 5);
	if(val & 0x40)
		target->imm |= 0xFFFFF000;

}
void decode_J(int32_t val, struct instruction *target){
	val = val >> 7;
	target->rd = val & 0x1F;
	val = val >> 5;
	target->imm = ((val & 0xFF) << 12);
	val = val >> 8;
	target->imm |= ((val & 0x1) << 11);
	val = val >> 1;
	target->imm |= ((val & 0x3FF) << 1);
	if(val & 0x400)
		target->imm |= 0xFFF00000;

}
