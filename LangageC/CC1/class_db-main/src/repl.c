#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "btree.h"
Node* delete(Node* root, int id);

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

typedef struct {
    StatementType type;
    Row row_to_insert;
    Row row_to_select;
} Statement;

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

Node* tree = NULL;

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}

void print_prompt() {
    printf("db > ");
}

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = malloc(*n);
        if (*lineptr == NULL) return -1;
    }

    char *ptr = *lineptr;
    int c;
    size_t len = 0;

    while ((c = fgetc(stream)) != EOF && c != '\n') {
        if (len + 1 >= *n) {
            *n *= 2;
            ptr = realloc(ptr, *n);
            if (ptr == NULL) return -1;
            *lineptr = ptr;
        }
        ptr[len++] = c;
    }
    ptr[len] = '\0';

    if (c == EOF && len == 0) return -1;
    ptr[len] = '\0';
    return len;
}

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    if (input_buffer->buffer[bytes_read - 1] == '\n') {
        input_buffer->input_length = bytes_read - 1;
        input_buffer->buffer[bytes_read - 1] = '\0';
    } else {
        input_buffer->input_length = bytes_read;
    }
}

void print_row(Row* row) {
    printf("(%d, %s, %s)\n", row->id, row->name, row->email);
}

void traverse_tree(Node* root) {
    if (root == NULL) return;
    traverse_tree(root->left);
    print_row(root->data);
    traverse_tree(root->right);
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        free_tree(tree);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement) {
    int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s",
                               &statement->row_to_insert.id,
                               statement->row_to_insert.name,
                               statement->row_to_insert.email);

    if (args_assigned < 3) {
        return PREPARE_UNRECOGNIZED_STATEMENT;
    }
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        return prepare_insert(input_buffer, statement);
    }

    if (strncmp(input_buffer->buffer, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        int id;
        int args_assigned = sscanf(input_buffer->buffer, "select %d", &id);
        if (args_assigned == 1) {
            statement->row_to_select.id = id;
        } else {
            statement->row_to_select.id = -1;
        }
        return PREPARE_SUCCESS;
    }
    else if (strncmp(input_buffer->buffer, "delete", 6) == 0) {
        int id;
        sscanf(input_buffer->buffer, "delete %d", &id);
        tree = delete(tree, id);
        printf("Deleted node with ID %d\\n", id);
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_insert(Statement* statement) {
    Row* row = (Row*)malloc(sizeof(Row));
    *row = statement->row_to_insert;
    tree = insert(tree, row);
    printf("Inserted row with ID %d.\n", row->id);
}

void execute_select(Statement* statement) {
    if (statement->row_to_select.id == -1) {
        traverse_tree(tree);
    } else {
        Row* row = search(tree, statement->row_to_select.id);
        if (row != NULL) {
            print_row(row);
        } else {
            printf("Row with ID %d not found.\n", statement->row_to_select.id);
        }
    }
}

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

void repl(void) {
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                continue;
            }
        }

        if (strncmp(input_buffer->buffer, "delete ", 7) == 0) {
            int id;
            if (sscanf(input_buffer->buffer, "delete %d", &id) == 1) {
                tree = delete(tree, id);
                printf("Deleted node with ID %d\n", id);
                continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                execute_statement(&statement);
            break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
            continue;
        }
    }
}
