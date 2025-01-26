#include "../include/emulator.h"
#include "emulator-load.h"
#include "emulator-exe.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    /* ./riscv-emulator <HEX INPUT> <EMULATION OUTPUT> */
    if(argc != 3) {
        printf("Usage: ./riscv-emulator <HEX INPUT> <EMULATION OUTPUT>\n");
        printf("error: invalid command\n");
        return 1;
    }

    char *hex_input_file = argv[1];
    char *emu_output_file = argv[2];


    FILE* inf = fopen(hex_input_file, "r");
    if(inf == NULL){
	fprintf(stderr, "E : Failed to open input file %s\n", hex_input_file);
	exit(EXIT_FAILURE);
    }

    FILE* outf = fopen(emu_output_file, "w");
    if(outf == NULL){
	fprintf(stderr, "E : Failed to open output file %s\n", emu_output_file);
	exit(EXIT_FAILURE);
    }

    struct emulator_state state;

    init_emulator(inf, &state);

    while(instruction_cycle(&state) != 0);

    for(int i = 0; i < 32; i++)
	   fprintf(outf, "x%d: %ld\n", i, state.registers[i]);
    
    fclose(inf);
    fclose(outf);

    return 0;
}
