#include <stdio.h>

#define termseq(name, value) int name(void) { return printf("%s", "\x1b[" value); }

termseq(alt_screen, "?1049h")
termseq(main_screen, "?1049l")

int main(void) {
    alt_screen();

    main_screen();
}
