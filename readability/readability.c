#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main(void)
{
    float numArr[3], l = 0, w = 0, s = 0, aveL, aveS, index;
    string p;
    // Prompt user to input a sentence
    p = get_string("Text: ");
    for (int i = 0; i < strlen(p); i++)
    {
        if (isalpha(p[i]))
        {
            l += 1;
        }
        else if (p[i] == ' ')
        {
            w += 1;
        }
        else if (i == strlen(p) - 1)
        {
            w += 1;
            s += 1;
        }
        else if (p[i] == '.' || p[i] == '?' || p[i] == '!')
        {
            s += 1;
        }
        else
        {
            continue;
        }
    }
    numArr[0] = l;
    numArr[1] = w;
    numArr[2] = s;
    // Call the average function for letters per 100 words
    aveL = (numArr[0] / numArr[1]) * 100.0;
    // Call the average function for sentences per 100 words
    aveS = (numArr[2] / numArr[1]) * 100.0;
    // Use the Coleman-Liau index formula
    index = ((0.0588 * aveL) - (0.296 * aveS) - 15.8);
    // Print the results to the terminal as Grade %i
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        int grade = round(index);
        printf("Grade %i\n", grade);
    }
}
