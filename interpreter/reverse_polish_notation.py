'''
Reverse Polish Notation algorithm
'''

def rpn(expr):
    PREC = {
        '^': 4,
        '*': 3,
        '/': 3,
        '+': 2,
        '-': 2,
        '(': 1
    }

    postfix = []
    ops = []

    for token in expr:
        if token.isdigit():
            postfix.append(token)

        elif token == '(':
            ops.append(token)

        elif token == ')':
            op = ops.pop()
            while op != '(':
                postfix.append(op)
                op = ops.pop()

        else:
            while len(ops) > 0 and PREC[ops[-1]] >= PREC[token]:
                postfix.append(ops.pop())
            ops.append(token)

    while len(ops) > 0:
        postfix.append(ops.pop())
    return ''.join(postfix)

def main():
    assert rpn('1+2') == '12+'
    assert rpn('1*2+3*4') == '12*34*+'
    assert rpn('1+(2+1)*3') == '121+3*+'
    assert rpn('5+(6-2)*9+3^(7-1)') == '562-9*+371-^+'

if __name__ == '__main__':
    main()
