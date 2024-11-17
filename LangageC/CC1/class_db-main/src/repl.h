// repl.h
#ifndef REPL_H
#define REPL_H

void repl();
struct InputBuffer* new_input_buffer();
void print_prompt();
ssize_t getline(char** lineptr, size_t* n, FILE* stream);
void read_input(struct InputBuffer* input_buffer);
void print_row(struct Row* row);
void traverse_tree(struct Node* node);
void close_input_buffer(struct InputBuffer* input_buffer);
void do_meta_command(struct InputBuffer* input_buffer);
void prepare_insert(struct InputBuffer* input_buffer);
void prepare_statement(struct InputBuffer* input_buffer);
void execute_insert(struct InputBuffer* input_buffer);
void execute_select(struct InputBuffer* input_buffer);
void execute_statement(struct InputBuffer* input_buffer);

extern struct Node* tree;

#endif // REPL_H