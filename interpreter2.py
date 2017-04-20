import operator

FUNCTION_CALL, FUNCTION_OPERATOR, FUNCTION, EMPTY, ID, ASSIGNMENT, LPAREN, RPAREN, MOD, MUL, DIV, INTEGER, PLUS, MINUS, EOF = ('FUNCTION_CALL', 'FUNCTION_OPERATOR', 'FUNCTION', 'EMPTY', 'ID', 'ASSIGNMENT', 'LPAREN', 'RPAREN', 'MOD', 'MUL', 'DIV', 'INTEGER', 'PLUS', 'MINUS', 'EOF')

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

KEYWORDS = {
    'fn': FUNCTION
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

            elif ch == '=' and self.peek() == '>':
                self.position += 2
                return Token(FUNCTION_OPERATOR, '=>')

            elif ch in SYMBOLS:
                token = Token(SYMBOLS[ch], ch)
                self.position += 1
                return token

            elif ch.isalpha():
                word = self.consume_variable()
                if word in KEYWORDS:
                    return Token(FUNCTION, word)
                else:
                    return Token(ID, word)

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
    def __init__(self, name, arg_names, expr):
        self.name = name
        self.arg_names = arg_names
        self.expr = expr

class FunctionCall(AST):
    def __init__(self, name, exprs):
        self.name = name
        self.exprs = exprs

class ParserException(Exception):
    pass

class Parser:
    def __init__(self, variables, functions):
        self.variables = variables
        self.functions = functions
    
    def parse_ast(self, expression):
        self.lexer = Lexer(expression)
        self.current_token = self.lexer.next_token()
        return self.parse_()

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
        if self.current_token.type == ID and \
           self.current_token.value in self.functions:
            return self.function_call()
        else:
            return self.factor()

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
            return self.term_precedence4()

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

    def function(self):
        # function identificator
        self.consume(FUNCTION)
        if self.current_token.type != ID:
            raise ParserException('Expected function name!')

        # function name
        name = self.current_token.value
        self.consume(ID)

        # function arguments
        arg_names = []
        while self.current_token.type == ID:
            if self.current_token.value in arg_names:
                raise InterpreterException(
                    "Error! Variable '{}' redefinition".format(
                        self.current_token.value))
            arg_names.append(self.current_token.value)
            self.consume(ID)

        # function operator
        if self.current_token.type != FUNCTION_OPERATOR:
            raise ParserException('Expected function operator "=>"!: ',
                                  self.current_token)
        self.consume(FUNCTION_OPERATOR)
        
        return Function(name=name, arg_names=arg_names, expr=self.expression())

    def program(self):
        '''
        function: fn-keyword fn-name { identifier } fn-operator expression
        fn-name: identifier
        fn-operator: '=>'
        fn-keyword: 'fn'
        
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
        if self.current_token.type == FUNCTION:
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

class Interpreter:
    def __init__(self):
        self.variables = {}
        self.local_variables = None
        self.functions = {}
        
    def eval(self, expression):
        self.local_variables = None
        self.current_function = None
        parser = Parser(self.variables, self.functions)
        ast = parser.parse_ast(expression)
        result = self.traverse(ast)
        return result

    def traverse_function(self, arg_names, fn_variables, node):
        if type(node) == Variable:
            if node.value not in arg_names and \
               node.value not in self.variables and \
               node.value not in fn_variables:
                raise InterpreterException(
                    "ERROR: Invalid identifier '{}'"
                    " in function body".format(node.value))
        elif type(node) == Assignment:
            variables.insert(node.left.value)
        
        if hasattr(node, 'left'):
            self.traverse_function(arg_names, fn_variables, node.left)
        if hasattr(node, 'right'):
            self.traverse_function(arg_names, fn_variables, node.right)
        if hasattr(node, 'expr'):
            self.traverse_function(arg_names, fn_variables, node.expr)

    def traverse(self, node):
        if type(node) == Empty:
            return ''
        
        elif type(node) == Integer:
            return node.value

        elif type(node) == Variable:
            if self.local_variables and node.value in self.local_variables:
                return self.local_variables[node.value]
            elif node.value in self.variables:
                return self.variables[node.value]
            
            raise InterpreterException(
                "ERROR: Invalid identifier."\
                " No variable '{}' was found.".format(node.value))

        elif type(node) == Assignment:
            if not self.local_variables:
                self.variables[node.left.value] = self.traverse(node.right)
                return self.variables[node.left.value]
            else:
                self.local_variables[node.left.value] = self.traverse(node.right)
                return self.local_variables[node.left.value]

        elif type(node) == BinaryOp:
            return OPERATORS[node.token.type](self.traverse(node.left),
                                              self.traverse(node.right))

        elif type(node) == Function:
            if node.name in self.variables or \
               self.local_variables and node.name in self.local_variables:
                raise InterpreterException(
                    'Function name conflicts with var:', node.name)
            self.functions[node.name] = [node.arg_names, node.expr]
            self.traverse_function(node.arg_names, set(), node.expr)
            return ''

        elif type(node) == FunctionCall:
            # traverse all arguments
            arg_values = [self.traverse(expr) for expr in node.exprs]
            self.local_variables = {}

            # set all local variables
            arg_names = self.functions[node.name][0]
            for name, value in zip(arg_names, arg_values):
                self.local_variables[name] = value

            if len(arg_values) != len(arg_names):
                raise InterpreterException(
                    'Wrong arguments list')
            
            result = self.traverse(self.functions[node.name][1])
            
            self.local_variables = None
            return result
        
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
    assert interpreter.eval('fn inc x => x + 1') == ''
    assert interpreter.eval('inc 1') == 2
    assert interpreter.eval('inc 2+1') == 4
    assert interpreter.eval('inc (2+1)*2') == 7
    assert interpreter.eval('a = 333') == 333
    assert interpreter.eval('inc a') == 334
    assert interpreter.eval('fn inc x => x + 10') == ''
    assert interpreter.eval('inc 1') == 11
    try:
        assert interpreter.eval('fn inc x => x + z') == ''
        assert False
    except InterpreterException as e:
        pass # ok no z in function body
    assert interpreter.eval('fn sum x y => x + y') == ''
    assert interpreter.eval('sum 13 11') == 24
    try:
        assert interpreter.eval('fn sum => x + y') == ''
    except InterpreterException as e:
        pass # unknown 'x' identifier
    assert interpreter.eval('fn avg x y => (x + y) / 2') == ''
    assert interpreter.eval('avg 4 4') == 4
    assert interpreter.eval('fn echo x => x') == ''
    assert interpreter.eval('fn add x y => x + y') == ''
    assert interpreter.eval('add echo 4 echo 3') == 7
    assert interpreter.eval('x = 0') == 0
    try:
        assert interpreter.eval('fn x => 0') == ''
    except InterpreterException as e:
        pass # conflicts with x var
    assert interpreter.eval('fn avg x y => (x + y) / 2') == ''
    try:
        assert interpreter.eval('avg 7 2 4') == ''
    except InterpreterException as e:
        pass # wrong argument list

if __name__ == '__main__':
    main()
