#include <stdlib.h>
#include <stdio.h>

typedef struct Char {
    struct Char* prev;
    char value;
    struct Char* next;
} Char;

typedef unsigned int uint;

uint scan_back(Char** cursor) {
    uint cursor_hard_index = 0;
    while ((**cursor).prev != NULL && (**cursor).value != '\n') {
        *cursor = (**cursor).prev;
        ++cursor_hard_index;
    }
    return cursor_hard_index;
}

uint min(uint a, uint b) {
    return a > b ? b : a;
}

void render(Char* cursor, uint width, uint height) {
    if (width == 0 || height == 0) return;
    // Going in the current line:
    Char* after = cursor;
    uint cursor_hard_index = scan_back(&cursor);
    uint lines_before = cursor_hard_index / width;
    uint cursor_soft_line_index = cursor_hard_index % width;
    // Going forward:
    uint lines_after = 0;
    {
        uint soft_line_index = cursor_hard_index;
        do {
            if (lines_after == height - 1) {
                break;
            }
            if (after->value == '\n' || soft_line_index == width - 1) {
                ++lines_after;
                soft_line_index = 0;
            }
            if (after->next == NULL) {
                break;
            }
            after = after->next;
        } while (after->next);
    }
    uint min_lines_after = min(lines_after, height/2);
    uint max_lines_before = height - 1 - min_lines_after;
    // Going back again:
    for (;;) {
        if (max_lines_before == lines_before || cursor->prev == NULL) { // Exactly right or best available

        } else if (max_lines_before > lines_before) { // Not enough lines

        } else { // More than enough lines

        }
    }
 // should be enough since I'm working with \n<this>\n
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

    `scan_back` loop, position recovery:
    * EXISTS<-a # break can't happen
    * EXISTS<-\n # end of line; needs +1 for correctness
    * MISSING<-a # end of line correct
    * MISSING<-\n # either needs +1 or doesn't (if the \n is from the current attempt or from the previous one)
    HOW TO FIX:
    since we're only doing `scan_back` if prev!=NULL, we can just safely do a "-1" before `scan_back`
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
    render(cursor, 4, 4);
}
