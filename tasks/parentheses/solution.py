#!/usr/bin/env python2
"""
Task 2.(python)
Write a program to check unbalanced parentheses, brackets and braces in given string.
"""

OPEN = {"(", "[", "{"}

OPEN_TO_CLOSE = {
    "(": ")",
    "[": "]",
    "{": "}",
}

CLOSE = {
    OPEN_TO_CLOSE[ch]
    for ch in OPEN
}


def is_open(ch):
    return ch in OPEN


def open_to_close(ch):
    return OPEN_TO_CLOSE.get(ch)


def is_close(ch):
    return ch in CLOSE


def check(text):
    check_stack = list()
    for position, ch in enumerate(text):
        if is_open(ch):
            check_stack.append(
                open_to_close(ch)
            )
        elif is_close(ch):
            if not check_stack:
                raise RuntimeError("Unexpected {} at position {}".format(ch, position))
            last = check_stack.pop()
            if last != ch:
                raise RuntimeError("Expected {} at position {}".format(ch, position))


def run():
    check("")
    check("()")
    check("[]{}()")
    check("[{}()]")
    check("[(({}()))]")
    check("[(({([])}))]")
    check("[]][")


if __name__ == "__main__":
    run()
