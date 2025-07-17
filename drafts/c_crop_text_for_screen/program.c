#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define Char_loop_prev(producer__notnull, receiver__nullable) for (Char* receiver__nullable = producer__notnull->prev__nullable; receiver__nullable != NULL; receiver__nullable = receiver__nullable->prev__nullable)
#define Char_loop_next(producer__notnull, receiver__nullable) for (Char* receiver__nullable = producer__notnull->next__nullable; receiver__nullable != NULL; receiver__nullable = receiver__nullable->next__nullable)
#define repeat(count) for (uint i = 0; i < count; ++i)
typedef unsigned int uint;
typedef uint uint_notnull;
typedef struct Char {
    struct Char* prev__nullable;
    char content;
    struct Char* next__nullable;
} Char;
typedef struct {
    Char* beginning_char__RO_notnull;
    uint width_limit__RO;
} SoftLine;
typedef struct {
    bool exists__RO;
    SoftLine soft_line__RO;
} OptionalSoftLine;
typedef struct {
    Char* selected_char__RO_notnull;
    SoftLine beginning_line__RO;
} RenderData;
void* die_if_null__notnull(void* ptr__nullable) {
    if (ptr__nullable == NULL) {
        exit(1);
    } else {
        void* ptr__notnull = ptr__nullable;
        return ptr__notnull;
    }
}
Char* str_to_Char__notnull(char* str__notnull) {
    if (str__notnull[0] == '\0') {
        fprintf(stderr, "Empty str in str_to_Char\n");
        exit(1);
    }
    Char* current = die_if_null__notnull(malloc(sizeof(Char)));
    current->prev__nullable = NULL;
    current->content = str__notnull[0];
    current->next__nullable = NULL;
    Char* first = current;
    while (1) {
        ++str__notnull;
        if (*str__notnull == '\0') break;
        Char* new = die_if_null__notnull(malloc(sizeof(Char)));
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
SoftLine SoftLine_make(Char* char__notnull, uint_notnull width_limit) {
    uint index_in_hard_line = 0;
    Char_loop_prev(char__notnull, prev__notnull) {
        if (prev__notnull->content == '\n') break;
        ++index_in_hard_line;
    }
    uint width_index = index_in_hard_line % width_limit;
    Char* beginning_char__notnull = char__notnull;
    repeat(width_index) {
        beginning_char__notnull = die_if_null__notnull(beginning_char__notnull->prev__nullable);
    }
    return (SoftLine) {
        .width_limit__RO = width_limit,
        .beginning_char__RO_notnull = beginning_char__notnull,
    };
}
OptionalSoftLine SoftLine_try_get_next(SoftLine* soft_line__notnull) {
    Char* beginning__not_null = soft_line__notnull->beginning_char__RO_notnull;
    bool exists = true;
    repeat(soft_line__notnull->width_limit__RO) {
        if (beginning__not_null->next__nullable == NULL) {
            exists = false;
            break;
        } else {
            bool newline_found = beginning__not_null->content == '\n';
            beginning__not_null = beginning__not_null->next__nullable;
            if (newline_found) {
                break;
            }
        }
    }
    return (OptionalSoftLine) {
        .exists__RO = exists,
        .soft_line__RO = (SoftLine) {
            .beginning_char__RO_notnull = beginning__not_null,
            .width_limit__RO = soft_line__notnull->width_limit__RO,
        },
    };
}
OptionalSoftLine SoftLine_try_get_prev(SoftLine* soft_line__notnull) {
    if (soft_line__notnull->beginning_char__RO_notnull->prev__nullable == NULL) {
        return (OptionalSoftLine) {
            .exists__RO = false,
        };
    } else {
        return (OptionalSoftLine) {
            .exists__RO = true,
            .soft_line__RO = SoftLine_make(soft_line__notnull->beginning_char__RO_notnull->prev__nullable, soft_line__notnull->width_limit__RO),
        };
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
void render(RenderData* render_data__notnull, WidthLimit width_limit, HeightLimit height_limit) {
    HeightIndex height_index = 0;
    WidthIndex width_index = 0;
    Char_loop_next(render_data__notnull->beginning_line__RO.beginning_char__RO_notnull, next__notnull) {
        if (height_index < height_limit) {
            break;
        }
        bool reset = false;
        if (next__notnull->content == '\n') {
            printf(" ");
            reset = true;
        }
        printf("%c", next__notnull->content);
        if (width_index == width_limit) {
            printf("\n");
            reset = true;
        }
        ++width_index;
        if (reset) {
            width_index = 0;
            ++height_index;
        }
    }
}
int main(void) {
    Char* selected_char__notnull = str_to_Char("Sa\n\n\nb\n\ncdefghi\n\n\n\nE");
    printf("Input:\n");
    Char_loop_next(selected_char__notnull, next__notnull) {
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
        Char_loop_next(render_data.beginning_line__RO.beginning_char__RO_notnull, next__notnull) {
            printf("%c", next__notnull->content);
        }
        printf("\n");
        selected_char__nullable = selected_char__notnull->next__nullable;
    }
    // render(&render_data, width_limit, height_limit);
}
