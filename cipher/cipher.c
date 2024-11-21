#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {

    //If your program is executed without any command-line arguments or with more than one command-line argument, your program should print an error message of your choice (with printf) and return from main a value of 1 (which tends to signify an error) immediately.
    // printf("%li\n", strlen(argv[1]));

    if (argc < 1 || argc > 2) {

        printf("Please input only one argument\n");

        return 1;

    } else if (argv[1]) {

//If the key is invalid (as by not containing 26 characters, containing any character that is not an alphabetic character, or not containing each letter exactly once), your program should print an error message of your choice (with printf) and return from main a value of 1 immediately.
    int keylength = strlen(argv[1]);

    if(keylength != 26) {

        printf("Please input only 26 Characters\n");
        return 1;
    };

    for(int i = 0; i < keylength; i++) {
        if(isalpha(argv[1][i]) == 0) {

            printf("Please input only alphabetic characters\n");
            return 1;

        } else {

            for(int j = 0; j < keylength; j++) {

                if(argv[1][j] == argv[1][i] && i != j) {

                    printf("Do not duplicate letters\n");
                    return 1;
                };

            }


        };


    };


    //Your program must output plaintext: (without a newline) and then prompt the user for a string of plaintext (using get_string).
    printf("plaintext: ");
    string input = "";
    input = get_string("Please provide an input ");
    //Your program must output ciphertext: (without a newline) followed by the plaintext’s corresponding ciphertext, with each alphabetical character in the plaintext substituted for the corresponding character in the ciphertext; non-alphabetical characters should be outputted unchanged.
    printf("ciphertext: ");
    for(int k = 0; k < strlen(input) ; k++) {

        if (isspace(input[k] > 0)) {

            printf("%c", input[k]);

        } else if(isblank(input[k]) > 0) {

            printf("%c", input[k]);

        } else if(ispunct(input[k]) > 0) {

            printf("%c", input[k]);

        }else if(isdigit(input[k]) > 0) {

            printf("%c", input[k]);

        } else if(isupper(input[k]) > 0) {

            int index = input[k] - 65;
            printf("%c", toupper(argv[1][index]));


        } else if(islower(input[k]) > 0) {

            if (input[k] >= 97 && input[k] <= 122) {

                int index = input[k] - 'a';
                // int index = input[k] - 97;
                printf("%c", tolower(argv[1][index]));


            } else {

                 printf("%c", input[k]);

            };

                // printf("%c", tolower(input[k]));



        };


    };

    printf("\n");

    } else if(!argv[1]) {
        return 1;
    };


};
