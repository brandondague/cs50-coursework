#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int validateKey(string s);

int main(int argc, string argv[])
{
    // If there is the correct number of input args
    if (argc == 2)
    {
        // Tracker for current index
        int cipherIndex;
        int valid = validateKey(argv[1]);
        if (valid == 1)
        {
            return 1;
        }
        else
        {
            // Promt user to enter a plaintext message
            string plainText = get_string("plaintext: ");
            char letter, upper, lower;
            // For each character in the message
            for (int i = 0; i < strlen(plainText); i++)
            {
                // If the char is uppercase
                if (isupper(plainText[i]))
                {
                    // Substract the ASCII value of 'A' from the char at the current index to equal to the cipher index
                    cipherIndex = plainText[i] - 'A';
                    // If the char at that cipher index is also upper case
                    if (isupper(argv[1][cipherIndex]))
                    {
                        // Set the char in the plaintext message equal to it's corresponding cipher value
                        plainText[i] = argv[1][cipherIndex];
                    }
                    else
                    {
                        // Or convert the value in the cipher to uppercase
                        letter = argv[1][cipherIndex];
                        upper = toupper(letter);
                        plainText[i] = upper;
                    }
                }
                // Do the opposite for lower case
                else if (islower(plainText[i]))
                {
                    cipherIndex = plainText[i] - 'a';
                    if (islower(argv[1][cipherIndex]))
                    {
                        plainText[i] = argv[1][cipherIndex];
                    }
                    else
                    {
                        letter = argv[1][cipherIndex];
                        lower = tolower(letter);
                        plainText[i] = lower;
                    }
                }
                else
                {
                    plainText[i] = plainText[i];
                }
            }
            printf("ciphertext: %s", plainText);
        }
    }
    else
    {
        // Remind user how to use the program
        printf("Usage: .substitution key");
        return 1;
    }
    printf("\n");
}

int validateKey(string s)
{
    if (strlen(s) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        for (int j = 0; j < 26; j++)
        {
            if (s[j] < 'A' || (s[j] > 'Z' && s[j] < 'a') || s[j] > 'z')
            {
                printf("Key contains an invalid character.\n");
                return 1;
            }
            for (int k = j + 1; k < 26; k++)
            {
                if (s[k] == s[j])
                {
                    printf("Key contains a duplicate character.\n");
                    return 1;
                }
            }
        }
        return 0;
    }
}