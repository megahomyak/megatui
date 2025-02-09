import curses

text = [
    "This is a sample block of text.",
    "You can move the cursor around.",
    "Use arrow keys to navigate.",
    "Press 'q' to quit the program.",
    "Enjoy using curses!"
]

def main(stdscr):
    stdscr.keypad(True)  # Enable special key input (like arrow keys)
    curses.noecho()  # Disable automatic echoing of typed characters
    curses.cbreak()  # Disable line buffering, so input is processed immediately

    x, y = 0, 0
    visual_x = x

    # Display text on the screen
    for i, line in enumerate(text):
        stdscr.addstr(i, 0, line)  # Print each line of text at the correct row

    stdscr.move(y, visual_x)  # Move cursor to the initial position
    stdscr.refresh()  # Refresh screen to reflect changes

    while True:
        ch = stdscr.getch()
        if ch == ord('q'):  # Read user input until 'q' is pressed
            break
        elif ch == curses.KEY_UP:
            if y > 0:
                y -= 1  # Move cursor up if not at the top
                if len(text[y]) - 1 < x:
                    visual_x = len(text[y]) - 1
        elif ch == curses.KEY_DOWN:
            if y < len(text) - 1:
                y += 1  # Move cursor down if not at the bottom
                if len(text[y]) - 1 < planned_x:
                    x = len(text[y]) - 1
        elif ch == curses.KEY_LEFT:
            if x > 0:
                x -= 1  # Move cursor left if not at the beginning of the line
                planned_x = x
        elif ch == curses.KEY_RIGHT:
            if x < len(text[y]) - 1:
                x += 1  # Move cursor right if not at the end of the line
                planned_x = x
        
        stdscr.move(y, visual_x)  # Update cursor position
        stdscr.refresh()  # Refresh screen to reflect changes

curses.wrapper(main)
