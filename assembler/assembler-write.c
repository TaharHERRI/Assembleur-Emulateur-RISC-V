#include "assembler-write.h"

void to_hex (uint32_t in, char hex[8]){
	static const char h[] = "0123456789ABCDEF";

	for(int i = 7; i >= 0; i --){
		hex[i] = h[in & 0xF];
		in = in >> 4;
	}

}

void line_to_hex(struct instruction* instr, char hex[8]){
    uint32_t line = 0b11111111111111111111111111111111;
    
    switch(instr->type){
        case TYPE_R:
//fprintf(outf,"%x/%x/%x/%x/%x/%x\n", instr->funct7,instr->rs2,instr->rs1,instr->funct3, instr->rd,instr->opcode);
            line = FUNCT7(instr) | RS2(instr) | RS1(instr) | FUNCT3(instr) | RD(instr) | OPCODE(instr); break;
        case TYPE_I:
//fprintf(outf,"%03x/%x/%x/%x/%x\n", instr->imm,instr->rs1,instr->funct3, instr->rd,instr->opcode);
            line = IMM_I(instr) | RS1(instr) | FUNCT3(instr) | RD(instr) | OPCODE(instr); break;
        case TYPE_S:
//fprintf(outf,"%x/%x/%x/%x/%x\n", instr->imm, instr->rs2, instr->rs1, instr->funct3, instr->opcode);
            line = IMM_S(instr) | RS2(instr) | RS1(instr) | FUNCT3(instr) | OPCODE(instr); break;
        case TYPE_B:
//fprintf(outf,"%x/%x/%x/%x/%x\n", instr->imm, instr->rs2, instr->rs1, instr->funct3, instr->opcode);
            line = IMM_B(instr) | RS2(instr) | RS1(instr) | FUNCT3(instr) | OPCODE(instr); break;
        case TYPE_J:
//fprintf(outf,"%x/%x/%x\n", instr->imm, instr->rd, instr->opcode); 
            line = IMM_J(instr) | RD(instr) | OPCODE(instr); break;
        case TYPE_NONE:
            line = 0b00000000000000000000000000000000; break;
    }

    to_hex(line, hex);

}

//each elmnt has 8 bits if we put them on 32 bits it will be like that
// 000000000000000000000000XXXXXXXX
//So to place each elmnt at the right place we always have to shift them to the
//left with the right coefficient

uint32_t FUNCT7(struct instruction* instr){
    uint32_t funct7 = instr->funct7;
    uint32_t mask_funct7 = 0b11111110000000000000000000000000;

    //Here we make sure that funct7 has 7 bits
    funct7 = funct7 << 25;

    return funct7 & mask_funct7;
}

uint32_t FUNCT3(struct instruction* instr){
    uint32_t funct3 = instr->funct3;
    uint32_t mask_funct3 = 0b00000000000000000111000000000000;

    //Here we make sure that funct3 has 3 bits
    funct3 = funct3 << 12;
     
    return funct3 & mask_funct3;
}

uint32_t RS2(struct instruction* instr){
    uint32_t rs2 = instr->rs2;
    uint32_t mask_rs2 = 0b00000001111100000000000000000000;

    rs2 = rs2 << 20;

    return rs2 & mask_rs2;
}

uint32_t RS1(struct instruction* instr){
    uint32_t rs1 = instr->rs1;
    uint32_t mask_rs1 = 0b00000000000011111000000000000000;

    rs1 = rs1 << 15;

    return rs1 & mask_rs1;
}

uint32_t RD(struct instruction* instr){
    uint32_t rd = instr->rd;
    uint32_t mask_rd = 0b00000000000000000000111110000000;

    rd = rd << 7;

    return rd & mask_rd;
}

uint32_t OPCODE(struct instruction* instr){
    uint32_t opcode = instr->opcode;
    uint32_t mask_opcode = 0b00000000000000000000000001111111;

    return opcode & mask_opcode;
}

int32_t IMM_I(struct instruction* instr){
    int32_t imm_I = instr->imm;
    int32_t mask_imm_I = 0b11111111111100000000000000000000;

    imm_I = imm_I << 20;

    return imm_I & mask_imm_I;
}

int32_t IMM_S(struct instruction* instr){
    int32_t imm_S = instr->imm;;
    int32_t mask_imm_S = 0b11111110000000000000111110000000;

    int32_t mask_imm_S1 = 0b00000000000000000000111111100000;
    int32_t mask_imm_S2 = 0b00000000000000000000000000011111;

    int32_t imm_S1 =  (imm_S & mask_imm_S1) << 20;
    int32_t imm_S2 =  (imm_S & mask_imm_S2) << 7;
    return (imm_S1 | imm_S2) & mask_imm_S;
}


int32_t IMM_B(struct instruction* instr){
    int32_t imm_B = instr->imm;
    int32_t mask_imm_B = 0b11111110000000000000111110000000;

    int32_t mask_imm_B1_1 = 0b00000000000000000001000000000000;
    int32_t mask_imm_B1_2 = 0b00000000000000000000011111100000;
    int32_t mask_imm_B2_1 = 0b00000000000000000000100000000000;
    int32_t mask_imm_B2_2 = 0b00000000000000000000000000011110;
      
    //Here we put everything at its place like mentioned in the datasheet
    int32_t imm_B1_1 = (imm_B & mask_imm_B1_1) << 19;
    int32_t imm_B1_2 = (imm_B & mask_imm_B1_2) << 20;
    int32_t imm_B2_1 = (imm_B & mask_imm_B2_1) >> 4;
    int32_t imm_B2_2 = (imm_B & mask_imm_B2_2) << 7;

    return ((imm_B1_1 | imm_B1_2) | (imm_B2_1 | imm_B2_2)) & mask_imm_B;
}



int32_t IMM_J(struct instruction* instr){
    int32_t imm_J = instr->imm;
    int32_t mask_imm_J = 0b11111111111111111111000000000000;

    int32_t mask_imm_J1 = 0b00000000000100000000000000000000;
    int32_t mask_imm_J2 = 0b00000000000000000000011111111110;
    int32_t mask_imm_J3 = 0b00000000000000000000100000000000;
    int32_t mask_imm_J4 = 0b00000000000011111111000000000000;
    
    //Here we put everything at its place like mentioned in the datasheet
    int32_t imm_J1 = (imm_J & mask_imm_J1) << 11;
    int32_t imm_J2 = (imm_J & mask_imm_J2) << 20;
    int32_t imm_J3 = (imm_J & mask_imm_J3) << 9;
    int32_t imm_J4 = (imm_J & mask_imm_J4);

    return ((imm_J1 | imm_J2 | imm_J3 | imm_J4)) & mask_imm_J;
}

