#include <stdio.h>
#include <stdlib.h>

#include "assembler/assembler.h"
#include "runtime/runtime.h"
#include "assembler/opstring_mapping.h"


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

    Assembly_response response;
    response.src_line[0] = '\0';
    assemble(src, output, &response);

    // test the error code and display if error

    if (response.response_code != 0){
        print_response(&response);
        fclose(src);
        fclose(output);
        return 1;
    }

    // close source file

    fclose(src);

    // if no error, begin execution of ./hexa.txt

    // run(output);

    // close output file

    fclose(output);

    return 0;
}