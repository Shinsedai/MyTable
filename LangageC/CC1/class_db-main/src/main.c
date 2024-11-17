#include <stdio.h>
#include <stdlib.h>
#include "repl.h"
#include <stdbool.h>
#include <string.h>

// Entry point of the program
int main(int argc, char* argv[], char* envp[]) {
    repl();                // Start the REPL (Read-Eval-Print Loop)
    return 0;              // Exit the program
}
