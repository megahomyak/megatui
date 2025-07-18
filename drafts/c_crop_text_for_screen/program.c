#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define Char_iter(producer__notnull, receiver__nullable) for (Char* receiver__nullable = producer__notnull; receiver__nullable != NULL; receiver__nullable = receiver__nullable->next__nullable)
#define Char_iter_prev(producer__notnull, receiver__nullable) for (Char* receiver__nullable = producer__notnull->prev__nullable; receiver__nullable != NULL; receiver__nullable = receiver__nullable->prev__nullable)
#define Char_iter_next(producer__notnull, receiver__nullable) for (Char* receiver__nullable = producer__notnull->next__nullable; receiver__nullable != NULL; receiver__nullable = receiver__nullable->next__nullable)
#define repeat(count) for (uint i = 0; i < count; ++i)
#define wrap(old_type, wrapper_type) typedef struct { old_type _content; } wrapper_type; old_type get_##wrapper_type(wrapper_type wrapper) { return wrapper._content; } wrapper_type make_##wrapper_type(old_type content)
#define die(msg) { fprintf(stderr, "death at file %s, line %u: %s", __FILE__, __LINE__, msg); exit(EXIT_FAILURE); }
#define wrap_ptr_notnull(old_type, wrapper_type) wrap(old_type, wrapper_type) { if (content == NULL) die("== NULL") return (wrapper_type) { ._content = content }; }
#define wrap_identity(old_type, wrapper_type) wrap(old_type, wrapper_type) { return (wrapper_type) { ._content = content }; }
#define checked_malloc_type(type) (type*) checked_malloc(sizeof(type))
#define malloc_checked_type(type) make_##type(malloc(sizeof(type)))

typedef unsigned int uint;
wrap(uint, uint_notnull) {
    if (content == 0) die("== 0")
    return (uint_notnull) {
        ._content = content,
    };
}
void* checked_malloc(size_t size) {
    void* result = malloc(size);
    if (result == NULL) die("== NULL")
    return result;
}

typedef struct char_list {
    struct char_list* prev__nullable;
    char content;
    struct char_list* next__nullable;
} char_list;
wrap_ptr_notnull(char_list*, char_list_notnull)
wrap_identity(char_list, soft_line)
wrap_ptr_notnull(soft_line*, soft_line_notnull)

typedef struct {
    char_list* selected_char__RO_notnull;
    soft_line* beginning_line__RO_notnull;
} RenderData;
char_list_notnull str_to_char_list(char* str__notnull) {
    if (str__notnull[0] == '\0') {
        fprintf(stderr, "Empty str in str_to_Char\n");
        exit(1);
    }
    char_list* current = checked_malloc_type(char_list);
    current->prev__nullable = NULL;
    current->content = str__notnull[0];
    current->next__nullable = NULL;
    char_list* first = current;
    while (1) {
        ++str__notnull;
        if (*str__notnull == '\0') break;
        char_list* new = checked_malloc_type(char_list);
        new->prev__nullable = current;
        new->content = *str__notnull;
        new->next__nullable = current->next__nullable;
        current->next__nullable = new;
        current = new;
    }
    return make_char_list_notnull(first);
}
uint min(uint a, uint b) {
    return a < b ? a : b;
}
uint subtract_saturating(uint minuend, uint subtrahend) {
    return minuend - min(minuend, subtrahend);
}
soft_line get_next_soft_line__nullable(soft_list* s__notnull) {

}
soft_line get_soft_line(char_list* c__notnull, uint_notnull width_limit) {
    uint index_in_hard_line = 0;
    Char_iter_prev(char__notnull, prev__notnull) {
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
Char* SoftLine_try_get_next__nullable_private(SoftLine* soft_line__notnull) {
    Char* beginning__not_null = soft_line__notnull->beginning_char__RO_notnull;
    repeat(soft_line__notnull->width_limit__RO) {
        if (beginning__not_null->next__nullable == NULL) {
            return NULL;
        } else {
            bool newline_found = beginning__not_null->content == '\n';
            beginning__not_null = die_if_null__notnull(beginning__not_null->next__nullable);
            if (newline_found) {
                break;
            }
        }
    }
    return beginning__not_null;
}
OptionalSoftLine SoftLine_try_get_next(SoftLine* soft_line__notnull) {
    Char* next__nullable = SoftLine_try_get_next__nullable_private(soft_line__notnull);
    if (next__nullable == NULL) {
        return (OptionalSoftLine) {
            .exists__RO = false,
        };
    } else {
        return (OptionalSoftLine) {
            .exists__RO = true,
            .soft_line__RO = (SoftLine) {
                .width_limit__RO = soft_line__notnull->width_limit__RO,
                .beginning_char__RO_notnull = die_if_null__notnull(next__nullable),
            },
        };
    }
}
OptionalSoftLine SoftLine_try_get_prev(SoftLine* soft_line__notnull) {
    Char* prev__nullable = soft_line__notnull->beginning_char__RO_notnull->prev__nullable;
    if (prev__nullable == NULL) {
        return (OptionalSoftLine) {
            .exists__RO = false,
        };
    } else {
        return (OptionalSoftLine) {
            .exists__RO = true,
            .soft_line__RO = SoftLine_make(die_if_null__notnull(prev__nullable), soft_line__notnull->width_limit__RO),
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
