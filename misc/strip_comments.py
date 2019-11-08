import time

def solution(text, markers):
    text = list(text)
    spaces = ' \t'
    i = 0
    start = time.time()
    while i < len(text):
        if text[i] in markers:
            j = i
            while i < len(text) and text[i] != '\n':
                i += 1
            del text[j:i]
            i = j

        j = i
        while i < len(text) and text[i] in spaces:
            i += 1

        if (i < len(text) and text[i] in markers) or i == len(text):
            del text[j:i]
            i = j
        else:
            i += 1
            
    end = time.time()
    print('time = "{}"'.format(end - start))
    #print('text = "{}"'.format(''.join(text)))
    return ''.join(text)

assert solution("apples, pears  \t", ["#", "!"]) == "apples, pears"
assert solution("apples, pears # and bananas\ngrapes\nbananas !apples", ["#", "!"]) == "apples, pears\ngrapes\nbananas"
assert solution("a #b\nc\nd $e f g", ["#", "$"]) == "a\nc\nd"
