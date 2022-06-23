#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Intitialize all int variables
    int i, j, k, rowNum;
    //Get number of rows from user
    do
    {
        rowNum = get_int("Enter a number between 1 and 8: ");
    }
    //While the number of input is more than 1 and less than 0
    while (rowNum < 1 || rowNum > 8);
    //For each row
    for (i = 1; i <= rowNum; i++)
    {
        //For each row up from the bottom
        for (j = 1; j <= rowNum - i; j++)
        {
            //Print a additional space
            printf(" ");
        }
        //For each space in the row
        for (k = 1; k <= (2 * i + 1); k++)
        {
            //If k is equal to the center of the row
            if (k == i + 1)
            {
                //Print a space
                printf("  ");
            }
            //Else
            else
            {
                //Print a #
                printf("#");
            }
        }
        //Print a new line
        printf("\n");
    }
}