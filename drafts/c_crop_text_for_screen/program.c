#include <stdlib.h>
#include <stdio.h>

typedef struct Char {
    struct Char* prev;
    char value;
    struct Char* next;
} Char;

void display(Char* cursor, int width, int height) {
    int line_cursor_index = 0;
    {
        Char* current = cursor;
        while ((current = current->next) != NULL) {
            line_cursor_index++;
        }
    }
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
