from types import SimpleNamespace as SN

def render(cursor, width, height):
    before = [[]]
    after = []
    current_lines = before
    beginning = cursor
    cursor_line = None
    while beginning.prev != None:
        beginning = beginning.prev
    while beginning != None:
        if beginning is cursor:
            cursor_line = current_lines.pop()
            after.append([])
            current_lines = after
        current_lines[-1][-1].append(beginning.value)
        beginning = beginning.next
    print(before)
    print(cursor_line)
    print(after)

def main():
    current = SN(value="1", next=None, prev=None)
    for c in "hello, world!\n\n\n\n\n\n\nCRAPPPPP\n|lalalalalalala\narstarstarstarstarsntaierthdienshtdienhrstidenhrsitden\nrotendoirsetndoiersntdoiernstodei\nrstidenroistendoiernstodeirnstoid\n\n\n\n\narst\narstarst\narstarstarst\n\n\n\n":
        if c == '|':
            cursor = current
        else:
            current.next = SN(prev=current, next=None, value=c)
            current = current.next
    render(cursor, 4, 4)

main()
