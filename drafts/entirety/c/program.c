#include <stdio.h>

#define termseq(name, value) inline void name(void) { printf("%s", "\x1b[" value); }

termseq(alt_screen, "?1049h")
termseq(main_screen, "?1049l")
termseq(clear_screen, "2J")

inline void move_cursor(int x, int y) {
    printf("\x1b[%d;%dH", x, y);
}

int main(void) {
    alt_screen();
    printf("test");
    getchar();
    main_screen();
}
