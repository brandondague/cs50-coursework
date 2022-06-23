#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    // If score1 is greater than score2
    if (score1 > score2)
    {
        // Print Player 1 wins!
        printf("Player 1 wins!\n");
    }
    // If score2 is greater than score1
    else if (score2 > score1)
    {
        // Print Player 2 wins!
        printf("Player 2 wins!\n");
    }
    // If the scores are equal
    else if (score1 == score2)
    {
        // Print Tie!
        printf("Tie!\n");
    }
    // Else
    else
    {
        // Print INVALID
        printf("INVALID");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    // Initialize score total and index of input string as an int equal to 0
    int scoreTot = 0, pointsIndex, i = 0;
    // While the current index is not equal to the length of the input word
    while (i != strlen(word))
    {
        // If the char at the current index is upper case
        if (isupper(word[i]))
        {
            // Subtract the ASCII value of 'A' from the char at the current index to get the alphebetical order
            pointsIndex = word[i] - 'A';
            // Add the points from the points array to the score total
            scoreTot += POINTS[pointsIndex];
        }
        // Repeat for lower case letters
        else if (islower(word[i]))
        {
            pointsIndex = word[i] - 'a';
            scoreTot += POINTS[pointsIndex];
        }
        // Discard punctuation
        else
        {
            scoreTot += 0;
        }
        // Increment i
        i++;
    }
    // Return the score total
    return scoreTot;
}
