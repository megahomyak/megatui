#include <stdlib.h>
#include <stdio.h>

typedef struct Char {
    struct Char* prev;
    char value;
    struct Char* next;
} Char;

typedef unsigned int uint;

/* cursor should be non-null when passed in */
void display(Char* cursor, uint width, uint height) {
    if (width == 0 || height == 0) return;
    /*
    go back the same line, counting characters traveled
    calculate on what character in its line the cursor is
    get the amt of lines after
    get how many lines should go before
    */
    // go back the same line, counting characters traveled
    uint line_cursor_index = 0;
    Char* cursor_line = cursor;
    while (cursor_line->prev != NULL && cursor_line->prev->value != '\n') {
        cursor_line = cursor_line->prev;
        ++line_cursor_index;
    }

    // calculate on what character in its line the cursor is
    uint screen_line_cursor_index = line_cursor_index % width;
    uint lines_before = line_cursor_index / width;

    // get the amt of lines after
    uint lines_after = 0;
    for (;;) {
        if (cursor->value == '\n' || screen_line_cursor_index == width) {
            screen_line_cursor_index = 0;
            ++lines_after;
            if (lines_before + lines_after + 1 >= height) {
                break;
            }
        }
        cursor = cursor->next;
        if (cursor == NULL) break;
        ++screen_line_cursor_index;
    }

    // get how many lines should go before
    // DO NOT TOUCH THE FUCKING "CURSOR_LINE"'s NAMING!!!! IT'S LIKE THAT FOR A REASON! IF BEFORE THE CURSOR LINE THERE'S NOTHING, WHAT'S THE CHARACTER GONNA BE? NULL? SO IT'S NOT "BEFORE_CURSOR_LINE"!!!!
    screen_line_cursor_index = 0;
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
    for (char* c = "hello, world!\n\n\n\n\n\n\nCRAPPPPP\n|lalalalalalala\narstarstarstarstarsntaierthdienshtdienhrstidenhrsitden\nrotendoirsetndoiersntdoiernstodei\nrstidenroistendoiernstodeirnstoid\n\n\n\n\narst\narstarst\narstarstarst\n\n\n\n"; *c; ++c) {
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
    display(cursor, 20, 5);
}
