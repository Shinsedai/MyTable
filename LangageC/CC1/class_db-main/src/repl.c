#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "btree.h"

// Enumeration for handling meta-commands
typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

// Enumeration for statement preparation results
typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

// Enumeration for types of SQL-like statements
typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

// Structure for holding a parsed SQL-like statement
typedef struct {
    StatementType type;
    Row row_to_insert; // Data for the "INSERT" statement
} Statement;

// Input buffer for storing user input
typedef struct {
    char* buffer;       // Pointer to the input string
    size_t buffer_length; // Allocated buffer size
    ssize_t input_length; // Length of the input string
} InputBuffer;

// Global binary tree for storing rows
Node* tree = NULL;

// Allocates and initializes a new input buffer
InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

// Prints the prompt for user input
void print_prompt() {
    printf("db > ");
}

// Custom implementation of getline for reading input
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128; // Initial buffer size
        *lineptr = malloc(*n);
        if (*lineptr == NULL) return -1;
    }

    char *ptr = *lineptr;
    int c;
    size_t len = 0;

    while ((c = fgetc(stream)) != EOF && c != '\n') {
        if (len + 1 >= *n) { // Resize buffer if needed
            *n *= 2;
            ptr = realloc(ptr, *n);
            if (ptr == NULL) return -1;
            *lineptr = ptr;
        }
        ptr[len++] = c;
    }

    if (c == EOF && len == 0) return -1; // End of input
    ptr[len] = '\0';
    return len;
}

// Reads input into the input buffer
void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = '\0';
}

// Prints a single row in a formatted manner
void print_row(Row* row) {
    printf("(%d, %s, %s)\n", row->id, row->name, row->email);
}

// Traverses and prints all rows in the binary tree
void traverse_tree(Node* root) {
    if (root == NULL) return;
    traverse_tree(root->left);   // Traverse left subtree
    print_row(root->data);       // Print the current node
    traverse_tree(root->right);  // Traverse right subtree
}

// Frees the memory allocated for the input buffer
void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

// Executes a meta-command (e.g., ".exit")
MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        free_tree(tree); // Free all nodes in the tree
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

// Parses an "INSERT" statement from the input
PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement) {
    int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s",
                               &statement->row_to_insert.id,
                               statement->row_to_insert.name,
                               statement->row_to_insert.email);

    if (args_assigned < 3) { // Ensure all arguments are present
        return PREPARE_UNRECOGNIZED_STATEMENT;
    }
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
}

// Parses input and determines the type of statement
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        return prepare_insert(input_buffer, statement);
    }

    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

// Executes an "INSERT" statement
void execute_insert(Statement* statement) {
    Row* row = (Row*)malloc(sizeof(Row)); // Allocate memory for the new row
    *row = statement->row_to_insert;     // Copy data into the row
    tree = insert(tree, row);            // Insert the row into the tree
    printf("Inserted row with ID %d.\n", row->id);
}

// Executes a "SELECT" statement
void execute_select(Statement* statement) {
    printf("Select all rows:\n");
    traverse_tree(tree);  // Traverse the tree and print all rows
}

// Executes the prepared statement
void execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            execute_insert(statement);
            break;
        case (STATEMENT_SELECT):
            execute_select(statement);
            break;
    }
}

// The main Read-Eval-Print Loop (REPL)
void repl(void) {
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') { // Handle meta-commands
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
        }
        execute_statement(&statement);
    }
}
