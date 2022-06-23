#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    //Prompt user to input their number as a long
    long input = get_long("Enter your card number: ");
    //Create placeholder value for input number
    long card = input;
    //Initialize values for sums, numbers for iteration placement, and remainders for breaking apart double digit numbers
    int evenSum = 0, oddSum = 0, num = 0, remainder = 0;
    //Initialize Boolean for card validity
    bool valid;
    //While placeholder is greater than 0
    while (card > 0)
    {
        //Multiply every other number by 2 starting with second to last number
        num = ((card / 10) % 10) * 2;
        //While that number is greater than 0
        while (num > 0)
        {
            //Look at 1st digit in double digit number
            remainder = num % 10;
            //Add numbers individually to even total
            evenSum += remainder;
            //Go to next digit
            num /= 10;
        }
        //Got to next even digit
        card /= 100;
    }
    //restore pre-iteration values
    remainder = 0;
    card = input;
    //While the card number is greater than 0
    while (card > 0)
    {
        //Obtain 1st digit not multiplied by 2
        remainder = card % 10;
        //Add that digit to the odd total
        oddSum += remainder;
        //Go to next odd digit
        card /= 100;
    }
    if ((evenSum + oddSum) % 10 == 0)
    {
        valid = true;
    }
    else
    {
        valid = false;
    }
    //if the card is valid
    if (valid == true)
    {
        //verify which card it is
        if ((input >= (34 * pow(10, 13)) && input < (35 * pow(10, 13))) || ((input >= (37 * pow(10, 13))) && input < (38 * pow(10, 13))))
        {
            printf("AMEX\n");
        }
        else if (input >= (51 * pow(10, 14)) && input < (56 * pow(10, 14)))
        {
            printf("MASTERCARD\n");
        }
        else if ((input >= (4 * pow(10, 12)) && input < (5 * pow(10, 12))) || ((input >= (4 * pow(10, 15))) && input < (5 * pow(10, 15))))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (valid == false)
    {
        printf("INVALID\n");
    }
}