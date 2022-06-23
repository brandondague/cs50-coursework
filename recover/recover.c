#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512
typedef uint8_t BYTE;
int main(int argc, char *argv[])
{
    // Check there is a valid number of inputs
    if (argc < 2)
    {
        printf("Usage ./recover IMAGE\n");
        return 1;
    }
    // Open card.raw
    FILE *raw_file = fopen(argv[1], "r");
    // If no file found throw error
    if (raw_file == NULL)
    {
        printf("ERROR: Could not open card.raw\n");
        return 2;
    }
    // Initialize variables
    int count = 0;
    FILE *picture = NULL;
    BYTE buffer[BLOCK_SIZE];
    // Loop through all blocks on card.raw
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // Check if next 4 bytes match the .jpg header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] >= 0x30 && buffer[3] <= 0xef))
        {
            // If this isn't the first .jpg open
            if (count != 0)
            {
                // Close the previous .jpg
                fclose(picture);
            }
            // Initialize the file
            char file[8];
            sprintf(file, "%03i.jpg", count);
            picture = fopen(file, "w");
            count++;
        }
        // If the .jpg has been found
        if (count != 0)
        {
            // Write to file
            fwrite(&buffer, 1, BLOCK_SIZE, picture);
        }
    }
    // Close all files
    fclose(raw_file);
    fclose(picture);
}