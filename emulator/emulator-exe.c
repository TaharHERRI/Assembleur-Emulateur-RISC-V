#include "emulator-exe.h"

int instruction_cycle(struct emulator_state* state){

	struct instruction instr;
	
	uint32_t hex = state->memory[(state->registers[REG_PC])/4];
	if(hex == 0) return 0;
	decode(hex, &instr); 
    	
	
	execute_instruction(&instr, state);

	state->registers[REG_PC] += 4;
	
	return 1;	

}


//hash would be slightly better but not worth the time
void execute_instruction(struct instruction* instr, struct emulator_state* state){

	switch(instr->type){
		case TYPE_I:
			if(instr->funct3)
				exe_ld(instr,state);
			else
				exe_addi(instr, state);
			break;
		case TYPE_R:
			if(instr->funct7)
				exe_sub(instr,state);
			else
				exe_add(instr, state);
			break;
		case TYPE_S:
			exe_sd(instr,state);
			break;
		case TYPE_J:
			exe_jal(instr, state);
			break;
		default:
			switch(instr->funct3){
				case 0:
					exe_beq(instr,state);
					break;
				case 1:
					exe_bne(instr, state);
					break;
				case 4: 
					exe_blt(instr,state);
					break;
				case 5: 
					exe_bge(instr,state);
					break;

			}
	}

	state->registers[0] = 0;

}



//rd = rs1 + rs2    
void exe_add(struct instruction* instr, struct emulator_state* state){
    uint8_t index_rd = instr->rd;

    uint8_t index_rs1 = instr->rs1;
    int64_t val_rs1 = state->registers[index_rs1];

    uint8_t index_rs2 = instr->rs2;
    int64_t val_rs2 = state->registers[index_rs2];

    state->registers[index_rd]  = val_rs1 + val_rs2;

    
}

//rd = rs1 - rs2    
void exe_sub(struct instruction* instr, struct emulator_state* state){
    uint8_t index_rd = instr->rd;

    uint8_t index_rs1 = instr->rs1;
    int64_t val_rs1 = state->registers[index_rs1];

    uint8_t index_rs2 = instr->rs2;
    int64_t val_rs2 = state->registers[index_rs2];

    state->registers[index_rd]  = val_rs1 - val_rs2;

    
}

//rd = rs1 + imm    
void exe_addi(struct instruction* instr, struct emulator_state* state){
    uint8_t index_rd = instr->rd;

    uint8_t index_rs1 = instr->rs1;
    int64_t val_rs1 = state->registers[index_rs1];

    int32_t val_imm = instr->imm;
    val_imm = (val_imm << 20) >> 20;
    
    state->registers[index_rd]  = val_rs1 + val_imm;
}

//rd = M[rs1+imm]   
//The purpose here is to take the bloc of 32 bits M[(rs1+imm)/4] to load it in the first 32 bits of rd
//and then to take the bloc M[((rs1+imm)/4)+1] to load it in the next 32 bits of rd
void exe_ld(struct instruction* instr, struct emulator_state* state){
    uint8_t index_rd = instr->rd;

    uint8_t index_rs1 = instr->rs1;
    int64_t val_rs1 = state->registers[index_rs1];

    int32_t val_imm = instr->imm;
    val_imm = (val_imm << 20) >> 20;

    uint16_t index_mem_1 = (val_imm + val_rs1)/4;
    uint16_t index_mem_2 = ((val_imm + val_rs1)/4)+1;

    uint64_t mask_1 = 0b1111111111111111111111111111111100000000000000000000000000000000;
    uint64_t tmp_rd_1 = ((uint64_t)(state->memory[index_mem_1]) << 32 ) & mask_1;

    uint64_t mask_2 = 0b0000000000000000000000000000000011111111111111111111111111111111;
    uint64_t tmp_rd_2 = (state->memory[index_mem_2]) & mask_2;

    state->registers[index_rd] = tmp_rd_1 | tmp_rd_2;

  
}

//M[rs1+imm] = rs2    
void exe_sd(struct instruction* instr, struct emulator_state* state){
    uint8_t index_rs1 = instr->rs1;
    int64_t val_rs1 = state->registers[index_rs1];

    uint8_t index_rs2 = instr->rs2;
    int64_t val_rs2 = state->registers[index_rs2];

    int32_t val_imm = instr->imm;
    val_imm = (val_imm << 20) >> 20;

    uint16_t index_mem_1 = (val_imm + val_rs1)/4;
    uint16_t index_mem_2 = ((val_imm + val_rs1)/4)+1;

    uint64_t mask_1 = 0b1111111111111111111111111111111100000000000000000000000000000000;
    uint64_t tmp_rs2_1 = (val_rs2 & mask_1) >> 32;
    state->memory[index_mem_1] = tmp_rs2_1;

    uint64_t mask_2 = 0b0000000000000000000000000000000011111111111111111111111111111111;
    uint64_t tmp_rs2_2 = (val_rs2 & mask_2);
    state->memory[index_mem_2] = tmp_rs2_2;

    
}

//if(rs1 == rs2)    PC += imm
void exe_beq(struct instruction* instr, struct emulator_state* state){
    int32_t val_imm = instr->imm;
    val_imm = (val_imm << 19) >> 19;
    if(state->registers[instr->rs1] == state->registers[instr->rs2]) state->registers[REG_PC] += val_imm-4;
}

//if(rs1 != rs2)    PC += imm
void exe_bne(struct instruction* instr, struct emulator_state* state){
    int32_t val_imm = instr->imm;
    val_imm = (val_imm << 19) >> 19;
   if(state->registers[instr->rs1] != state->registers[instr->rs2]) state->registers[REG_PC] += val_imm-4;
    
}

//if(rs1 < rs2)    PC += imm
void exe_blt(struct instruction* instr, struct emulator_state* state){
    int32_t val_imm = instr->imm;
    val_imm = (val_imm << 19) >> 19;
   if(state->registers[instr->rs1] < state->registers[instr->rs2]) state->registers[REG_PC] += val_imm-4;
    
}

//if(rs1 >= rs2)    PC += imm
void exe_bge(struct instruction* instr, struct emulator_state* state){
    int32_t val_imm = instr->imm;
    val_imm = (val_imm << 19) >> 19;
   if(state->registers[instr->rs1] >= state->registers[instr->rs2]) state->registers[REG_PC] += val_imm-4;
    
}

//rd = PC+4;    PC += imm
void exe_jal(struct instruction* instr, struct emulator_state* state){
    int32_t val_imm = instr->imm;
    val_imm = (val_imm << 11) >> 11;
    state->registers[instr->rd] = state->registers[REG_PC] + 4;
    state->registers[REG_PC] += val_imm-4;
}
