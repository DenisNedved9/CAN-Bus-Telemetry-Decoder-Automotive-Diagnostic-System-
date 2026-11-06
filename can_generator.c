#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int id;      // ID modulu (určuje, KDO zprávu poslal)
    int length;  // Délka platných dat (v našem projektu vždy 8 bajtů)
    int8_t data; // Pole 8 bajtů nesoucích samotná data
} CAN_Frame;

int amount_of_times = 8;

int main()
{
    CAN_Frame *output = malloc(amount_of_times * sizeof(CAN_Frame));

    srand(time(NULL));

    for (int i = 0; i < amount_of_times; i++)
    {
        char name[50] = "";
        sprintf(name, "input_data\\data_%d", i);
        FILE *pFile = fopen(name, "w");
        if (pFile == NULL)
        {
            printf("Failed to open an file!");
            return 1;
        }
        int number = (rand() % 3) + 1;
        number *= 100;
        output[i].id = number;
        output[i].length = 8;

        number = rand() % 256;
        output[i].data = (int8_t)number;

        fprintf(pFile, "%d\n%d\n", output[i].id, output[i].length);
        for (int x = 7; x >= 0; x--)
        {
            fprintf(pFile, "%d", (output[i].data >> x) & 1);
        }
        fprintf(pFile, "\n");

        fclose(pFile);
    }

    free(output);
    return 0;
}
