#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define notnull
#define repeat(count, index_receiver) for (uint index_receiver = 0; index_receiver < count; ++index_receiver)
#define Char_loop_prev(producer__notnull, receiver__nullable) for (Char* receiver__nullable = producer__notnull->prev__nullable; receiver__nullable != NULL; receiver__nullable = receiver__nullable->prev__nullable)
#define Char_loop_next(producer__notnull, receiver__nullable) for (Char* receiver__nullable = producer__notnull->next__nullable; receiver__nullable != NULL; receiver__nullable = receiver__nullable->next__nullable)
typedef unsigned int uint;
typedef uint WidthLimit /* > 0 */;
typedef uint WidthIndex /* < WidthLimit */;
typedef uint HeightLimit /* > 0 */;
typedef uint HeightIndex /* < HeightLimit */;
typedef struct Char {
    struct Char* prev__nullable;
    char content;
    struct Char* next__nullable;
} Char;
typedef struct {
    Char* beginning_char__RO_notnull;
    WidthLimit width_limit__RO;
} SoftLine;
typedef struct {
    WidthIndex width_index__RO;
    SoftLine soft_line__RO;
} SelectedChar;
typedef struct {
    bool exists__RO;
    SoftLine soft_line__RO;
} OptionalSoftLine;
typedef struct {
    Char* selected_char__RO_notnull;
    SoftLine beginning_line__RO;
} RenderData;
void notnull* die_if_null(void* ptr__nullable) {
    if (ptr__nullable == NULL) {
        exit(1);
    } else {
        void* ptr__notnull = ptr__nullable;
        return ptr__notnull;
    }
}
Char notnull* str_to_Char(char* str__notnull) {
    if (str__notnull[0] == '\0') {
        fprintf(stderr, "Empty str in str_to_Char\n");
        exit(1);
    }
    Char* current = die_if_null(malloc(sizeof(Char)));
    current->prev__nullable = NULL;
    current->content = str__notnull[0];
    current->next__nullable = NULL;
    Char* first = current;
    while (1) {
        ++str__notnull;
        if (*str__notnull == '\0') break;
        Char* new = die_if_null(malloc(sizeof(Char)));
        new->prev__nullable = current;
        new->content = *str__notnull;
        new->next__nullable = current->next__nullable;
        current->next__nullable = new;
        current = new;
    }
    return first;
}
uint min(uint a, uint b) {
    return a < b ? a : b;
}
uint subtract_saturating(uint minuend, uint subtrahend) {
    return minuend - min(minuend, subtrahend);
}
SelectedChar SelectedChar_make(Char* char__notnull, WidthLimit width_limit) {
    uint index_in_hard_line = 0;
    Char_loop_prev(char__notnull, prev__notnull) {
        if (prev__notnull->content == '\n') break;
        ++index_in_hard_line;
    }
    WidthIndex width_index = index_in_hard_line % width_limit;
    Char* beginning_char__notnull = char__notnull;
    repeat(width_index, _i) {
        beginning_char__notnull = die_if_null(beginning_char__notnull->prev__nullable);
    }
    return (SelectedChar) {
        .soft_line__RO = (SoftLine) {
            .width_limit__RO = width_limit,
            .beginning_char__RO_notnull = beginning_char__notnull,
        },
        .width_index__RO = width_index,
    };
}
OptionalSoftLine SoftLine_try_get_next(SoftLine* soft_line__notnull) {
    // TODO
}
OptionalSoftLine SoftLine_try_get_prev(SoftLine* soft_line__notnull) {
    // TODO
}
RenderData RenderData_make(SelectedChar selected_char) {
    ParsingContext parsing_context = ParsingContext_make(width_limit);
    SoftLine selection_line = SoftLine_make_from_Char(selection, &parsing_context);
    return (RenderData) {
        .beginning = beginning,
        .selection_width_index = selection_width_index,
        .selection_height_index = selection_height_index,
    };
}

RenderData meaning(render_data) assuming(render_data.cursor_row_index < height_limit && render_data.cursor_column_index < width_limit) find_render_data(Char* current, index width_limit assuming(width_limit > 0), index height_limit assuming(height_limit > 0)) {
    Char* initial = current;
    const index current_lines_count = 1;
    index back_lines_count = 0;
    index initial_width_index = find_line_beginning(&current, &back_lines_count, width_limit);
    index forward_lines_limit = saturating_subtract(height_limit, current_lines_count + back_lines_count);
    index forward_lines_count = get_forward_lines_count(initial, width_limit, initial_width_index, forward_lines_limit);
    index back_lines_limit = saturating_subtract(height_limit, current_lines_count + back_lines_count + min(forward_lines_count, height_limit / 2));
    while (back_lines_count < back_lines_limit && initial->prev != NULL) {
        initial = initial->prev;
        ++back_lines_count;
        find_line_beginning(&current, &back_lines_count, width_limit);
    }
    index extra_lines = saturating_subtract(back_lines_count, back_lines_limit);
    for (index i = 0; i < extra_lines; ++i) {
        for (index i = 0; i < width_limit; ++i) {
            initial = initial->next;
        }
    }
    return (RenderData) {
        .render_beginning = initial,
        .cursor_x = initial_width_index,
        .cursor_y = back_lines_count - extra_lines,
    };
}

void render(RenderData render_data, index width_limit assuming(width_limit > 0), index height_limit assuming(height_limit > 0)) {
    index width_index = 0;
    for (Char* beginning = render_data.render_beginning; beginning != NULL; beginning = beginning->next) {
        ++width_index;
        if (beginning->content == '\n') {
            printf(" ");
        }
        printf("%c", beginning->content);
    }
}

void clear_screen() {
    printf("\033[2J\033[H");
}

int main(void) {
    Char* input = str_to_Char("a\n\n\nb\n\ncdefghi\n\n\n\n");
    index width_limit = 5;
    index height_limit = 4;
    char direction = 'f';
    while (1) {
        clear_screen();
        render(find_render_data(input, width_limit, height_limit), width_limit, height_limit);
        char c;
        if (scanf("%c", &c) == EOF || c == 's' /* "stop" */) {
            break;
        } else if (c == 'f' /* "forwards" */) {
            direction = 'f';
        } else if (c == 'b' /* "backwards" */) {
            direction = 'b';
        } else if (c == '\n') {
            if (direction == 'f') {
                if (input->next != NULL) {
                    input = input->next;
                }
            } else if (direction == 'b') {
                if (input->prev != NULL) {
                    input = input->prev;
                }
            }
        }
    }
}
