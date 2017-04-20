def count_change2(money, coins):
    coins = sorted(coins)
    counters = [0 for _ in range(0, len(coins))]
    max_counters = [money // coin for coin in coins]

    result = 0
    max_counters_first = max_counters[0]
    max_counters_last = max_counters[-1]
    coins_last = coins[-1]
    len_counters = len(counters) - 1

    summa = sum(coin * count for coin, count in zip(coins, counters))

    while counters[0] <= max_counters_first:
        
        ########################################################################
        # First part - handle last coin by iterate it up.
        ########################################################################
        
        # Try to adjust count of last coin to the nearest to money if need.
        money_minus_summa = money - summa
        if money_minus_summa >= coins[-1]:
            counters[-1] = money_minus_summa // coins_last

        if summa + coins_last * counters[-1] == money:
            result += 1
            
        counters[-1] = max_counters_last

        ########################################################################
        # Second part - try to empty counters if maximum exceeded.
        ########################################################################

        i = len_counters

        while i != 0:
            while counters[i] >= max_counters[i] and i != 0:
                counters[i] = 0
                i -= 1
            else:
                counters[i] += 1

            # summa = sum(coin * count for coin, count in zip(coins, counters))
            j = 0
            summa = 0
            while j < len_counters:
                summa += coins[j] * counters[j]
                j += 1

            if summa < money:
                break
            elif summa == money:
                result += 1
            
            counters[i] = max_counters[i] + 1

    return result

# def count_change(money, coins):
#     if money<0:
#         return 0
#     if money == 0:
#         return 1
#     if money>0 and not coins:
#         return 0
#     return count_change(money-coins[-1],coins) + count_change(money,coins[:-1])

def count_change(money, coins):
    dp = [1]+[0]*money
    print(dp)
    for coin in coins:
        for x in range(coin,money+1):
            dp[x] += dp[x-coin]
    print(dp)
    return dp[money]

if __name__ == '__main__':
    assert count_change(3, [1, 2, 3]) == 3
    
    assert count_change(1, [1, 2]) == 1
    assert count_change(2, [1, 2]) == 2
    assert count_change(3, [1, 2]) == 2
    assert count_change(4, [1, 2]) == 3
    assert count_change(5, [1, 2]) == 3
    assert count_change(6, [1, 2]) == 4
    assert count_change(7, [1, 2]) == 4
    assert count_change(8, [1, 2]) == 5

    assert count_change(1, [1, 2, 3]) == 1
    assert count_change(2, [1, 2, 3]) == 2
    assert count_change(3, [1, 2, 3]) == 3
    assert count_change(4, [1, 2, 3]) == 4
    assert count_change(5, [1, 2, 3]) == 5
    assert count_change(6, [1, 2, 3]) == 7
    assert count_change(7, [1, 2, 3]) == 8
    assert count_change(8, [1, 2, 3]) == 10

    assert count_change(10, [5, 2, 3]) == 4

    assert count_change(1000, [5, 2, 3]) == 16834
    #assert count_change(10000000, [5, 2, 3]) == 1668334
