import operator

FUNCTION_ARGS_START, FUNCTION_ARGS_STOP, EMPTY, ID, ASSIGNMENT, LPAREN, RPAREN, MOD, MUL, DIV, INTEGER, PLUS, MINUS, EOF = ('FUNCTION_ARGS_START', 'FUNCTION_ARGS_STOP', 'EMPTY', 'ID', 'ASSIGNMENT', 'LPAREN', 'RPAREN', 'MOD', 'MUL', 'DIV', 'INTEGER', 'PLUS', 'MINUS', 'EOF')

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

    def peek(self):
        return self.text[self.position+1]

    def next_token(self):
        while True:
            if not self.position < len(self.text):
                return Token(EOF, None)

            ch = self.text[self.position]

            if ch.isspace():
                self.consume_spaces()

            elif ch.isdigit():
                return Token(INTEGER, self.consume_integer())

            elif ch == '[':
                self.position += 1
                return Token(FUNCTION_ARGS_START, '[')

            elif ch == ']':
                self.position += 1
                return Token(FUNCTION_ARGS_STOP, ']')

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

class Function(AST):
    def __init__(self, arg_names, expr):
        self.arg_names = arg_names
        self.expr = expr

class ParserException(Exception):
    pass

class Parser:
    def parse_ast(self, expression):
        self.lexer = Lexer(expression)
        self.current_token = self.lexer.next_token()
        return self.parse_()

    def consume(self, token_type):
        if self.current_token.type != token_type:
            raise ParserException('Wrong token type = {}'.format(token_type))
        self.current_token = self.lexer.next_token()

    def factor(self):
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

    def function_call(self):
        fn_name = self.current_token.value
        self.consume(ID)
        exprs = []
        while self.current_token.type == ID and \
              self.current_token.value in self.functions:
            name = self.current_token.value
            self.consume(ID)
            exprs.append(FunctionCall(name, [self.program()]))
            
        while self.current_token.type != EOF:
            exprs.append(self.program())
            
        node = FunctionCall(name=fn_name, exprs=exprs)
        return node

    def term_precedence4(self):
        return self.factor()

    def term_precedence3(self):
        if self.current_token.type == LPAREN:
            self.consume(LPAREN)
            node = self.expression()
            self.consume(RPAREN)
            return node
        else:
            return self.term_precedence4()

    def term_precedence2(self):
        node = self.term_precedence3()
        while self.current_token.type in (MUL, DIV, MOD):
            token = self.current_token
            self.consume(token.type)
            node = BinaryOp(left=node, token=token, right=self.term_precedence3())
        return node

    def term_precedence1(self):
        node = self.term_precedence2()
        while self.current_token.type in (PLUS, MINUS):
            token = self.current_token
            self.consume(token.type)
            node = BinaryOp(left=node, token=token, right=self.term_precedence2())
        return node

    def term_precedence0(self):
        node = self.term_precedence1()
        if self.current_token.type == ASSIGNMENT:
            token = self.current_token
            self.consume(token.type)
            node = Assignment(left=node, token=token, right=self.term_precedence1())
        return node

    def expression(self):
        return self.term_precedence0()

    def function(self):
        self.consume(FUNCTION_ARGS_START)

        arg_names = []
        while self.current_token.type == ID:
            if self.current_token.value in arg_names:
                raise InterpreterException(
                    "Error! Variable '{}' redefinition".format(
                        self.current_token.value))
            arg_names.append(self.current_token.value)
            self.consume(ID)

        self.consume(FUNCTION_ARGS_STOP)
        
        return Function(arg_names=arg_names, expr=self.expression())

    def program(self):
        if self.current_token.type == FUNCTION_ARGS_START:
            ast = self.function()
        else:
            ast = self.expression()
        
        return ast

    def parse_(self):
        ast = self.program()
        
        if self.current_token.type != EOF:
            raise InterpreterException('Wrong input:', self.current_token)
        
        return ast

class InterpreterException(Exception):
    pass

class Compiler:
    def __init__(self):
        self.variables = {}
        
    def pass1(self, expression):
        parser = Parser()
        ast = parser.parse_ast(expression)
        result = self.traverse(ast)
        return result

    def traverse(self, node):
        if type(node) == Empty:
            return ''
        
        elif type(node) == Integer:
            return {
                'op': 'imm',
                'n': node.value
            }

        elif type(node) == Variable:
            return {
                'op': 'arg',
                'n': node.value
            }

        elif type(node) == BinaryOp:
            return {
                'op': node.token.value,
                'a': self.traverse(node.left),
                'b': self.traverse(node.right),
            }

        elif type(node) == Function:
            return self.traverse(node.expr)
        
        else:
            raise InterpreterException('Wrong node:', node)

def main():
    compiler = Compiler()
    print(compiler.eval('[ a b ] a + b + 1'))

if __name__ == '__main__':
    main()
