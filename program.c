#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ncurses.h>
/* utils */
typedef enum {
    STATIC = 0,
    EDIT = 1,
    BUTTON = 2,
} CHAR_TYPE;
typedef unsigned int uint;
typedef struct char_list {
    struct char_list* prev_nullable;
    char content;
    CHAR_TYPE type;
    uint button_index;
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
char_list* push_char_after_notnull(char_list* current_notnull, char content, CHAR_TYPE type, uint button_index) {
    char_list* new_notnull = malloc_notnull(sizeof(char_list));
    new_notnull->content = content;
    new_notnull->prev_nullable = current_notnull;
    new_notnull->next_nullable = current_notnull->next_nullable;
    if (current_notnull->next_nullable != NULL) current_notnull->next_nullable->prev_nullable = new_notnull;
    new_notnull->button_index = button_index;
    new_notnull->type = type;
    current_notnull->next_nullable = new_notnull;
    return new_notnull;
}
char_list* push_char_before_notnull(char_list* current_notnull, char content, CHAR_TYPE type, uint button_index) {
    char_list* new_notnull = malloc_notnull(sizeof(char_list));
    new_notnull->content = content;
    new_notnull->prev_nullable = current_notnull->prev_nullable;
    if (current_notnull->prev_nullable != NULL) current_notnull->prev_nullable->next_nullable = new_notnull;
    new_notnull->next_nullable = current_notnull;
    new_notnull->button_index = button_index;
    new_notnull->type = type;
    current_notnull->prev_nullable = new_notnull;
    return new_notnull;
}
char_list* extend_char_list_notnull(char_list* current_nullable, char* new_chars_notnull, CHAR_TYPE type, uint button_index) {
    char_list* current_notnull;
    if (current_nullable == NULL) {
        current_notnull = malloc_notnull(sizeof(char_list));
        current_notnull->type = type;
        current_notnull->button_index = button_index;
        if (new_chars_notnull[0] == '\0') {
            fprintf(stderr, "empty str\n");
            exit(EXIT_FAILURE);
        }
        current_notnull->content = *new_chars_notnull;
        ++new_chars_notnull;
        current_notnull->next_nullable = NULL;
        current_notnull->prev_nullable = NULL;
    } else {
        current_notnull = current_nullable;
    }
    for (; *new_chars_notnull != '\0'; ++new_chars_notnull) {
        current_notnull = push_char_after_notnull(current_notnull, *new_chars_notnull, type, button_index);
    }
    return current_notnull;
}
char_list* roll_to_beginning_notnull(char_list* char_list_notnull) {
    while (char_list_notnull->prev_nullable != NULL) char_list_notnull = char_list_notnull->prev_nullable;
    return char_list_notnull;
}
char_list* find_next_edit_sequence_nullable(char_list* char_list_nullable) {
    while (char_list_nullable != NULL) {
        if (char_list_nullable->type == EDIT) return char_list_nullable;
        char_list_nullable = char_list_nullable->next_nullable;
    }
    return NULL;
}
char_list* move_to_next_edit_char_nullable(char_list** char_list_nullable_notnull) {
    char_list* char_list_nullable = *char_list_nullable_notnull;
    if (char_list_nullable != NULL && char_list_nullable->type == EDIT) {
        char_list* next_nullable = char_list_nullable->next_nullable;
        *char_list_nullable_notnull = next_nullable;
        if (next_nullable != NULL && next_nullable->type == EDIT) {
            return char_list_nullable;
        }
    }
    return NULL;
}
uint min(uint a, uint b) {
    return a < b ? a : b;
}
uint subtract_saturating(uint minuend, uint subtrahend) {
    return minuend - min(minuend, subtrahend);
}
/* rendering */
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
    char_list* after_soft_line_notnull = current_soft_line_notnull;
    while (after_soft_lines + current_soft_lines <= height_limit_notzero) {
        char_list* after_soft_line_nullable = get_next_soft_line_nullable(after_soft_line_notnull, width_limit_notzero);
        if (after_soft_line_nullable == NULL) break;
        after_soft_line_notnull = after_soft_line_nullable;
        ++after_soft_lines;
    }
    uint max_before_soft_lines = subtract_saturating(height_limit_notzero, current_soft_lines + min(after_soft_lines, height_limit_notzero / 2));
    uint before_soft_lines = 0;
    char_list* before_soft_line_notnull = current_soft_line_notnull;
    while (before_soft_lines < max_before_soft_lines) {
        char_list* before_soft_line_nullable = get_prev_soft_line_nullable(before_soft_line_notnull, width_limit_notzero);
        if (before_soft_line_nullable == NULL) break;
        before_soft_line_notnull = before_soft_line_nullable;
        ++before_soft_lines;
    }
    return before_soft_line_notnull;
}
uint render_and_return_cursor_x(char_list* beginning_soft_line_notnull, uint width_limit_notzero, uint height_limit_notzero, char_list* current_char_notnull) {
    clear();
    char_list* current_char_nullable = beginning_soft_line_notnull;
    int cursor_x, cursor_y;
    for (uint current_y = 0; current_y < height_limit_notzero; ++current_y) {
        for (uint current_x = 0; current_x < width_limit_notzero; ++current_x) {
            if (current_char_notnull == current_char_nullable) {
                cursor_x = current_x;
                cursor_y = current_y;
            }
            printw("%c", current_char_nullable->content);
            current_char_nullable = current_char_nullable->next_nullable;
            if (current_char_nullable == NULL) goto end;
        }
    }
    end:
    refresh();
    move(cursor_x, cursor_y);
    return cursor_x;
    for (uint current_y = 0;;) {
        char_list* current_char_nullable = beginning_soft_line_nullable;
        beginning_soft_line_nullable = get_next_soft_line_nullable(beginning_soft_line_nullable, width_limit_notzero);
        uint current_x = 0;
        for (; current_char_nullable != beginning_soft_line_nullable; current_char_nullable = current_char_nullable->next_nullable) {
            if (current_char_nullable == current_char_notnull) {
                cursor_x = current_x;
                cursor_y = current_y;
            }
            char content = current_char_nullable->content;
            int color_pair_id = current_char_nullable->type;
            if (color_pair_id != 0) attron(COLOR_PAIR(color_pair_id));
            mvinsch(current_y, current_x, content == '\n' ? ' ' : content);
            if (color_pair_id != 0) attroff(COLOR_PAIR(color_pair_id));
            ++current_x;
        }
        ++current_y;
        if (beginning_soft_line_nullable == NULL || current_y == height_limit_notzero) break;
    }
    mvprintw(0, 0, "%d %d %c %d", cursor_y, cursor_x, current_char_notnull->content, current_char_notnull->content);
    //cursor_x = 12; cursor_y = 15;
    //cursor_x = 13; cursor_y = 15;
    move(cursor_y, cursor_x);
    refresh();
    return cursor_x;
}
/* UI */
char_list* wait_for_button_activation_and_return_the_destination_notnull(char_list* current_char_notnull) {
    initscr();
    //start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    init_pair(EDIT, COLOR_BLACK, COLOR_YELLOW);
    init_pair(BUTTON, COLOR_BLACK, COLOR_BLUE);
    bool cursor_target_x_set = false;
    uint cursor_target_x;
    while (1) {
        uint width_limit_notzero = getmaxx(stdscr);
        uint height_limit_notzero = getmaxy(stdscr);
        char_list* current_soft_line_notnull = get_soft_line_notnull(current_char_notnull, width_limit_notzero);
        uint cursor_x = render_and_return_cursor_x(find_render_beginning_soft_line_notnull(current_soft_line_notnull, width_limit_notzero, height_limit_notzero), width_limit_notzero, height_limit_notzero, current_char_notnull);
        if (!cursor_target_x_set) {
            cursor_target_x = cursor_x;
            cursor_target_x_set = true;
        }
        int key = getch();
        if (key != KEY_RESIZE) {
            if (key == KEY_LEFT) {
                if (current_char_notnull->prev_nullable != NULL) {
                    current_char_notnull = current_char_notnull->prev_nullable;
                    cursor_target_x_set = false;
                }
            } else if (key == KEY_RIGHT) {
                if (current_char_notnull->next_nullable != NULL) {
                    current_char_notnull = current_char_notnull->next_nullable;
                    cursor_target_x_set = false;
                }
            } else if (key == KEY_UP) {
                char_list* prev_soft_line_nullable = get_prev_soft_line_nullable(current_soft_line_notnull, width_limit_notzero);
                if (prev_soft_line_nullable != NULL) {
                    current_char_notnull = prev_soft_line_nullable;
                    for (uint i = 0; i < cursor_target_x; ++i) {
                        if (current_char_notnull->next_nullable == current_soft_line_notnull) break;
                        current_char_notnull = current_char_notnull->next_nullable;
                    }
                }
            } else if (key == KEY_DOWN) {
                char_list* next_soft_line_nullable = get_next_soft_line_nullable(current_soft_line_notnull, width_limit_notzero);
                if (next_soft_line_nullable != NULL) {
                    current_char_notnull = next_soft_line_nullable;
                    char_list* next_next_soft_line_nullable = get_next_soft_line_nullable(next_soft_line_nullable, width_limit_notzero);
                    for (uint i = 0; i < cursor_target_x; ++i) {
                        if (current_char_notnull->next_nullable == next_next_soft_line_nullable) break;
                        current_char_notnull = current_char_notnull->next_nullable;
                    }
                }
            } else {
                if (current_char_notnull->type == BUTTON) {
                    if (key == '\n') break;
                } else if (current_char_notnull->type == EDIT) {
                    if (key == KEY_BACKSPACE) {
                        if (current_char_notnull->prev_nullable != NULL && current_char_notnull->prev_nullable->type == EDIT) {
                            char_list* prev_nullable = current_char_notnull->prev_nullable;
                            if (prev_nullable->prev_nullable != NULL) prev_nullable->prev_nullable->next_nullable = current_char_notnull;
                            current_char_notnull->prev_nullable = prev_nullable->prev_nullable;
                            free(prev_nullable);
                        }
                    } else {
                        push_char_before_notnull(current_char_notnull, key, EDIT, 0);
                    }
                }
            }
        }
    }
    endwin();
    return current_char_notnull;
}
/* usage */
enum {
    EXIT_WITHOUT_EDIT_SEQUENCE_PRINTING,
    EXIT_WITH_EDIT_SEQUENCE_PRINTING,
};
int main(void) {
    char_list* input_notnull = extend_char_list_notnull(NULL, "Move around with arrow keys, enter or erase or divide text with newlines in yellow edit sequences and press buttons with Enter.\n", STATIC, 0);
    input_notnull = extend_char_list_notnull(input_notnull, "Exit without sequence printing", BUTTON, EXIT_WITHOUT_EDIT_SEQUENCE_PRINTING);
    input_notnull = extend_char_list_notnull(input_notnull, " ", STATIC, 0);
    input_notnull = extend_char_list_notnull(input_notnull, "Exit with sequence printing", BUTTON, EXIT_WITH_EDIT_SEQUENCE_PRINTING);
    input_notnull = extend_char_list_notnull(input_notnull, "\n\n", STATIC, 0);
    input_notnull = extend_char_list_notnull(input_notnull, "Edit sequence 1: ", STATIC, 0);
    input_notnull = extend_char_list_notnull(input_notnull, "default value\n", EDIT, 0);
    input_notnull = extend_char_list_notnull(input_notnull, "Edit sequence 2: ", STATIC, 0);
    input_notnull = extend_char_list_notnull(input_notnull, " ", EDIT, 0);
    input_notnull = wait_for_button_activation_and_return_the_destination_notnull(roll_to_beginning_notnull(input_notnull));
    if (input_notnull->button_index == EXIT_WITHOUT_EDIT_SEQUENCE_PRINTING) {
        printf("Exiting without edit sequence printing\n");
    } else if (input_notnull->button_index == EXIT_WITH_EDIT_SEQUENCE_PRINTING) {
        input_notnull = roll_to_beginning_notnull(input_notnull);
        uint edit_sequence_number = 1;
        char_list* edit_sequence_nullable = input_notnull;
        while (1) {
            edit_sequence_nullable = find_next_edit_sequence_nullable(edit_sequence_nullable);
            if (edit_sequence_nullable == NULL) break;
            printf("Edit sequence %d: [", edit_sequence_number);
            ++edit_sequence_number;
            while (1) {
                char_list* edit_char_nullable = move_to_next_edit_char_nullable(&edit_sequence_nullable);
                if (edit_char_nullable == NULL) break;
                printf("%c", edit_char_nullable->content);
            }
            printf("]\n");
        }
    }
}
