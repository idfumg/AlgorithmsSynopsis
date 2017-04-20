def reverse_number(n):
    rev = 0
    while n > 0:
        rev = rev * 10 + n % 10
        n //= 10
    return rev

def number_digits_count(n):
    length = 0
    while n > 0:
        n //= 10
        length += 1
    return length

def number_digits_count2(n):
    import math
    return math.floor(math.log(n, 10) + 1)

def number_bits_count(n):
    import math
    return math.floor(math.log(n, 2) + 1)
    
def rotate_number(n):
    import math
    length = math.floor(math.log(n, 10) + 1)
    return n % 10 * pow(10, length - 1) + n // 10

def is_whole_number(n):
    return n % 1 == 0

def is_power_of_2(n):
    return n and not (n & (n - 1))

def next_power_of_2(n):
    import math
    if n and not (n & (n - 1)):
        return n
    return 2**(math.floor(math.log(n, 2) + 1))

assert reverse_number(12345) == 54321
assert number_digits_count(12345) == 5
assert number_digits_count2(12345) == 5
assert number_bits_count(3) == 2
assert rotate_number(1234) == 4123
assert is_whole_number(1) == True
assert is_whole_number(1.0) == True
assert is_whole_number(1.1) == False
assert is_power_of_2(2) == True
assert is_power_of_2(8) == True
assert is_power_of_2(9) == False
assert next_power_of_2(3) == 4
assert next_power_of_2(4) == 4
assert next_power_of_2(9) == 16
