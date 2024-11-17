#include <stdio.h>
#include <cs50.h>

void print_left_row(int length, int spaces);
void print_right_row(int length);

int main(void) {
    int height = 0;

     do {
      height = get_int("Please provide a number between 1-8 for the pyramid height: ");
     } while (height <= 0 || height > 8);

     for(int i = 0; i < height ; i++) {
        print_left_row(i + 1, height - i - 1);
        printf("  ");
        print_right_row(i + 1);
     };


}

void print_left_row(int length, int spaces) {

    for(int j = 0; j < spaces ; j++) {
        printf(" ");
    };
    for(int i = 0; i < length ; i++) {
        printf("#");
    };

};

void print_right_row(int length) {
    for(int i = 0; i < length ; i++) {
        printf("#");
    };
    printf("\n");
};
