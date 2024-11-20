#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

string input = " ";

int readability(string input);

int main(int argc, char *argv[])
{

    input = get_string("Please provide an input: \n");

    int score = readability(input);

    if (score >= 16)
    {

        printf("Grade 16+\n");
    }
    else if (score < 16 && score >= 1)
    {

        printf("Grade %i\n", score);
    }
    else if (score < 1)
    {

        printf("Before Grade 1\n");
    };
};

int readability(string text)
{

    int rawValues[] = {};
    int wordcounter = 0;
    int sentencecounter = 0;
    int lettercounter = 0;
    // period(".") is 46 ; space(" ") is 32

    int stringlength = strlen(text);

    for (int i = 0; i < stringlength; i++)
    {
        printf("%i\n", text[i]);

        if ((text[i] == 46 || text[i] == 63 || text[i] == 33) &&
            (text[i - 1] != 63 || text[i - 1] != 33))
        {

            // if(text[i-1] >= 41 && text[i-1] <= 122) {

            sentencecounter++;
            printf("Sentence Counter: %i\n", sentencecounter);

            //}
        }
        else if (text[i] == 32)
        {

            wordcounter++;
            printf("Word Counter: %i\n", wordcounter);
        }
        else if (text[i] >= 41 && text[i] <= 122)
        {

            lettercounter++;
            printf("Letter Counter: %i\n", lettercounter);
        }

        if (sentencecounter == 0 && wordcounter > 0)
        {
            sentencecounter = 1;
        };
    };

    float input1 = (.0588 * ((float) lettercounter / wordcounter * 100));
    float input2 = (.296 * ((float) sentencecounter / wordcounter * 100));

    printf("%f\n", input1);
    printf("%f\n", input2);

    float output = roundf(input1 - input2 - 15.8);

    printf("%f\n", output);

    // index = 0.0588 * L - 0.296 * S - 15.8

    return output;
};
