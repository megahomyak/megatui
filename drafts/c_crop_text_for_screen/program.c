#include <stdlib.h>
#include <stdio.h>

typedef struct Char {
    struct Char* prev;
    char value;
    struct Char* next;
} Char;

typedef unsigned int uint;

#define DEBUG
#ifdef DEBUG
#define debug_printf(...) printf(__VA_ARGS__)
#else
#define debug_printf(...)
#endif

uint _scan_back(Char** cursor, uint* soft_lines_before, uint width) {
    uint cursor_hard_index = 0;
    while ((**cursor).prev != NULL && (**cursor).prev->value != '\n') {
        *cursor = (**cursor).prev;
        ++cursor_hard_index;
    }
    *soft_lines_before += cursor_hard_index / width;
    return cursor_hard_index % width;
}

uint _min(uint a, uint b) {
    return a > b ? b : a;
}

uint _scan_forward(uint cursor_soft_line_index, Char** cursor, uint max_lines_after, uint width, uint height) {
    uint soft_lines_after = 0;
    do {
        if (soft_lines_after == max_lines_after) {
            break;
        }
        if ((**cursor).value == '\n' || cursor_soft_line_index == width) {
            ++soft_lines_after;
            cursor_soft_line_index = 0;
        } else {
            ++cursor_soft_line_index;
        }
        if ((**cursor).next == NULL) {
            break;
        }
        *cursor = (**cursor).next;
    } while ((**cursor).next);
    return soft_lines_after;
}

void _render_immediately(Char* cursor, uint width, uint height) {
    uint cursor_soft_line_index = 0;
    uint soft_lines = 1;
    do {
        printf("%c", cursor->value);
        ++cursor_soft_line_index;
        if (cursor->value == '\n' || cursor_soft_line_index == width) {
            if (cursor->value != '\n') {
                debug_printf("+\n");
            }
            if (soft_lines == height) {
                return;
            }
            ++soft_lines;
            cursor_soft_line_index = 0;
        }
        cursor = cursor->next;
    } while (cursor);
}

void render(Char* cursor, uint width, uint height) {
    if (width == 0 || height == 0) return;
    // Going in the current line:
    Char* after = cursor;
    uint soft_lines_before = 0;
    uint cursor_soft_line_index = _scan_back(&cursor, &soft_lines_before, width);
    debug_printf("b %c %d %d\n", cursor->value, cursor_soft_line_index, soft_lines_before);
    // Going forward:
    uint soft_lines_after = _scan_forward(cursor_soft_line_index, &after, height - 1, width, height);
    debug_printf("f %c\n", cursor->value);
    uint min_soft_lines_after = _min(soft_lines_after, height/2);
    debug_printf("_ %d\n", min_soft_lines_after);
    uint max_soft_lines_before = height - 1 - min_soft_lines_after;
    // Going back again:
    for (;;) {
        if (max_soft_lines_before == soft_lines_before || cursor->prev == NULL) { // Exactly right or best available
            return _render_immediately(cursor, width, height);
        } else if (max_soft_lines_before > soft_lines_before) { // Not enough lines
            debug_printf("bb %c %d %d %d\n", cursor->value, 0, max_soft_lines_before, soft_lines_before);
            cursor = cursor->prev;
            _scan_back(&cursor, &soft_lines_before, width);
            debug_printf("bb %c %d\n", cursor->value, soft_lines_before);
        } else { // More than enough lines
            soft_lines_before -= _scan_forward(0, &cursor, soft_lines_before - max_soft_lines_before, width, height);
            debug_printf("f %c\n", cursor->value);
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
    for (char* c = "hello, world!\n\n\n\n\n\n\nCRAPPPP\n1234567890abcdef|lalalalalalala\narstarstarstarstarsntaierthdienshtdienhrstidenhrsitden\nrotendoirsetndoiersntdoiernstodei\nrstidenroistendoiernstodeirnstoid\n\n\n\n\narst\narstarst\narstarstarst\n\n\n\n"; *c; ++c) {
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
    render(cursor, 8, 6);
}
