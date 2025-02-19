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
    /*
     *
     * Algo from notes:
     * let result1 = Getting first line ("scan_back");
    let result2 = result1;
    Getting forward lines (single loop);
    loop {
    if (is too far back) { // have > needed
    loop forward to needed line and do nothing else
    } else if (just right || current->prev == NULL) { // have == needed
    return result
    } else { // not enough lines back; have < needed
    result2 = Loop a line back ("scan_back")
    }
    }

    Actually, it's a little different: the cursor stands in the middle of the line, while other invocations of `scan_back` will immediately stumble upon an "\n" which will be just the end of the preceding line. I should just probably just go past "\n" when scanning back. But what to do with NULL though? I should probably make two separate algorithms, but in my plans I imagine them as one because they are very similar

    Ya, actually: take the cursor onto "\n", or if there's NULL - don't take: I am checking for "prev=NULL" already, that's the return condition, so it should be fine to stop at "\n" OR before "NULL"

    result:
    * Cursor pointer
    * Cursor on-screen position index

    Task: getting on-screen cursor position index.
    Solution:
    * Remember that in-soft-line offset from the first `scan_back`, that's our `x`
    * Get the count of lines before, that's our `y`
    */
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
