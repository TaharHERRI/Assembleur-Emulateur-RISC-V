#include <stdio.h>
#include "assembler.h"

int main(int argc, char **argv)
{
    /* ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT> */
    if(argc != 3) {
        printf("usage: ./riscv-assembler <ASSEMBLER INPUT> <HEX OUTPUT>\n");
        return 1;
    }

    char *asm_input_file = argv[1];
    char *hex_output_file = argv[2];

    FILE* inf = fopen(asm_input_file, "r");
    if(inf == NULL){
	fprintf(stderr, "E : Failed to open input file %s\n", asm_input_file);
	exit(EXIT_FAILURE);
    }

    FILE* outf = fopen(hex_output_file, "w");
    if(outf == NULL){
	fprintf(stderr, "E : Failed to open output file %s\n", hex_output_file);
	exit(EXIT_FAILURE);
    }

    if(assemble(inf, outf) == -1)
	    printf("E : assembly error, exiting\n");
    else
	    printf("I : assembly success !\n");

    fclose(inf);
    fclose(outf);

    return 0;
}
