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

void link_before(Context *ctx, Cell *new_cell) {
    new_cell->prev = ctx->current_cell->prev;
    new_cell->next = ctx->current_cell;
    if (new_cell->prev != NULL) {
        new_cell->prev->next = new_cell;
    }
    ctx->current_cell->prev = new_cell;
}

void link_after(Context *ctx, Cell *new_cell) {
    new_cell->prev = ctx->current_cell;
    new_cell->next = ctx->current_cell->next;
    if (new_cell->next != NULL) {
        new_cell->next->prev = new_cell;
    }
    ctx->current_cell->next = new_cell;
    ctx->current_cell = new_cell;
}

void add_editable_before(Context *ctx, char content) {
    Cell *new_cell = malloc_cell();
    new_cell->type = EDITABLE;
    new_cell->content = content;

    link_before(ctx, new_cell);
}

void add_editable_after(Context *ctx, char content) {
    Cell *new_cell = malloc_cell();
    new_cell->type = EDITABLE;
    new_cell->content = content;

    link_after(ctx, new_cell);
}

void add_static(Context *ctx, char content) {
    Cell *new_cell = malloc_cell();
    new_cell->type = STATIC;
    new_cell->content = content;

    link_after(ctx, new_cell);
}

void add_button(Context *ctx, char content, Index index) {
    Cell *new_cell = malloc_cell();
    new_cell->type = BUTTON;
    new_cell->content = content;
    new_cell->index = index;

    link_after(ctx, new_cell);
}

void redraw(Context *ctx) {
    int width = getmaxx(ctx->win);
    int height = getmaxy(ctx->win);
    wclear(ctx->win);
}

#define iter_str(str) for (char *c = str; *c; ++c)

int main(void) {
    Context ctx = {
        .win = initscr(),
        .current_cell = malloc_cell(),
    };
    ctx.current_cell->prev = NULL;
    ctx.current_cell->next = NULL;
    ctx.current_cell->type = STATIC;
    ctx.current_cell->content = 'T';
    Cell *first_cell = ctx.current_cell;
    iter_str("est static text ") {
        add_static(&ctx, *c);
    }
    iter_str("editable! ") {
        add_editable_after(&ctx, *c);
    }
    iter_str("button") {
        add_button(&ctx, *c, 1);
    }
    nonl();
    cbreak();
    echo();

    {
        Cell *current_cell = first_cell;
        do {
            waddch(ctx.win, current_cell->content);
            current_cell = current_cell->next;
        } while (current_cell);
    }

    wgetch(ctx.win);

    endwin();
}
