rv = []
with open('strip_comments.py') as fd:
    for line in fd:
        if 'assert' not in line:
            rv.append(line)
print('\n'.join(rv))
