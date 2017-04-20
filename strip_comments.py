def in_marker(i, text, marker):
    k = i
    for ch in marker:
        if text[k] != ch:
            return False
        k += 1
    i += len(marker) - 1
    return True

def in_markers(i, text, markers):
    for marker in markers:
        if in_marker(i, text, marker):
            return True
    return False

def solution(text_, markers):
    text = list(text_)
    spaces = ' \t'
    i = 0
    while i < len(text):
        j = i

        while i < len(text) and text[i] in spaces:
            i += 1

        if in_markers(i, text, markers):
            while i < len(text) and text[i] != '\n':
                i += 1
            del text[j:i]
            i = j

        i += 1
    return ''.join(text)

assert solution("apples, pears # and bananas\ngrapes\nbananas !apples", ["#", "!"]) == "apples, pears\ngrapes\nbananas")
assert (solution("a #b\nc\nd $e f g", ["#", "$"]) == "a\nc\nd")
assert solution("apples, pears    \n", ["#", "!"]) == "apples, pears\n")
