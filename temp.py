from pprint import pprint

PLUS, MINUS, MUL, DIV, MOD, POWER, EOF, LEFT, RIGHT, LPAREN, \
RPAREN, INTEGER, OPERATOR, OPERAND, VAR, ASSIGN, SEMI, \
LBRACE, RBRACE = \
('PLUS', 'MINUS', 'MUL', 'DIV', 'MOD', 'POWER', 'EOF', 'LEFT', 'RIGHT', 'LPAREN', \
 'RPAREN', 'INTEGER', 'OPERATOR', 'OPERAND', 'VAR', 'ASSIGN', 'SEMI', \
 'LBRACE', 'RBRACE')

def token(**kwargs):
    return dict(**kwargs)
    
OPTABLE = {
    '^': token(type=OPERATOR, subtype=POWER,  prec=4, assoc=RIGHT),
    '*': token(type=OPERATOR, subtype=MUL,    prec=3, assoc=LEFT),
    '/': token(type=OPERATOR, subtype=DIV,    prec=3, assoc=LEFT),
    '%': token(type=OPERATOR, subtype=MOD,    prec=3, assoc=LEFT),
    '+': token(type=OPERATOR, subtype=PLUS,   prec=2, assoc=LEFT),
    '-': token(type=OPERATOR, subtype=MINUS,  prec=2, assoc=LEFT),
    '=': token(type=OPERATOR, subtype=ASSIGN, prec=1, assoc=RIGHT),
    '(': token(type=LPAREN,   subtype=LPAREN, prec=0, assoc=LEFT),
    ')': token(type=RPAREN,   subtype=RPAREN, prec=0, assoc=LEFT)
}

class LexerException(Exception):
    pass

class Lexer:
    def __init__(self, expr):
        self.expr = expr
        self.pos = 0

        if not isinstance(expr, str):
            raise LexerException('Wrong expression')

    def consume_spaces(self):
        while self.pos < len(self.expr) and self.expr[self.pos].isspace():
            self.pos += 1

    def consume_integer(self):
        start_pos = self.pos
        while self.pos < len(self.expr) and self.expr[self.pos].isdigit():
            self.pos += 1
        return float(self.expr[start_pos:self.pos])

    def consume_var(self):
        start_pos = self.pos
        while self.pos < len(self.expr) and \
              (self.expr[self.pos].isalpha() or self.expr[self.pos].isdigit()):
            self.pos += 1
        return self.expr[start_pos:self.pos]

    def peek(self):
        if self.pos < len(self.expr):
            return self.expr[self.pos]
        return ''

    def next_token(self):
        while True:
            if not self.pos < len(self.expr):
                return token(type=EOF)

            ch = self.expr[self.pos]

            if ch.isspace():
                self.consume_spaces()
            elif ch.isdigit():
                return token(type=OPERAND,
                             subtype=INTEGER,
                             value=self.consume_integer())
            elif ch in OPTABLE:
                self.pos += 1
                return OPTABLE[ch]
            elif ch.isalpha():
                return token(type=VAR, value=self.consume_var())
            elif ch == ';':
                self.pos += 1
                return token(type=SEMI)
            elif ch == '{':
                self.pos += 1
                return token(type=LBRACE)
            elif ch == '}':
                self.pos += 1
                return token(type=RBRACE)
            else:
                raise LexerException('Unexpected character: "' + ch + '"')

    def tokens(self):
        token = self.next_token()
        while token['type'] != EOF:
            yield token
            token = self.next_token()

def node(token):
    if token['type'] == OPERAND:
        return dict(type=token['subtype'], value=token['value'])
    elif token['type'] == OPERATOR:
        return dict(type=token['subtype'], left=None, right=None)
    elif token['type'] == VAR:
        return dict(type=token['type'], value=token['value'])
    else:
        raise Exception('Wrong token:', token)

def build_ast(postfix):
    root = node(postfix[-1])
    current = root

    for elem in reversed(postfix[0:-1]):
        n = node(elem)

        while not 'left' in current or (current['left'] and current['right']):
            child = current
            current = current['parent']
            if 'parent' in child:
                del child['parent']
        
        n['parent'] = current
        
        if not current['right']:
            current['right'] = n
        else:
            current['left'] = n
        
        current = n

    if 'parent' in current:
        del current['parent']
    return root

class InterpreterException(Exception):
    pass

class Interpreter:
    def __init__(self):
        self.variables = [{}]
        self.lexer = None
        self.token = None

    def build_statement(self):
        results = []
        ops = []

        while self.token['type'] not in (SEMI, EOF, LBRACE, RBRACE):
            if self.token['type'] in (OPERAND, VAR):
                results.append(self.token)
            elif self.token['type'] == LPAREN:
                ops.append(self.token)
            elif self.token['type'] == RPAREN:
                op = ops.pop()
                while op['type'] != LPAREN:
                    results.append(op)
                    op = ops.pop()
            else:
                while (ops and self.token['type'] == OPERATOR) and \
                      ((self.token['assoc'] == LEFT and \
                        self.token['prec'] <= ops[-1]['prec']) or \
                       (self.token['assoc'] == RIGHT and \
                        self.token['prec'] < ops[-1]['prec'])):
                    results.append(ops.pop())
                ops.append(self.token)
            
            self.token = self.lexer.next_token()

        while ops:
            results.append(ops.pop())

        return results

    def build_statements(self):
        statements = []
        postfix = None

        while True:
            if self.token['type'] == SEMI:
                self.token = self.lexer.next_token()
            elif self.token['type'] == LBRACE:
                self.token = self.lexer.next_token()
                statements.append(self.build_statements())
            elif self.token['type'] == RBRACE:
                self.token = self.lexer.next_token()
                return statements
            elif self.token['type'] == EOF:
                return statements
            else:
                postfix = self.build_statement()
                if postfix:
                    try:
                        statements.append(build_ast(postfix))
                    except KeyError as e:
                        return None

        return statements

    def build_program(self):
        return {
            'type': 'program',
            'statements': self.build_statements()
        }

    def execute(self, expr):
        if expr == '':
            return ''
        
        self.lexer = Lexer(expr)
        self.token = self.lexer.next_token()
        
        program = self.build_program()
        if self.token['type'] != EOF or not program['statements']:
            raise InterpreterException('Wrong expression: "' + expr + '"')

        rv = ''
        for statement in program['statements']:
            rv = self.traverse(statement)
        return rv

    def find_variables(self, name):
        for variables in reversed(self.variables):
            if name in variables:
                return variables
        return None

    def traverse(self, node):
        if type(node) == list:
            rv = ''
            self.variables.append({})
            for statement in node:
                rv = self.traverse(statement)
            del self.variables[-1]
            return rv
        
        elif node['type'] == VAR:
            vs = self.find_variables(node['value'])
            if vs:
                return vs[node['value']]

            raise InterpreterException(
                'No variable "{}" was found'.format(node['value']))
        
        elif node['type'] == INTEGER:
            return node['value']

        elif node['type'] == ASSIGN:
            name = node['left']['value']
            vs = self.find_variables(name)
            if vs:
                vs[name] = self.traverse(node['right'])
                return vs[name]
            else:
                self.variables[-1][name] = self.traverse(node['right'])
                return self.variables[-1][name]

        elif node['type'] == PLUS:
            return self.traverse(node['left']) + self.traverse(node['right'])
        elif node['type'] == MINUS:
            return self.traverse(node['left']) - self.traverse(node['right'])
        elif node['type'] == MUL:
            return self.traverse(node['left']) * self.traverse(node['right'])
        elif node['type'] == DIV:
            return self.traverse(node['left']) / self.traverse(node['right'])
        elif node['type'] == MOD:
            return self.traverse(node['left']) % self.traverse(node['right'])
        elif node['type'] == POWER:
            return self.traverse(node['left']) ** self.traverse(node['right'])

        else:
            raise InterpreterException('Wrong AST node:', node)
    
def main():
    interpreter = Interpreter()

    try:
        assert interpreter.execute('a = x + 1')
    except InterpreterException as e:
        pass # x not found
    
    assert interpreter.execute('1+2') == 3
    assert interpreter.execute('1+2*3') == 7
    assert interpreter.execute('1+2*3^2') == 19
    assert interpreter.execute('3+4*2/(5-1)^2^3') == 3.0001220703125
    assert interpreter.execute(
'''
a = 1 + 2 * 3;
b = 3;
c = a - b;
''') == 4
    assert interpreter.execute(
'''
{
a = 1 + 2;
}
''') == 3
    assert interpreter.execute(
'''
{
a = 1 + 2 * 3;
b = 3;
c = a - b;
    {
        d = c - 1;
    }
}
''') == 3

    assert interpreter.execute(
'''
{1; 2+1;}
''') == 3

    assert interpreter.execute(
'''
{
    { 1; 2+1; }
    {
        a = 1; a = 1 + 2 * 3;
        a;
        {
            b = 3;
            {
                c = a - b;
                {
                    d = c - 1;
                }
            }
        }
    }
}
''') == 3

    assert interpreter.execute(
'''
a = 2;
1;
{
    {b = 1;}
    
}
a;
''') == 2

    assert interpreter.execute(
'''
a;
''') == 2

    assert interpreter.execute(
'''
b = 1;
{
    {
        b = 2;
    }
}
''') == 2

    assert interpreter.execute(
'''
b;
''') == 2

    try:
        assert interpreter.execute(
'''
b = 1;
{
    {
        b = 2 + q;
    }
}
''') == 2
    except InterpreterException as e:
        pass # no q found

    try:
        assert interpreter.execute('a a') == ''
    except InterpreterException as e:
        pass # wrog expression
    
    assert interpreter.execute('') == ''

    #assert interpreter.execute('def inc(x) { x + 1 }') == ''

if __name__ == '__main__':
    main()
