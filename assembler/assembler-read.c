#include "assembler.h"


char burn_spaces(char* line, int* index){
	while(line[*index] == ' '){
		(*index)++;
	}
	
	return line[*index];

}

int read_text_to_delim(char* line, char delim, int* index, uint32_t* target){

	*target = 0;
	int itter = 0;


	for(;line[*index] != delim && line[*index] != ' '; (*index)++){
	
		if(*index == MAX_LINE_SIZE || itter == 6) return -1; //valid inputs range from 2-5 chars
		
		*target |= (line[*index] << (itter*8));
		itter++;
	}

	//space burn before delim
	if(delim != ' ' && burn_spaces(line, index) != delim){
		fprintf(stderr, "E : Syntax error - unexpected character '%c' in line : %s\n", line[*index], line);
	        return -1;
	}	       

	//space burn after delim
	(*index)++;
	burn_spaces(line, index);

	return 0;
}

int read_text_integer(char* line, int* index, char delim, int32_t* target){
	
	int sign = 1;
	*target = 0;

	if(line[*index] == '-'){
		sign = -1;
		(*index)++;
	}	

	
	//while x is not a number
	while(line[*index] >= 0x30 && line[*index] <= 0x39){ //delims are <30
		*target = *target * 10 + line[*index]-0x30;
		(*index)++;
	}

	*target *= sign;

	//space burn before delim
	if(burn_spaces(line, index) != delim){
		fprintf(stderr,"E : Syntax error - invalid character '%c' in line : %s\n", line[*index], line);
		return -1;
	}

	(*index)++;
//burning after delim 
	burn_spaces(line, index);

return 0;
	
}

int get_instruction_template(char* line, int *n, struct instruction* target){

	//indexed by instruction hash
	static const struct instruction templates[] ={
		{TYPE_NONE, 0, 0,0,0,0,0,0,0, {ARG_NONE, ARG_NONE, ARG_NONE}}, //none
		{TYPE_B, 6645602, 0x63, 5, 0, 0,0,0,0, {ARG_RS1, ARG_RS2, ARG_IMM}}, //bge
		{TYPE_NONE, 0, 0,0,0,0,0,0,0, {ARG_NONE, ARG_NONE, ARG_NONE}}, //none
		{TYPE_NONE, 0, 0,0,0,0,0,0,0, {ARG_NONE, ARG_NONE, ARG_NONE}}, //none
		{TYPE_I, 30317, 0x13, 0, 0, 0,0,0,0, {ARG_RD, ARG_RS1, ARG_NONE}}, //mv
		{TYPE_B, 7431522, 0x63, 0, 0, 0,0,0,0, {ARG_RS1, ARG_RS2, ARG_IMM}}, //beq
		{TYPE_NONE, 0,0,0,0,0,0,0,0, {ARG_NONE, ARG_NONE, ARG_NONE}}, //none
		{TYPE_S, 25715, 0x23, 3, 0, 0,0,0,0, {ARG_RS2, ARG_IMMOFFSET, ARG_NONE}}, //sd
		{TYPE_NONE, 0,0,0,0,0,0,0,0, {ARG_NONE, ARG_NONE, ARG_NONE}}, //none
		{TYPE_J, 106, 0x6F, 0, 0, 0,0,0,0, {ARG_IMM, ARG_NONE, ARG_NONE}}, //j 
		{TYPE_I, 26988, 19, 0, 0, 0,0,0,0, {ARG_RD, ARG_IMM, ARG_NONE}}, //li
		{TYPE_I, 1768186977, 0x13, 0, 0, 0,0,0,0, {ARG_RD, ARG_RS1, ARG_IMM}}, //addi
		{TYPE_I, 25708, 3, 3, 0, 0,0,0,0, {ARG_RD, ARG_IMMOFFSET, ARG_NONE}}, // ld
		{TYPE_R, 6579297, 51, 0, 0, 0,0,0,0, {ARG_RD, ARG_RS1, ARG_RS2}}, //add
		{TYPE_R, 6452595, 51, 0, 0x20, 0,0,0,0, {ARG_RD, ARG_RS1, ARG_RS2}}, //sub	
		{TYPE_B, 7629922, 0x63, 4, 0, 0,0,0,0, {ARG_RS1, ARG_RS2, ARG_IMM}}, //blt
		{TYPE_B, 6647394, 0x63, 1, 0, 0,0,0,0, {ARG_RS1, ARG_RS2, ARG_IMM}}, //bne
		{TYPE_J, 7102826, 0x6F, 0, 0, 0,0,0,0, {ARG_RD, ARG_IMM, ARG_NONE}}, //jal
		{TYPE_NONE, 0,0,0,0,0,0,0,0, {ARG_NONE, ARG_NONE, ARG_NONE}} //none
	};
	
	uint32_t hash;
	if(read_text_to_delim(line, ' ', n, &hash) == -1)
		return -1;

	//doing the hash
	*target=templates[(hash&HASH_MASK)%19];

	if(target->checkval != hash){
		fprintf(stderr, "E : unrecognized instruction (checkval) on line : %s\n", line);
		return -1;
	}

	return 0;

}

int read_text_register(char* line, int* index, char delim, uint8_t* target){
	
	uint32_t val;

	//x..
	if(line[*index] == 'x'){

		(*index)++;
		if(read_text_integer(line, index, delim, (int32_t*) &val) == -1)
			return -1;


		if(val >= 32){
			fprintf(stderr, "E : Syntax error - invalid register 'x%d' in line : %s\n", val, line);
			return -1;
		}

		*target = val&0xFF;
		return 0;
	}

	
	//mnemonic
	if(read_text_to_delim(line, delim, index, &val) == -1)
		return -1;

	*target = hash_reg(val);

	if(*target == 32)
		return -1;
		
	return 0;
}
	

int read_arg_rs1(char* line, int* index, struct instruction* instr){
	
	return read_text_register(line, index, ',', &(instr->rs1));
}

int read_arg_rs2(char* line, int* index, struct instruction* instr){

	return read_text_register(line, index, ',', &(instr->rs2));
}

int read_arg_rd(char* line, int* index, struct instruction* instr){

	return read_text_register(line, index, ',', &(instr->rd));
}

int read_arg_imm(char* line, int* index, struct instruction* instr){
	static const int32_t maxval[] = {
		8191,
		8191,
		16383,
		4194303
	};
	
	if(read_text_integer(line, index, ',', &(instr->imm)) == -1)
		return -1;
	
	if(abs(instr->imm) > maxval[instr->type]){
		fprintf(stderr, "E : Immediate value %d is above type max %d\n", instr->imm, maxval[instr->type]);
		return -1;
	}

	return 0;
}

int read_arg_immoffset(char* line, int* index, struct instruction* instr){
	if(read_text_integer(line, index, '(', &(instr->imm)) == -1)
		return -1;
	return read_text_register(line, index, ')', &(instr->rs1));
	
}


int read_text_instruction(char buffer[MAX_LINE_SIZE], int read_index, struct instruction* instr){

	//indexed by enum argtype
	static int (* const routines[]) (char*, int*, struct instruction*) = {
		NULL, 
		&read_arg_rs1, 
		&read_arg_rs2, 
		&read_arg_imm, 
		&read_arg_immoffset,
		&read_arg_rd
	};

	int index = 0;

	buffer[read_index] = ','; //normalizing field delimiter
		
	if(get_instruction_template(buffer, &index, instr) == -1 || instr->type == TYPE_NONE){
		fprintf(stderr, "E : Syntax error - Unrecognized instruction on line : %s\n", buffer);
		return -1;
	}

	for(int i = 0; i < 3; i ++){
		if(instr->argument_types[i] == ARG_NONE) break;

		if(routines[instr->argument_types[i]](buffer, &index, instr) == -1){
			printf("E : Syntax error - Unrecognized argument %d on line : %s\n", i, buffer);
			return -1;
		}
	}

	return 0;

}

// hash generated by gperf and adapted to use the same input format as the homemade instruction hash
//
// could be improved a lot with more time
unsigned int
hash_reg (unsigned int in)
{
	static const uint8_t table[] = {
		6,
		27,
		32,
		32,
		32,
		9,
		4,
		25,
		32,
		32,
		11,
		2,
		24,
		32,
		32,
		5,
		26,
		32,
		32,
		32,
		8,
		28,
		32,
		32,
		32,
		10,
		19,
		32,
		32,
		32,
		31,
		13,
		32,
		32,
		32,
		22,
		7,
		32,
		32,
		32,
		16,
		18,
		32,
		32,
		32,
		30,
		12,
		32,
		32,
		32,
		21,
		23,
		0,
		32,
		32,
		15,
		17,
		32,
		32,
		32,
		29,
		1,
		32,
		32,
		32,
		20,
		3,
		32,
		32,
		32,
		14,
		32,
		32,
		32,
		32,
		32,
		32,
		32,
		32,
		32
	};

	static const int checktable[] = {0x31740000, 0x31317300, 0x20, 0x20, 0x20, 0x31730000, 0x70740000, 0x39730000, 0x20, 0x20, 0x31610000, 0x70730000, 0x38730000, 0x20, 0x20, 0x30740000, 0x30317300, 0x20, 0x20, 0x20, 0x30730000, 0x33740000, 0x20, 0x20, 0x20, 0x30610000, 0x33730000, 0x20, 0x20, 0x20, 0x36740000, 0x33610000, 0x20, 0x20, 0x20, 0x36730000, 0x32740000, 0x20, 0x20, 0x20, 0x36610000, 0x32730000, 0x20, 0x20, 0x20, 0x35740000, 0x32610000, 0x20, 0x20, 0x20, 0x35730000, 0x37730000, 0x6f72657a, 0x20, 0x20, 0x35610000, 0x37610000, 0x20, 0x20, 0x20, 0x34740000, 0x61720000, 0x20, 0x20, 0x20, 0x34730000, 0x70670000,0x20, 0x20, 0x20, 0x34610000, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20}; 


	//ugly last minute adaptation to gperf input format
	int val = in;
	while( (val&0xFF000000) == 0)
		val = val << 8;
  
  
  static const unsigned char asso_values[] =
    {
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 15,  0,
      36, 21, 60, 45, 30, 46,  7,  2, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 10, 73, 73,
      73, 73, 73, 60, 73, 73, 73, 73, 73, 73,
      73,  0,  6, 73, 51,  5,  0, 73, 73, 73,
      73, 73, 73, 73, 73, 73, 73, 73
    };
       char val1=asso_values[(val&0xFF0000) >> 16];
       char val2= asso_values[(val&0xFF000000) >> 24];
     	int i = val1+val2+((val&0x100) >> 8);

	if(checktable[i] != val){
		fprintf(stderr," E : Syntax error - unrecognized register (checkval)\n");
		return 32;
	}

       return table[i];
}

//UNUSED
//temporary until I figure out a hash
int fake_register_hash(int val){

	switch (val){
		case 0x6f72657a:
			return 0;
		case 0x6172:
			return 1;
		case 0x7073:
			return 2;
		case 0x7067:
			return 3;
		case 0x7074:
			return 4;
		case 0x3074:
			return 5;
		case 0x3174:
			return 6;
		case 0x3274:
			return 7;
		case 0x3073:
			return 8;
		case 0x3173:
			return 9;
		case 0x3061:
			return 10;
		case 0x3161:
			return 11;
		case 0x3261:
			return 12;
		case 0x3361:
			return 13;
		case 0x3461:
			return 14;
		case 0x3561:
			return 15;
		case 0x3661:
			return 16;
		case 0x3761:
			return 17;
		case 0x3273:
			return 18;
		case 0x3373:
			return 19;
		case 0x3473:
			return 20;
		case 0x3573:
			return 21;
		case 0x3673:
			return 22;
		case 0x3773:
			return 23;
		case 0x3873:
			return 24;
		case 0x3973:
			return 25;
		case 0x303173:
			return 26;
		case 0x313173:
			return 27;
		case 0x3374:
			return 28;
		case 0x3474:
			return 29;
		case 0x3574:
			return 30;
		case 0x3674:
			return 31;

	}

	return 32;
	
}


