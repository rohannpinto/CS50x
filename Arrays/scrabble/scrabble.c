#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const int values[] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                      1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
int pointValues(string input);

int main(void)
{

    string player1 = get_string("Player 1: ");
    string player2 = get_string("Player 2: ");

    int score1 = pointValues(player1);
    int score2 = pointValues(player2);

    if (score1 > score2)
    {

        printf("Player 1 Wins!\n");
    }
    else if (score2 > score1)
    {

        printf("Player 2 Wins!\n");
    }
    else if (score1 == score2)
    {

        printf("Tie!\n");
    }
};

int pointValues(string input)
{

    int score = 0;
    for (int i = 0; i < strlen(input); i++)
    {

        int character = input[i];
        if (isupper(input[i]))
        {

            score = score + values[(int) character - 65];
        }
        else if (islower(input[i]))
        {

            score = score + values[(int) character - 97];
        }
    };

    return score;
};
