#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
typedef unsigned int uint;
typedef struct { uint notnull; } uint_notnull;
void check_notnull(void* ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Empty ptr\n");
        exit(EXIT_FAILURE);
    }
}
typedef struct char_list {
    struct char_list* prev;
    char content;
    struct char_list* next;
} char_list;
typedef struct { char_list* notnull; } char_list_notnullp;
char_list_notnullp char_list_notnull(char_list* char_list) {
    check_notnull(char_list);
    return (char_list_notnullp) { char_list };
}
typedef struct { char_list* nullable; } soft_line_nullablep;
typedef struct { char_list_notnullp notnull; } soft_line_notnullp;

typedef struct { char* notnull; } char_notnullp;

void* malloc_notnull(size_t size) {
    void* ptr = malloc(size);
    check_notnull(ptr);
    return ptr;
}
char_list_notnullp str_to_char_list(char_notnullp str) {
    if (str.notnull[0] == '\0') {
        fprintf(stderr, "Empty str\n");
        exit(EXIT_FAILURE);
    }
    char_list_notnullp current = { malloc_notnull(sizeof(char_list)) };
    current.notnull->prev = NULL;
    current.notnull->content = str.notnull[0];
    current.notnull->next = NULL;
    char_list_notnullp first = current;
    while (1) {
        ++str.notnull;
        if (*str.notnull == '\0') break;
        char_list_notnullp new = { malloc_notnull(sizeof(char_list)) };
        new.notnull->prev = current.notnull;
        new.notnull->content = *str.notnull;
        new.notnull->next = current.notnull;
        current.notnull->next = new.notnull;
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
soft_line_notnullp get_soft_line(char_list_notnullp current_line, uint_notnull width_limit) {
    uint index_in_hard_line = 0;
    for (char_list* prev_char = current_line.notnull->prev; prev_char != NULL; prev_char = prev_char->prev) {
        char_list_notnullp prev_notnull = { notnull(prev_char) };
        if (prev_notnull.notnull->content == '\n') break;
        ++index_in_hard_line;
    }
    uint width_index = index_in_hard_line % width_limit.notnull;
    char_list_notnullp beginning_char = current_line;
    for (uint i = 0; i < width_index; ++i) {
        beginning_char = (char_list_notnullp) { notnull(beginning_char.notnull->prev) };
    }
    return (soft_line_notnullp) { beginning_char };
}
soft_line_nullablep get_next_soft_line(soft_line_notnullp current_line, uint_notnull width_limit) {
    char_list_notnullp beginning = current_line.notnull;
    for (uint i = 0; i < width_limit.notnull; ++i) {
        if (beginning.notnull->next == NULL) return (soft_line_nullablep) { NULL };
        else {
            bool newline_found = beginning.notnull->content == '\n';
            beginning = (char_list_notnullp){ notnull(beginning.notnull->next) };
            if (newline_found) break;
        }
    }
    return (soft_line_nullablep) { beginning.notnull };
}
soft_line_nullablep get_prev_soft_line(soft_line_notnullp current_line, uint_notnull width_limit) {
    char_list* prev = current_line.notnull.notnull->prev;
    if (prev == NULL) return (soft_line_nullablep) { NULL };
    else return (soft_line_nullablep) { get_soft_line((char_list_notnullp) { notnull(prev) }, width_limit).notnull.notnull };
}
soft_line_notnullp find_render_beginning(soft_line_notnullp current_line, uint_notnull height_limit, uint_notnull width_limit) {
    const uint current_lines = 1;
    uint after_lines = 0;
    soft_line_notnullp after_line = current_line;
    while (after_lines + current_lines < height_limit.notnull) {
        soft_line_nullablep after_line_nullable = get_next_soft_line(current_line, width_limit);
        if (after_line_nullable.nullable == NULL) break;
        else {
            after_line = (soft_line_notnullp) { notnull(after_line_nullable.nullable) };
        }
    }
}
RenderData RenderData_make(uint_notnull width_limit, uint_notnull height_limit, Char* selected_char__notnull) {
    SoftLine selected_line = SoftLine_make(selected_char__notnull, width_limit);
    const uint lines_selected = 1;
    uint lines_after = 0;
    SoftLine line_after = selected_line;
    while (lines_after + lines_selected < height_limit) {
        OptionalSoftLine optional_line_after = SoftLine_try_get_next(&line_after);
        if (optional_line_after.exists__RO) {
            ++lines_after;
            line_after = optional_line_after.soft_line__RO;
        } else {
            break;
        }
    }
    uint max_lines_before = subtract_saturating(height_limit, lines_selected + min(lines_after, height_limit / 2));
    uint lines_before = 0;
    SoftLine line_before = selected_line;
    while (lines_before < max_lines_before) {
        OptionalSoftLine optional_line_before = SoftLine_try_get_prev(&line_before);
        if (optional_line_before.exists__RO) {
            line_before = optional_line_before.soft_line__RO;
            ++lines_before;
        } else {
            break;
        }
    }
    SoftLine beginning_line = line_before;
    return (RenderData) {
        .beginning_line__RO = beginning_line,
        .selected_char__RO_notnull = selected_char__notnull,
    };
}
void SoftLine_render(SoftLine* line__notnull) {
    Char* end = SoftLine_try_get_next__nullable_private(line__notnull);
    Char_iter(line__notnull->beginning_char__RO_notnull, next) {
        if (next == end) break;
        printf("%c", next->content == '\n' ? ' ' : next->content);
    }
    printf("\n");
}
void render(RenderData* render_data__notnull, uint_notnull height_limit) {
    SoftLine line = render_data__notnull->beginning_line__RO;
    repeat(height_limit) {
        SoftLine_render(&line);
        OptionalSoftLine next = SoftLine_try_get_next(&line);
        if (next.exists__RO) {
            line = next.soft_line__RO;
        } else {
            break;
        }
    }
}
int main(void) {
    Char* selected_char__notnull = str_to_Char__notnull("Sa\n\n\nb\n\ncdefghi\n\n\n\nE");
    printf("Input:\n");
    Char_iter_next(selected_char__notnull, next__notnull) {
        printf("%c", next__notnull->content);
    }
    printf("\n");
    const uint_notnull width_limit = 4;
    const uint_notnull height_limit = 4;
    uint shift = 0;
    Char* selected_char__nullable = selected_char__notnull;
    while (selected_char__nullable != NULL) {
        Char* selected_char__notnull = die_if_null__notnull(selected_char__nullable);
        printf("Shift %u:\n", shift);
        ++shift;
        RenderData render_data = RenderData_make(width_limit, height_limit, selected_char__notnull);
        selected_char__nullable = selected_char__notnull->next__nullable;
    }
    // render(&render_data, width_limit, height_limit);
}
