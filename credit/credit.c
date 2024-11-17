#include <stdio.h>
#include <cs50.h>
#include <string.h>

int checksum(long creditcard, int length);
int firstDigit(int n);
int get_int_len(long value);
string creditcheck(long creditcard, int length);

int main(void) {

    long creditcard = get_long("Please provide a Credit Card Number: ");
    int length = get_int_len(creditcard);
    //printf("Main Length: %i\n", length);
    int checksumcheck = checksum(creditcard, length);
    //printf("Main Check Sum Check: %i\n", checksumcheck);
    string creditcheckcheck = creditcheck(creditcard, length);
    //printf("Main Credit Check Check: %s\n", creditcheckcheck);
    if(checksumcheck == 1) {
        printf("%s", creditcheckcheck);
    } else if (checksumcheck == 0) {
        printf("INVALID\n");
    };
};

int firstDigit(int n)
{
    while (n >= 10)
        n /= 10;

    return n;
};

int get_int_len(long value){
  int l=1;
  while(value > 9){
    l++;
    value/=10; }
  return l;
};

int checksum(long creditcard, int length) {
    //int length = get_int_len(creditcard);
    //printf("%i\n", length);
    int sum = 0;
    for(int i = 0 ; i <= length ; i+=2) {
        long level = 0;
        long division = 1;
        if (i > 0) {
            level = 10;
            for(int j = 0 ; j < i - 1 ; j++) {
            level = level * 10;
            division = level/10;
            };
              //printf("First Sum Level: %ld\n", level);

            int digit = (creditcard % level)/division;

            //printf("First Sum Level Digit: %i\n", digit);
            int doubled = digit * 2;
            //printf("First Sum Level Doubled: %i\n", doubled);
            if (doubled >= 10) {
                int firstdigit = doubled / 10;
                int lastdigit = doubled % 10;
                sum = sum + firstdigit + lastdigit;
                //printf("First Sum: %i = %i + %i + %i\n", sum, sum, firstdigit, lastdigit);
            } else if (doubled < 10) {
                sum = sum + doubled;
                //printf("First Sum: %i = %i + %i\n", sum, sum, doubled);

            };

        };


    };

    int secondsum = 0;
    for(int i = 1 ; i <= length ; i+=2) {
        long division = 1;
        long digit = creditcard/10;
        long level = 10;
        if (i > 0) {
            for(int j = 0 ; j < i - 1 ; j++) {
            level = level * 10;
            division = level/10;
            };
            //printf("Second Sum Level: %ld\n", level);

        };

        digit = (creditcard % level)/division;
        //printf("Second Sum Level Digit: %li\n", digit);
        secondsum = secondsum + digit;
        //printf("Second Sum: %i = %i + %li\n", secondsum, secondsum, digit);

    };
    int totalsum = sum + secondsum;
    //printf("Total Sum: %i = %i + %i\n", totalsum, sum, secondsum);

    int remainder = totalsum % 10;
    // printf("Remainder: %i = %i modulo 10\n", remainder, totalsum);
    if(remainder == 0) {
            return 1;
        } else return 0;

};

string creditcheck(long creditcard, int length) {
    while (creditcard > 100) {
            creditcard /= 10;
        };
    int twodigits = creditcard;
    while (creditcard > 10) {
            creditcard /= 10;
        };
    int firstdigit = creditcard;

    string response = "INVALID\n";
    printf("Credit Check Check Entered");
    printf("Two Digits: %i First Digit: %i", twodigits, firstdigit);

    //Visa uses 13- and 16-digit numbers && all Visa numbers start with 4.
    if(firstdigit == 4) {
        printf("Visa Check Check Entered");
        if(length == 16 || length == 13) {
            response = "VISA\n";
            return response;
        };
    //American Express uses 15-digit numbers && All American Express numbers start with 34 or 37
    } else if (length == 15) {
        printf("AMEX Check Check Entered");
        if(twodigits == 34 || twodigits == 37) {
            response = "AMEX\n";
            return response;
        }
    //MasterCard uses 16-digit numbers, and most MasterCard numbers start with 51, 52, 53, 54, or 55
    } else if (length == 16) {
        printf("Mastercard Check Check Entered");
        if(firstdigit == 4) {
            response = "VISA\n";
            return response;
        } else if(twodigits > 50 && twodigits < 56) {
            response = "MASTERCARD\n";
            return response;
        }
    }
    return response;






};




