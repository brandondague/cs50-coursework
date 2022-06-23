from cs50 import get_string

l = w = s = 0
p = get_string("Text: ")
length = len(p)

for i in range(length):
    if (p[i].isalpha()):
        l += 1
    elif (p[i] == " "):
        w += 1
    elif (i == (len(p) - 1)):
        w += 1
        s += 1
    elif (p[i] == "." or p[i] == "?" or p[i] == "!"):
        s += 1
    else:
        continue

aveL = l / w * 100.0
aveS = s / w * 100.0
index = round((0.0588 * aveL) - (0.296 * aveS) - 15.8)

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")