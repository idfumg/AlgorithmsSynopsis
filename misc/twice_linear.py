def dbl_linear(n):
    u = [1]
    i = 0
    while n > len(u):
        y = 2 * u[i] + 1
        z = 3 * u[i] + 1
        print(y, z)
        u.append(y)
        u.append(z)
        u.sort()
        i += 1
    u.sort()
    print(u)
    print(u[n])
    return u[n-1]

assert dbl_linear(10) == 22
assert dbl_linear(20) == 57
assert dbl_linear(30) == 91
