from types import SimpleNamespace
import curses

"""
Implement only the basics:
* Sequence of letters
* Each letetr is static, editable or button
"""

def main(stdscr):
    curses.use_default_colors()
    STATIC_COLOR_PAIR = 1
    EDITABLE_COLOR_PAIR = 2
    BUTTON_COLOR_PAIR = 3
    curses.init_pair(STATIC_COLOR_PAIR, curses.COLOR_WHITE, curses.COLOR_BLACK)
    curses.init_pair(EDITABLE_COLOR_PAIR, curses.COLOR_BLACK, curses.COLOR_YELLOW)
    curses.init_pair(BUTTON_COLOR_PAIR, curses.COLOR_WHITE, curses.COLOR_BLUE)

    stdscr.addstr("static ", curses.color_pair(STATIC_COLOR_PAIR))
    stdscr.addstr("editable", curses.color_pair(EDITABLE_COLOR_PAIR))
    stdscr.addstr(" ", curses.color_pair(STATIC_COLOR_PAIR))
    stdscr.addstr("button", curses.color_pair(BUTTON_COLOR_PAIR))
    stdscr.addstr(" longlonglonglonglonglonglonglonglonglonglonglonglonglonglonglonglonglonglonglong", curses.color_pair(STATIC_COLOR_PAIR))
    y = 0
    x = 0
    stdscr.move(y, x)
    while True:
        key = stdscr.getkey()
        if key == curses.KEY_UP:
            y -= 1
            stdscr.move(y, x)
        elif key == curses.KEY_DOWN:
            y += 1
            stdscr.move(y, x)
        elif key == curses.KEY_RIGHT:
            x += 1
            stdscr.move(y, x)
        elif key == curses.KEY_LEFT:
            x -= 1
            stdscr.move(y, x)

curses.wrapper(main)
