#!/bin/sh
gcc program.c -lncurses -g && echo "The PID to connect with: $$ . Press any key" && read _ && exec ./a.out
