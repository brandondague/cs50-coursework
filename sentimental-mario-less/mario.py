from cs50 import get_int

while True:
    rows = get_int("Height: ")
    if rows > 0 and rows <= 8:
        break
for i in range(rows):
    print()
    for j in range(rows):
        if j <= rows - (i + 2):
            print(" ", end="")
        else:
            print("#", end="")
print()