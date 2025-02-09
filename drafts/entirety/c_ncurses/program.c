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
    WINDOW /* non-null */ *win;
    Cell /* non-null */ *current_cell;
} Context;

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

void link_before(Cell *current, Cell *new) {
    new->prev = current->prev;
    new->next = current;
    if (new->prev != NULL) {
        new->prev->next = new;
    }
    current->prev = new;
}

void link_after(Cell *current, Cell *new) {
    new->prev = current;
    new->next = current->next;
    if (new->next != NULL) {
        new->next->prev = new;
    }
    current->prev = new;
}

void add_editable(Context *ctx, char content) {
    Cell *new_cell = malloc_cell();
    new_cell->type = EDITABLE;
    new_cell->content = content;

    link_before(ctx->current_cell, new_cell);
}

void add_static(Context *ctx, char content) {
    Cell *new_cell = malloc_cell();
    new_cell->type = STATIC;
    new_cell->content = content;

    link_after(ctx->current_cell, new_cell);
}

void add_button(Context *ctx, char content, Index index) {
    Cell *new_cell = malloc_cell();
    new_cell->type = BUTTON;
    new_cell->content = content;
    new_cell->index = index;

    link_after(ctx->current_cell, new_cell);
}

void redraw(Context *ctx) {
    int width = getmaxx(ctx->win);
    int height = getmaxy(ctx->win);
    wclear(ctx->win);
}

#define iter_str(str) for (char *c = str; *c; ++c)

int main(void) {
    Cell *current_cell = malloc_cell();
    current_cell->prev = NULL;
    current_cell->next = NULL;
    current_cell->type = STATIC;
    current_cell->content = 'T';
    Context ctx = {
        .win = initscr(),
        .current_cell = current_cell,
    };
    iter_str("est static text ") {
        add_static(&ctx, *c);
    }
    iter_str("editable! ") {
        add_editable(&ctx, *c);
    }
    iter_str("button") {
        add_button(&ctx, *c, 1);
    }
    nonl();
    cbreak();
    echo();

    wgetch(ctx.win);

    endwin();
}
