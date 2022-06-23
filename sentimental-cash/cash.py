from cs50 import get_float

coins = 0

while True:
    change = get_float("Change owed: ")
    if change >= 0:
        break

change = change * 100

while change > 0:
    if change >= 25:
        coins += 1
        change -= 25
    elif change >= 10:
        coins += 1
        change -= 10
    elif change >= 5:
        coins += 1
        change -= 5
    elif change >= 1:
        coins += 1
        change -= 1

print(f"{coins}")