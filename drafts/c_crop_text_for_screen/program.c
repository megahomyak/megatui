#include <stdlib.h>
#include <stdio.h>

typedef struct Char {
    struct Char* prev;
    char value;
    struct Char* next;
} Char;

typedef unsigned int uint;

void display(Char* cursor, uint width, uint height) {
    if (width == 0 || height == 0) return;
    Char* cursor_line = cursor;
    uint cursor_in_hard_line_index = 0;
    while (cursor_line->prev != NULL && cursor_line->prev->value != '\n') {
        cursor_in_hard_line_index++;
        cursor_line = cursor_line->prev;
    }
    uint cursor_in_soft_line_index = cursor_in_hard_line_index % width;
    uint count_of_lines_before_cursor_soft_line = cursor_in_hard_line_index / width;
    uint current_char_in_soft_line_index = 0;
    while (count_of_lines_before_cursor_soft_line != 0) {
        current_char_in_soft_line_index++;
        if (current_char_in_soft_line_index == width) {
            count_of_lines_before_cursor_soft_line--;
            current_char_in_soft_line_index = 0;
        }
    } // should be enough since I'm working with \n<this>\n
}

void die_if_null(void* ptr) {
    if (ptr == NULL) {
        exit(1);
    }
}

int main(void) {
    Char* current = malloc(sizeof(Char));
    die_if_null(current);
    current->prev = NULL;
    current->value = '1';
    current->next = NULL;
    Char* cursor = NULL;
    for (char* c = "hello, world!\n\n\n\n\n\n\nCRAPPPPP\n12341234123412|lalalalalalala\narstarstarstarstarsntaierthdienshtdienhrstidenhrsitden\nrotendoirsetndoiersntdoiernstodei\nrstidenroistendoiernstodeirnstoid\n\n\n\n\narst\narstarst\narstarstarst\n\n\n\n"; *c; ++c) {
        if (*c == '|') {
            cursor = current;
        } else {
            current->next = malloc(sizeof(Char));
            die_if_null(current->next);
            current->next->prev = current;
            current = current->next;
            current->next = NULL;
            current->value = *c;
        }
    }
    die_if_null(cursor);
    display(cursor, 4, 4);
}
