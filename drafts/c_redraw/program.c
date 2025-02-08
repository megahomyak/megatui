#include <stdio.h>
#include <termios.h>
#include <unistd.h>

struct termios make_stdin_raw(void) {
    struct termios old_stdin_params;
    tcgetattr(STDIN_FILENO, &old_stdin_params);
    struct termios new_stdin_params = old_stdin_params;
    new_stdin_params.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_stdin_params);
    return old_stdin_params;
}

void set_stdin_params(struct termios *old_stdin_params) {
    tcsetattr(STDIN_FILENO, TCSANOW, old_stdin_params);
}

int main(void) {
    struct termios old_stdin_params = make_stdin_raw();

    printf("%s", "abc\nd");
    getchar();
    printf("%s", "\b \b");
    getchar();
    printf("%s", "\b \b");
    getchar();
    printf("%s", "\b \b");
    getchar();

    set_stdin_params(&old_stdin_params);
}
