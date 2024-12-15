#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    if (card == NULL) {
        return 1;
    }

    uint8_t buffer[512];
    int counter = 0;
    FILE *img = NULL;
    bool foundjpeg = false;
    char filename[8];


    while (fread(buffer, 1, 512, card) == 512) {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {

            foundjpeg = true;

        };

        //open img file if header bytes found
        if(foundjpeg == true) {

            if(counter == 0) {
                //first img file
                sprintf(filename, "%03i.jpg", counter);

                img = fopen(filename, "w");
                fwrite(buffer, 1, 512, img);
                counter++;
                foundjpeg = false;


            } else if(counter > 0){
                //all subsequent img files
                fclose(img);
                sprintf(filename, "%03i.jpg", counter);

                img = fopen(filename, "w");
                fwrite(buffer, 1, 512, img);
                counter++;
                foundjpeg = false;

            }

        } else if(counter > 0) {
            //if an image goes across multiple buffers
            fwrite(buffer, 1, 512, img);

        }

    }
//close last image and card
fclose(img);
fclose(card);
return 0;

}
