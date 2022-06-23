#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Prompt user to input name
    string name = get_string("What's your name? ");
    //Print "Hello, " + the user's input
    printf("Hello, %s!\n", name);
}