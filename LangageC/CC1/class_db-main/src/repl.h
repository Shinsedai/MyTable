#ifndef REPL_H
#define REPL_H

#include <stddef.h> // For size_t and ssize_t
#include <stdio.h>  // For FILE*

// Function prototypes for REPL-related operations
void repl(); // Starts the Read-Eval-Print Loop

// Struct declarations
struct InputBuffer;
struct Row;
struct Node;

// Functions for handling user input
struct InputBuffer* new_input_buffer();                      // Allocates and initializes a new input buffer
void print_prompt();                                         // Prints the command prompt
ssize_t getline(char** lineptr, size_t* n, FILE* stream);    // Reads a line from the input stream
void read_input(struct InputBuffer* input_buffer);           // Reads user input into the input buffer
void close_input_buffer(struct InputBuffer* input_buffer);   // Frees memory for the input buffer

// Functions for handling database rows and trees
void print_row(struct Row* row);                             // Prints a single row
void traverse_tree(struct Node* node);                       // Traverses and prints all rows in the tree

// Functions for processing and executing commands
int do_meta_command(struct InputBuffer* input_buffer);       // Executes meta-commands (e.g., ".exit")
int prepare_insert(struct InputBuffer* input_buffer);        // Prepares an INSERT statement
int prepare_statement(struct InputBuffer* input_buffer);     // Prepares a SQL-like statement
void execute_insert(struct InputBuffer* input_buffer);       // Executes an INSERT statement
void execute_select(struct InputBuffer* input_buffer);       // Executes a SELECT statement
void execute_statement(struct InputBuffer* input_buffer);    // Executes a parsed statement

// Extern declaration for the global tree
extern struct Node* tree; // Global binary tree

#endif // REPL_H
