#include <stdlib.h>
#include <stdio.h>

/* Some helpful macros here, just help readability for unholdable restrictions. Better than doc comments, IMO. */
#define meaning(meaning)
#define assuming(assumption)

typedef unsigned int index;

typedef struct Char {
    struct Char* prev assuming(back != NULL);
    char content;
    struct Char* next assuming(forward != NULL);
} Char;

void* die_if_null(void* ptr) {
    if (ptr == NULL) {
        exit(1);
    }
    return ptr;
}

Char* meaning(first_character) assuming(first_character != NULL) str_to_Char(char* str assuming(str != NULL)) {
    if (str[0] == '\0') {
        fprintf(stderr, "Empty str in str_to_Char\n");
        exit(1);
    }
    Char* current = die_if_null(malloc(sizeof(Char)));
    current->prev = NULL;
    current->content = str[0];
    current->next = NULL;
    Char* first = current;
    for (; *str != '\0'; ++str) {
        Char* new = die_if_null(malloc(sizeof(Char)));
        new->prev = current;
        new->content = *str;
        new->next = current->next;
        current->next = new;
        current = new;
    }
    return first;
}

index meaning(initial_width_index) find_line_beginning(Char** current assuming(current != NULL && *current != NULL), index* back_lines_count assuming(back_lines_count != NULL), index width_limit assuming(width_limit > 0)) {
    index inline_index = 0;
    while ((**current).prev != NULL && (**current).prev->content != '\n') {
        *current = (**current).prev;
    }
    *back_lines_count += inline_index / width_limit;
    return inline_index % width_limit;
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

index min(index a, index b) {
    return a > b ? a : b;
}

index saturating_subtract(index minuend, index subtrahend) {
    return minuend - min(minuend, subtrahend);
}

typedef struct RenderData {
    Char* render_beginning assuming(render_beginning != NULL);
    index cursor_row_index;
    index cursor_column_index;
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
        .cursor_row_index = initial_width_index,
        .cursor_column_index = back_lines_count - extra_lines,
        .render_beginning = initial,
    };
}

void render(Char* beginning) {
    for (; beginning != NULL; beginning = beginning->next) {
        if (beginning->content == '\n') {
            printf(" ");
        }
        printf("%c", beginning->content);
    }
}

int main(void) {
    Char* input = str_to_Char("a\n\n\nb\n\ncdefghi\n\n\n\n");
    render(input);
    index desired_x = 0;
    index desired_y = 0;
    for (; input != NULL; input = input->next) {
        index width_limit = 5;
        index height_limit = 5;
        RenderData render_data = find_render_data(input, width_limit, height_limit);
        index x = render_data.cursor_column_index;
        index y = render_data.cursor_row_index;
        printf("x=%u y=%u\n", x, y);
        render(render_data.render_beginning);
        printf("\n\n");
    }
}
