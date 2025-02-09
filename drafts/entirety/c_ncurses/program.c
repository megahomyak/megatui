#include <ncurses.h>
#include <stdlib.h>

typedef enum {
    STATIC,
    BUTTON,
    EDITABLE,
} CellType;

typedef unsigned long int Index;

typedef struct Cell {
    char content;
    CellType type;
    struct Cell *next;
    struct Cell *prev;
    Index index;
} Cell;

typedef struct {
    WINDOW *win;
    Cell *current_cell;
} Context;

void init_static(Cell *current, char content, Cell *next, Cell *prev) {
    current->next = next;
    current->prev = prev;
    current->type = STATIC;
    current->content = content;
}

void init_button(Cell *current, char content, Cell *next, Cell *prev, Index index) {
    current->next = next;
    current->prev = prev;
    current->type = BUTTON;
    current->content = content;
    current->index = index;
}

Cell *malloc_cell(void) {
    Cell *cell = malloc(sizeof (*cell));
    if (cell == NULL) {
        perror("No memory left");
        exit(1);
    }
    return cell;
}

void free_cell(Cell *cell) {
    free(cell);
}

void add_editable(Context *ctx, char content, Cell *next, Cell *prev) {
    Cell *new_cell = malloc_cell();
    new_cell->prev = ctx->current_cell->prev;
    new_cell->next = ctx->current_cell;
    new_cell->content = content;
    new_cell->type = EDITABLE;
    ctx->current_cell->prev = new_cell;
}

void redraw(Context *ctx) {
    int width = getmaxx(ctx->win);
    int height = getmaxy(ctx->win);
    wclear(ctx->win);
}

int main(void) {
    Context ctx = {
        .win = initscr(),
    };
    nonl();
    cbreak();
    echo();

    endwin();
}
