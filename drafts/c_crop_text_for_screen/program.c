#include <stdlib.h>
#include <stdio.h>

#define nullable
#define not_null

typedef unsigned int uint;
typedef unsigned int uint_nonzero;

typedef struct Char {
    struct Char nullable* prev;
    char content;
    struct Char nullable* next;
} Char;

void not_null* die_if_null(void nullable* ptr) {
    if (ptr == NULL) {
        exit(1);
    }
    return ptr;
}

// Tested, correct
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

typedef struct {
    Char* char_;
    uint input_soft_line_index;
    uint back_hard_lines_count;
} HardLineBeginning;

// Tested, correct
HardLineBeginning find_line_beginning(Char not_null* current, uint_nonzero width_limit) {
    uint input_hard_line_index = 0;
    while (current->prev != NULL && current->prev->content != '\n') {
        ++input_hard_line_index;
        current = current->prev;
    }
    return (HardLineBeginning) {
        .input_soft_line_index = input_hard_line_index % width_limit,
        .char_ = current,
        .back_hard_lines_count = input_hard_line_index / width_limit,
    };
}

index meaning(forward_lines_count) get_forward_lines_count(Char* current assuming(current != NULL), index width_limit assuming(width_limit > 0), index current_width_index assuming(current_width_index < width_limit), index forward_lines_limit) {
    index forward_lines_count = 0;
    for (; current->next != NULL && forward_lines_count < forward_lines_limit; current = current->next) {
        ++current_width_index;
        if (current_width_index == width_limit) {
            current_width_index = 0;
            ++forward_lines_count;
        }
        if (current->content == '\n') {
            current_width_index = 0;
            ++forward_lines_count;
        }
    }
    return forward_lines_count;
}

// Obviously correct
index min(index a, index b) {
    return a < b ? a : b;
}

// Obviously correct
index saturating_subtract(index minuend, index subtrahend) {
    return minuend - min(minuend, subtrahend);
}

typedef struct RenderData {
    Char* render_beginning assuming(render_beginning != NULL);
    index cursor_x;
    index cursor_y;
} RenderData;

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
