all: simulateur

simulateur:
		gcc -g -o simulateur -Wall main.c assembler/assembler.c assembler/error_checking.c assembler/Label_vector.c assembler/opstring_mapping.c assembler/Error.c runtime/runtime.c runtime/Instruction_vector.c 

mrproper:
	rm simulateur hexa.txt