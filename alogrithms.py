# число - степень двойки.
i & (i - 1) == 0


# наибольший общий делитель.
def gcd(a, b):
    while a:
        a, b = b % a, a
    return b
