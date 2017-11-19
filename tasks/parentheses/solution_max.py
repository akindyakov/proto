#!/usr/bin/env python2
"""
Task 2
(extention)
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
    max_len = 0
    for position, ch in enumerate(text):
        if is_open(ch):
            check_stack.append(
                open_to_close(ch)
            )
        elif is_close(ch):
            if not check_stack:
                break
            last = check_stack.pop()
            if last != ch:
                break
        if not check_stack:
            max_len = position + 1
    return text[:max_len]


def run():
    assert "" == check("")
    assert "()" == check("()}}")
    assert "[]{}()" == check("[]{}()(")
    assert "[{}()]" == check("[{}()]{{")
    assert "[(({}()))]" == check("[(({}()))]))")
    assert "[(({([])}))]" == check("[(({([])}))]}}{{")
    assert "[]" == check("[]][")
    assert "[()]" == check("[()]}][")


if __name__ == "__main__":
    run()
