def is_valid_braces(text):
    braces = { ')': '(', '}': '{', ']': '[' }
    opened = []
    for ch in text:
        if ch in braces.values():
            opened.append(ch)
        elif opened[-1] == braces[ch]:
            opened.pop()
    return len(opened) == 0

assert is_valid_braces('([{}])') == True
assert is_valid_braces('[(])') == False
assert is_valid_braces('(}') == False
assert is_valid_braces('(){}[]') == True
