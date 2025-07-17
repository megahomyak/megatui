#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define nullable
#define not_null
#define Char_loop_prev(producer, consumer) for (Char nullable* consumer = producer->prev; consumer != NULL; consumer = consumer->prev)
#define Char_loop_next(producer, consumer) for (Char nullable* consumer = producer->next; consumer != NULL; consumer = consumer->next)
typedef unsigned int uint;
typedef uint WidthLimit /* > 0 */;
typedef uint WidthIndex /* < WidthLimit */;
typedef uint HeightLimit /* > 0 */;
typedef uint HeightIndex /* < HeightLimit */;
typedef struct Char {
    struct Char nullable* prev;
    char content;
    struct Char nullable* next;
} Char;
typedef struct {
    WidthLimit _width_limit;
} ParsingContext;
typedef struct {
    Char not_null* _char;
    ParsingContext not_null* _parsing_context;
} SoftLine;
typedef struct {
    SoftLine _soft_line;
    WidthIndex _selection_char_width_index;
} SelectionSoftLine;
typedef struct {
    bool exists;
    SoftLine soft_line;
} OptionalSoftLine;
typedef struct {
    WidthIndex selection_char_width_index;
    HeightIndex selection_char_height_index;
    Char beginning_char;
} RenderData;
void not_null* die_if_null(void nullable* ptr) {
    if (ptr == NULL) {
        exit(1);
    }
    return ptr;
}
Char not_null* str_to_Char(char not_null* str) {
    if (str[0] == '\0') {
        fprintf(stderr, "Empty str in str_to_Char\n");
        exit(1);
    }
    Char* current = die_if_null(malloc(sizeof(Char)));
    current->prev = NULL;
    current->content = str[0];
    current->next = NULL;
    Char* first = current;
    while (1) {
        ++str;
        if (*str == '\0') break;
        Char* new = die_if_null(malloc(sizeof(Char)));
        new->prev = current;
        new->content = *str;
        new->next = current->next;
        current->next = new;
        current = new;
    }
    return first;
}
uint min(uint a, uint b) {
    return a < b ? a : b;
}
uint saturating_subtract(uint minuend, uint subtrahend) {
    return minuend - min(minuend, subtrahend);
}
ParsingContext ParsingContext_make(WidthLimit width_limit) {
    return (ParsingContext) {
        ._width_limit = width_limit,
    };
}
WidthLimit ParsingContext_get_width_limit(ParsingContext not_null* parsing_context) {
    return parsing_context->_width_limit;
}
SelectionSoftLine SelectionSoftLine_make_from_selection_char(Char not_null* char_, ParsingContext not_null* parsing_context) {
    uint selection_char_hard_line_index = 0;
    Char_loop_prev(char_, _prev) {
        if (_prev->content == '\n') break;
        ++selection_char_hard_line_index;
    }
    WidthIndex width_index = selection_char_hard_line_index % ParsingContext_get_width_limit(parsing_context);
    return (SelectionSoftLine) {
        ._soft_line = (SoftLine) {
            ._char = char_,
            ._parsing_context = parsing_context,
        },
        ._selection_char_width_index = width_index,
    };
}
SoftLine SelectionSoftLine_get_soft_line(SelectionSoftLine not_null* selection_soft_line) {
    return selection_soft_line->_soft_line;
}
SoftLine SelectionSoftLine_get_selection_char_width_index(SelectionSoftLine not_null* selection_soft_line) {
    return selection_soft_line->_selection_char_width_index;
}
Char not_null* SoftLine_get_selected_char(SoftLine not_null* soft_line) {
    return soft_line->_char;
}
OptionalSoftLine SoftLine_get_optional_next(SoftLine not_null* soft_line) {
    // TODO
}
OptionalSoftLine SoftLine_get_optional_prev(SoftLine not_null* soft_line) {
    // TODO
}
RenderData RenderData_make_from_Char_selection(Char not_null* selection, WidthLimit width_limit) {
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
