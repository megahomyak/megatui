#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
typedef unsigned int uint;
typedef struct char_list {
    struct char_list* prev_nullable;
    char content;
    struct char_list* next_nullable;
} char_list;
void* malloc_notnull(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "ptr is null\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
char_list* str_to_char_list_notnull(char* str_notnull) {
    if (str_notnull[0] == '\0') {
        fprintf(stderr, "empty str\n");
        exit(EXIT_FAILURE);
    }
    char_list* current_notnull = malloc_notnull(sizeof(char_list));
    current_notnull->prev_nullable = NULL;
    current_notnull->content = str_notnull[0];
    current_notnull->next_nullable = NULL;
    char_list* first_notnull = current_notnull;
    while (1) {
        ++str_notnull;
        if (*str_notnull == '\0') break;
        char_list* new_notnull = malloc_notnull(sizeof(char_list));
        new_notnull->prev_nullable = current_notnull;
        new_notnull->content = *str_notnull;
        new_notnull->next_nullable = current_notnull->next_nullable;
        current_notnull->next_nullable = new_notnull;
        current_notnull = new_notnull;
    }
    return first_notnull;
}
uint min(uint a, uint b) {
    return a < b ? a : b;
}
uint subtract_saturating(uint minuend, uint subtrahend) {
    return minuend - min(minuend, subtrahend);
}
char_list* get_soft_line_notnull(char_list* current_char_notnull, uint width_limit_notzero) {
    uint index_in_hard_line = 0;
    for (char_list* prev_char_nullable = current_char_notnull->prev_nullable; prev_char_nullable != NULL; prev_char_nullable = prev_char_nullable->prev_nullable) {
        if (prev_char_nullable->content == '\n') break;
        ++index_in_hard_line;
    }
    uint width_index = index_in_hard_line % width_limit_notzero;
    char_list* beginning_char_notnull = current_char_notnull;
    for (uint i = 0; i < width_index; ++i) {
        beginning_char_notnull = beginning_char_notnull->prev_nullable;
    }
    return beginning_char_notnull;
}
char_list* get_next_soft_line_nullable(char_list* current_soft_line_notnull, uint width_limit_notzero) {
    char_list* beginning_char_notnull = current_soft_line_notnull;
    for (uint i = 0; i < width_limit_notzero; ++i) {
        if (beginning_char_notnull->next_nullable == NULL) return NULL;
        else {
            bool newline_found = beginning_char_notnull->content == '\n';
            beginning_char_notnull = beginning_char_notnull->next_nullable;
            if (newline_found) break;
        }
    }
    return beginning_char_notnull;
}
char_list* get_prev_soft_line_nullable(char_list* current_soft_line_notnull, uint width_limit_notzero) {
    char_list* prev_char_nullable = current_soft_line_notnull->prev_nullable;
    if (prev_char_nullable == NULL) return NULL;
    else return get_soft_line_notnull(prev_char_nullable, width_limit_notzero);
}
char_list* find_render_beginning_soft_line_notnull(char_list* current_soft_line_notnull, uint width_limit_notzero, uint height_limit_notzero) {
    const uint current_soft_lines = 1;
    uint after_soft_lines = 0;
    char_list* after_soft_line_nullable = current_soft_line_notnull;
    while (after_soft_lines + current_soft_lines < height_limit_notzero) {
        after_soft_line_nullable = get_next_soft_line_nullable(after_soft_line_nullable, width_limit_notzero);
        if (after_soft_line_nullable == NULL) break;
        else ++after_soft_lines;
    }
    uint max_before_soft_lines = subtract_saturating(height_limit_notzero, current_soft_lines + min(after_soft_lines, height_limit_notzero / 2));
    uint before_soft_lines = 0;
    char_list* before_soft_line_notnull = current_soft_line_notnull;
    while (before_soft_lines < max_before_soft_lines) {
        char_list* before_soft_line_nullable = get_prev_soft_line_nullable(before_soft_line_notnull, width_limit_notzero);
        if (before_soft_line_nullable == NULL) break;
        else {
            ++before_soft_lines;
            before_soft_line_notnull = before_soft_line_nullable;
        }
    }
    return before_soft_line_notnull;
}
void render(char_list* beginning_soft_line_notnull, uint width_limit_notzero, uint height_limit_notzero) {
    char_list* beginning_soft_line_nullable = beginning_soft_line_notnull;
    for (uint i = 0;;) {
        char_list* current_char_nullable = beginning_soft_line_nullable;
        beginning_soft_line_nullable = get_next_soft_line_nullable(beginning_soft_line_nullable, width_limit_notzero);
        for (; current_char_nullable != beginning_soft_line_nullable; current_char_nullable = current_char_nullable->next_nullable) {
            char content = current_char_nullable->content;
            printf("%c", content == '\n' ? ' ' : content);
        }
        ++i;
        if (beginning_soft_line_nullable == NULL || i == height_limit_notzero) break;
        printf("\n");
    }
    printf("\n");
}
int main(void) {
    char_list* input_nullable = str_to_char_list_notnull("Sa\n\n\nb\n\ncdefghi\n\n\n\nE");
    const uint width_limit_notzero = 6;
    const uint height_limit_notzero = 4;
    uint shift = 0;
    while (input_nullable != 0) {
        printf("Shift %u:\n", shift);
        ++shift;
        render(find_render_beginning_soft_line_notnull(input_nullable, width_limit_notzero, height_limit_notzero), width_limit_notzero, height_limit_notzero);
        input_nullable = input_nullable->next_nullable;
    }
}
