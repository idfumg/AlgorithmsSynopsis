import operator

EMPTY, ID, ASSIGNMENT, LPAREN, RPAREN, MOD, MUL, DIV, INTEGER, PLUS, MINUS, EOF = ('EMPTY', 'ID', 'ASSIGNMENT', 'LPAREN', 'RPAREN', 'MOD', 'MUL', 'DIV', 'INTEGER', 'PLUS', 'MINUS', 'EOF')

SYMBOLS = {
    '+': PLUS,
    '-': MINUS,
    '*': MUL,
    '/': DIV,
    '%': MOD,
    '(': LPAREN,
    ')': RPAREN,
    '=': ASSIGNMENT
}

OPERATORS = {
    PLUS: operator.add,
    MINUS: operator.sub,
    MUL: operator.mul,
    DIV: operator.truediv,
    MOD: operator.mod
}

class Token:
    def __init__(self, type, value):
        self.type = type
        self.value = value

    def __str__(self):
        return 'Token({}, {})'.format(self.type, self.value)

    def __repr__(self):
        return self.__str__()

class LexerException(Exception):
    pass

class Lexer:
    def __init__(self, expression):
        self.text = expression
        self.position = 0

        if not isinstance(expression, str):
            raise LexerException('Wrong expression!')

    def consume_spaces(self):
        while self.position < len(self.text) and self.text[self.position].isspace():
            self.position += 1

    def consume_integer(self):
        start_position = self.position
        while self.position < len(self.text) and self.text[self.position].isdigit():
            self.position += 1
        return int(self.text[start_position:self.position])

    def consume_variable(self):
        start_position = self.position
        while self.position < len(self.text) and self.text[self.position].isalpha():
            self.position += 1
        return self.text[start_position:self.position]

    def next_token(self):
        while True:
            if not self.position < len(self.text):
                return Token(EOF, None)

            ch = self.text[self.position]

            if ch.isspace():
                self.consume_spaces()

            elif ch.isdigit():
                return Token(INTEGER, self.consume_integer())

            elif ch in SYMBOLS:
                token = Token(SYMBOLS[ch], ch)
                self.position += 1
                return token

            elif ch.isalpha():
                return Token(ID, self.consume_variable())

            else:
                raise LexerException('Unexpected character:', ch)

class AST:
    pass

class Integer(AST):
    def __init__(self, token):
        self.token = token
        self.value = token.value

class Variable(AST):
    def __init__(self, token):
        self.token = token
        self.value = token.value

class Assignment(AST):
    def __init__(self, left, token, right):
        self.left = left
        self.token = token
        self.right = right

class BinaryOp(AST):
    def __init__(self, left, token, right):
        self.left = left
        self.token = token
        self.right = right

class Empty(AST):
    pass

class ParserException(Exception):
    pass

class Parser:
    def parse_ast(self, expression):
        self.lexer = Lexer(expression)
        self.current_token = self.lexer.next_token()
        return self.program()

    def consume(self, token_type):
        if self.current_token.type != token_type:
            raise ParserException('Wrong token type = {}'.format(token_type))
        self.current_token = self.lexer.next_token()

    def factor(self):
        '''
        factor: INTEGER | identifier
        
        assignment: identifier '=' expression
        identifier: letter | '_' { identifier-char }
        identifier-char: '_' | letter | digit
        number: { digit } [ '.' digit { digit } ]
        letter: 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
        digit: '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
        '''
        if self.current_token.type == INTEGER:
            node = Integer(self.current_token)
            self.consume(INTEGER)
            return node
        
        elif self.current_token.type == ID:
            node = Variable(self.current_token)
            self.consume(ID)
            return node

        elif self.current_token.type == EOF:
            node = Empty()
            self.consume(EOF)
            return node

        else:
            raise ParserException('Wrong token:', self.current_token)

    def term_precedence3(self):
        '''
        term_precedence3: factor | "(" expression ")"
        factor: INTEGER | identifier
        
        assignment: identifier '=' expression
        identifier: letter | '_' { identifier-char }
        identifier-char: '_' | letter | digit
        number: { digit } [ '.' digit { digit } ]
        letter: 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
        digit: '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
        '''
        if self.current_token.type == LPAREN:
            self.consume(LPAREN)
            node = self.expression()
            self.consume(RPAREN)
            return node
        else:
            return self.factor()

    def term_precedence2(self):
        '''
        term_precedence2: term_precedence3 ((MUL | DIV) term_precedence3)*
        term_precedence3: factor | "(" expression ")"
        factor: INTEGER | identifier
        
        assignment: identifier '=' expression
        identifier: letter | '_' { identifier-char }
        identifier-char: '_' | letter | digit
        number: { digit } [ '.' digit { digit } ]
        letter: 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
        digit: '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
        '''
        node = self.term_precedence3()
        while self.current_token.type in (MUL, DIV, MOD):
            token = self.current_token
            self.consume(token.type)
            node = BinaryOp(left=node, token=token, right=self.term_precedence3())
        return node

    def term_precedence1(self):
        '''
        term_precedence1: term_precedence2 ((MINUS | PLUS) term_precedence2)*
        term_precedence2: term_precedence3 ((MUL | DIV) term_precedence3)*
        term_precedence3: factor | "(" expression ")"
        factor: INTEGER | identifier
        
        assignment: identifier '=' expression
        identifier: letter | '_' { identifier-char }
        identifier-char: '_' | letter | digit
        number: { digit } [ '.' digit { digit } ]
        letter: 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
        digit: '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
        '''
        node = self.term_precedence2()
        while self.current_token.type in (PLUS, MINUS):
            token = self.current_token
            self.consume(token.type)
            node = BinaryOp(left=node, token=token, right=self.term_precedence2())
        return node

    def term_precedence0(self):
        '''
        term_precedence0: term_precedence1 | assignment
        term_precedence1: term_precedence2 ((MINUS | PLUS) term_precedence2)*
        term_precedence2: term_precedence3 ((MUL | DIV) term_precedence3)*
        term_precedence3: factor | "(" expression ")"
        factor: INTEGER | identifier
        
        assignment: identifier '=' expression
        identifier: letter | '_' { identifier-char }
        identifier-char: '_' | letter | digit
        number: { digit } [ '.' digit { digit } ]
        letter: 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
        digit: '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
        '''
        node = self.term_precedence1()
        if self.current_token.type == ASSIGNMENT:
            token = self.current_token
            self.consume(token.type)
            node = Assignment(left=node, token=token, right=self.term_precedence1())
        return node

    def expression(self):
        '''
        expression: term_precedence0
        term_precedence0: term_precedence1 | assignment
        term_precedence1: term_precedence2 ((MINUS | PLUS) term_precedence2)*
        term_precedence2: term_precedence3 ((MUL | DIV) term_precedence3)*
        term_precedence3: factor | "(" expression ")"
        factor: INTEGER | identifier
        
        assignment: identifier '=' expression
        identifier: letter | '_' { identifier-char }
        identifier-char: '_' | letter | digit
        number: { digit } [ '.' digit { digit } ]
        letter: 'a' | 'b' | ... | 'y' | 'z' | 'A' | 'B' | ... | 'Y' | 'Z'
        digit: '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
        '''
        return self.term_precedence0()

    def program(self):
        ast = self.expression()
        if self.current_token.type != EOF:
            raise InterpreterException('Wrong input:', self.current_token)
        return ast

class InterpreterException(Exception):
    pass

class Interpreter:
    def __init__(self):
        self.parser = Parser()
        self.variables = {}
        
    def eval(self, expression):
        ast = self.parser.parse_ast(expression)
        result = self.traverse(ast)
        return result

    def traverse(self, node):
        if type(node) == Empty:
            return ''
        
        elif type(node) == Integer:
            return node.value

        elif type(node) == Variable:
            if node.value in self.variables:
                return self.variables[node.value]
            
            raise InterpreterException(
                "ERROR: Invalid identifier."\
                " No variable '{}' was found.".format(node.value))

        elif type(node) == Assignment:
            self.variables[node.left.value] = self.traverse(node.right)
            return self.variables[node.left.value]

        elif type(node) == BinaryOp:
            return OPERATORS[node.token.type](self.traverse(node.left),
                                              self.traverse(node.right))
        
        else:
            raise InterpreterException('Wrong node:', node)

def main():
    interpreter = Interpreter()
    assert interpreter.eval('') == ''
    assert interpreter.eval('9') == 9
    assert interpreter.eval('9*9') == 81
    assert interpreter.eval('9*9/3') == 27
    assert interpreter.eval('9*9/3/3') == 9
    assert interpreter.eval('1*2/2 + 9%3  ') == 1
    assert interpreter.eval('9+9') == 18
    assert interpreter.eval('9-9') == 0
    assert interpreter.eval('9-9+1') == 1
    assert interpreter.eval('1 + 2 * 3') == 7
    assert interpreter.eval('1 + 2 * 3 + 1 + 2 - 3') == 7
    assert interpreter.eval('(1 + 2) * 3') == 9
    assert interpreter.eval('a = 1') == 1
    assert interpreter.eval('a') == 1
    try:
        assert interpreter.eval('b') == 2
        assert false
    except InterpreterException as e: # not found
        pass # ok
    assert interpreter.eval('a + 1') == 2
    try:
        assert interpreter.eval('2 2') == 4
    except InterpreterException as e:
        pass # ok

if __name__ == '__main__':
    main()
