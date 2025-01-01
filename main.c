#include <stdio.h>
#include <stdlib.h>

#include "assembler/assembler.h"
#include "assembler/Label_vector.h"
#include "runtime/runtime.h"

int main(int argc, char ** argv){

    // test argc

    if (argc != 2){
        fprintf(stderr, "Usage example: ./simulateur pgm.txt\n");
        return 1;
    }

    // open the source file, open a new output file in the same folder, test descriptors

    FILE * src, * output;

    src = fopen(argv[1], "r");

    if (! src){
        perror("source code opening");
        return 1;
    }

    output = fopen("hexa.txt", "w");

    if (! output){
        perror("output file opening");
        fclose(src);
        return 1;
    }

    // parsing and assembling

    // allocated as a main() local var because usefull for 3 underlying func : parse, assemble & run
    Label_vector * labels = Label_vector_construct();

    // non zero if any error occured
    int response = parse(src, labels);

    if (response == 0)
        assemble(src, output, labels);

    fclose(src);

    // if no error (response is 0), begin execution of ./hexa.txt

    // if (response == 0)
    //     run(output);

    // close output file

    fclose(output);

    return 0;
}