import curses
from types import SimpleNamespace as SN

STATIC = object()
BUTTON = object()
EDITABLE = object()

current_cell = SN(
    type=STATIC,
    char="T",
    prev=None,
    next=None,
    index=None,
)
first_cell = current_cell
def push_static(text):
    global current_cell
    for char in text:
        current_cell = SN(
            type=STATIC,
            char=char,
            prev=current_cell,
            next=current_cell.next,
            index=None,
        )
def push_editable(text):
    global current_cell
    for char in text:
        current_cell = SN(
            type=EDITABLE,
            char=char,
            prev=current_cell,
            next=current_cell.next,
            index=None,
        )
def push_button(text, index):
    global current_cell
    for char in text:
        current_cell = SN(
            type=BUTTON,
            char=char,
            prev=current_cell,
            next=current_cell.next,
            index=index,
        )
push_static("est static text ")
push_editable("editable! ")
push_button("button", 1)

def main(stdscr):
    stdscr.keypad(True)  # Enable special key input (like arrow keys)
    curses.noecho()  # Disable automatic echoing of typed characters
    curses.cbreak()  # Disable line buffering, so input is processed immediately

    height, width = stdscr.getmaxyx()
    stdscr.addstr("#" * (width + 1))
    stdscr.getch()
    return

    def redraw():
        height, width = stdscr.getmaxyx()
        line_pos = 0
        current_cell_local = current_cell
        while True:
            if current_cell.prev is None:
                break
            line_pos += 1
            if current_cell.prev.char == "\n":
                break
            current_cell = current_cell.prev
        x = line_pos 

    # Display text on the screen
    for i, line in enumerate(text):
        stdscr.addstr(i, 0, line)  # Print each line of text at the correct row

    while True:
        stdscr.move(y, x)
        stdscr.refresh()

        ch = stdscr.getch()
        if ch == ord('q'):  # Read user input until 'q' is pressed
            break
        elif ch == curses.KEY_UP:
            if y > 0:
                y -= 1  # Move cursor up if not at the top
        elif ch == curses.KEY_DOWN:
            if y < len(text) - 1:
                y += 1  # Move cursor down if not at the bottom
        elif ch == curses.KEY_LEFT:
            x = min(len(text[y]) - 1, x)
            if x > 0:
                x -= 1  # Move cursor left if not at the beginning of the line
        elif ch == curses.KEY_RIGHT:
            x = min(len(text[y]) - 1, x)
            if x < len(text[y]) - 1:
                x += 1  # Move cursor right if not at the end of the line

        stdscr.move(y, min(len(text[y]) - 1, x))
        
        stdscr.refresh()  # Refresh screen to reflect changes

curses.wrapper(main)
