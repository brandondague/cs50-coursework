// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    // Create an array called header that is the size of the header with the uint8_t data type
    uint8_t header[HEADER_SIZE];
    // Read from the input file to the location of the header array
    fread(&header, HEADER_SIZE, 1, input);
    // Write to the output file from the location of the header array
    fwrite(&header, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file
    // Create a new variable with the int16_t datatype
    int16_t sample;
    // Read from the input file to the location of the variable and while there is something there
    while (fread(&sample, sizeof(int16_t), 1, input))
    {
        // Multiply the original sample by the factor porvided by the user
        sample *= factor;
        // Write this from the location of the variable to the output file
        fwrite(&sample, sizeof(int16_t), 1, output);
    }
    // Close files
    fclose(input);
    fclose(output);
}
