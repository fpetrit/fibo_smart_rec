#include <stdio.h>
#include <stdlib.h>

#include "assembler/assembler.h"
#include "runtime/runtime.h"
#include "Label_vector.c"

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

    Label_vector * labels = Label_vector_construct();

    unsigned char response = parse(src, &labels);

    void assemble(src, output, &labels);

    // close source file

    fclose(src);

    // if no error (response is 0), begin execution of ./hexa.txt

    if (response == 0)
        run(output);

    // close output file

    fclose(output);

    return 0;
}