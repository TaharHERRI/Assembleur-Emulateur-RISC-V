#include "assembler.h"

#define MAX_WRITE_LINE 1024

int assemble(FILE* inf, FILE* outf){
	
	struct instruction instr;
	
	char *read_buffer = malloc(MAX_LINE_SIZE); //getline() buffer needs to be malloced	
	size_t n = MAX_LINE_SIZE;
	int read;

	char write_buffer[9*MAX_WRITE_LINE];
	int write_index = 0;
	
	while((read = getline(&read_buffer, &n, inf)) != -1){
		if(read_buffer[0] == '#' || read_buffer[0] == '\n') continue;
		if(read_text_instruction(read_buffer, read-1, &instr) == -1)
			return -1;	
		

		//buffering fwrite for speeeeeeeeed
		line_to_hex(&instr, &(write_buffer[write_index*9]));
		write_index ++;
		write_buffer[write_index*9-1] = '\n';
		if(write_index == MAX_WRITE_LINE){
			fwrite(write_buffer , sizeof(char)*9, MAX_WRITE_LINE, outf);
			write_index = 0;
		}
	}
	fwrite(write_buffer , sizeof(char)*9, write_index, outf);

	return 1;

}

