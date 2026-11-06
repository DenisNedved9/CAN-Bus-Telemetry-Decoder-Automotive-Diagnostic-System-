#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int id;        // ID modulu (určuje, KDO zprávu poslal)
    int length;    // Délka platných dat (v našem projektu vždy 8 bajtů)
    char data[12]; // Pole 8 bajtů nesoucích samotná data
} CAN_Frame;

int main()
{
    int number = 0;
    FILE *pOutput = fopen("output_data.txt", "w");
    if (pOutput == NULL)
    {
        printf("Failed to open!");
        return 1;
    }
    int velikost = 2;
    CAN_Frame *files = malloc(velikost * sizeof(CAN_Frame));

    char buffer[96] = "0";


    while (1)
    {
        
        char name[40] = "";
        sprintf(name, "input_data\\data_%d", number);
        FILE *pFile = fopen(name, "r");
        if (pFile == NULL)
        {
            break;
        }
        if (number >= velikost)
        {
            velikost *= 2;
            CAN_Frame *temp = realloc(files, velikost * sizeof(CAN_Frame));
            if (temp != NULL)
            {
                files = temp;
            }
            else
            {
                printf("Chyba alokace pameti!\n");
                fclose(pFile);
                free(files);
                return 1;
            }
        }
        int type = 0;
        while (fgets(buffer, sizeof(buffer), pFile) != NULL)
        {

            buffer[strcspn(buffer, "\n")] = '\0';
            switch (type)
            {
            case 0:
                files[number].id = atoi(strdup(buffer));
                break;
            case 1:
                files[number].length = atoi(strdup(buffer));
                break;
            case 2:
                strcpy(files[number].data, strdup(buffer));
                break;
            }

            type++;
        }
        // printf("%d\n", files[the_number].id);
        //  printf("%d\n", files[the_number].length);
        //  printf("%s\n", files[the_number].data);

        fclose(pFile);

        number++;
    }
    // printf("%d\n", number);
    for (int i = 0; i < number; i++)
    {
        // printf("%d\n", i);
        // printf("%d\n", files[i].id);
        if (files[i].id == 100)
        {
            int RPM = 0;
            if (files[i].data[6] == '1')
            {
                RPM += 10;
            }
            if (files[i].data[5] == '1')
            {
                RPM += 100;
            }
            if (files[i].data[4] == '1')
            {
                RPM += 1000;
            }
            if (files[i].data[3] == '1')
            {
                RPM += 2000;
            }
            if (files[i].data[2] == '1')
            {
                RPM += 3000;
            }
            if (files[i].data[1] == '1')
            {
                RPM += 4000;
            }

            fprintf(pOutput, "ENGINE RECORD:\n\n");
            fprintf(pOutput, "Temperature: %s\n", files[i].data[7] == '1' ? "normal" : "too hot");
            fprintf(pOutput, "approximate RPM: %d\n", RPM);
            fprintf(pOutput, "Oil pressure: %s\n", files[i].data[0] == '1' ? "normal" : "too strong");
        }
        else if (files[i].id == 200)
        {
            fprintf(pOutput, "DOORS RECORD:\n\n");
            fprintf(pOutput, "Right front door: %s\n", files[i].data[7] == '1' ? "open" : "closed");
            fprintf(pOutput, "Left front door: %s\n", files[i].data[6] == '1' ? "open" : "closed");
            fprintf(pOutput, "Right back door: %s\n", files[i].data[5] == '1' ? "open" : "closed");
            fprintf(pOutput, "Left back door: %s\n", files[i].data[4] == '1' ? "open" : "closed");
            fprintf(pOutput, "Trunk: %s\n", files[i].data[3] == '1' ? "open" : "closed");
        }
        else if (files[i].id == 300)
        {
            fprintf(pOutput, "Brake System RECORD:\n\n");
            fprintf(pOutput, "ABS Status: %s\n", files[i].data[7] == '1' ? "on" : "off");
            fprintf(pOutput, "Brake Pad Wear: %s\n", files[i].data[6] == '1' ? "ok" : "need replacement!");
            fprintf(pOutput, "Electronic Stability Control: %s\n", files[i].data[5] == '1' ? "stable" : "not stable!");
            fprintf(pOutput, "Right front tire brake system: %s\n", files[i].data[4] == '1' ? "ok" : "needs to be checked");
            fprintf(pOutput, "Left front tire brake system: %s\n", files[i].data[3] == '1' ? "ok" : "needs to be checked");
            fprintf(pOutput, "Right back tire brake system: %s\n", files[i].data[2] == '1' ? "ok" : "needs to be checked");
            fprintf(pOutput, "Left back tire brake system: %s\n", files[i].data[1] == '1' ? "ok" : "needs to be checked");
        }

        fprintf(pOutput, "\n________________________________________\n\n");
    }

    printf("Saved into 'output_data.txt'");

    fclose(pOutput);
    free(files);
    return 0;
}