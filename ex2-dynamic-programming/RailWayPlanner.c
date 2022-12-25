#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#define LINE_LEN 1024
#define OUTPUT_FILE_NAME "railway_planner_output.txt"
#define WRITE "w"
#define READ "r"
#define COMMA ","
#define ENTER '\n'
#define END_LINE "\r\n"
#define ONE 1
#define TWO 2
#define THREE 3
#define ARGS_ERROR "Usage: RailwayPlanner <InputFile>"
#define NO_FILE_ERROR "File doesn't exists."
#define INVALID_FILE_ERROR "Invalid input in line: %d."
#define EMPTY_FILE_ERROR "File is empty."
#define MIN_MSG  "The minimal price is: %d"

/**
 * A struct representing each part of the whole railway.
 */
typedef struct Part
{
    int start;
    int end;
    int partLen;
    int partPrice;
} Part;

/**
 * Global decelerations.
 */
long railWayLength;
long numberOfParts;
int ** table;
Part * parts;
char* partsTypes;

/**
 * A method thar validates the part fields.
 * @param line the line containing the part information.
 * @param part pointer to the part object
 * @param railWay pinter to the railway object.
 * @return 1 if there is a problem and 0 if not.
 */
int partInfoValidation(char line[LINE_LEN], Part * part)
{
    char end = 0;
    char start = 0;
    char *ptr = strtok(line, COMMA);
    int c = 0;
    while(ptr != NULL)
    {
        if(c < TWO && strlen(ptr) != ONE)
        {
            return 1;
        }
        if(c == 0)
        {
            strcpy(&start, ptr);
        }
        else if(c == ONE)
        {
            strcpy(&end, ptr);
        }
        else if(c == TWO)
        {
            for (int i = 0; i < (int)strlen(ptr); ++i)
            {
                if(isdigit(ptr[i]) == 0)
                {
                    return 1;
                }
            }
            part->partLen = (int)strtol(ptr, &ptr, 0);
        }
        else if(c == THREE)
        {
            for (int i = 0; i <(int) strlen(ptr); ++i)
            {
                if(isdigit(ptr[i]) == 0)
                {
                    return 1;
                }
            }
            part->partPrice = (int)strtol(ptr, &ptr, 10);
        }
        ptr = strtok(NULL, COMMA);
        c++;
    }
    if(c != 4 || part->partPrice <= 0 || part->partLen <= 0 )
    {
        return 1;
    }
    int j = 0;
    int k = 0;
    for (int i = 0; i <numberOfParts; ++i)
    {

        if(strchr(&partsTypes[i], start) == NULL)
        {
            j++;
        }
        if(strchr(&partsTypes[i], end) == NULL)
        {
            k++;
        }
    }
    if(j == numberOfParts || k == numberOfParts)
    {
        return 1;
    }
    part->start = (int)numberOfParts - ONE - j;
    part->end = (int)numberOfParts - ONE - k;
    return 0;
}

/**
 * A method thar validates the railway fields.
 * @param lineNum number of the specific line.
 * @param line that contains all the information.
 * @param railWay railway object
 * @return  1 if there is a problem and 0 if not.
 */
int railwayInfoValidation(int lineNum, char line[LINE_LEN])
{
    if(lineNum == ONE || lineNum == TWO)
    {
        char *ptr;
        for (int i = 0; i < (int)strlen(line); ++i)
        {
            if(isdigit(line[i]) == 0)
            {
                return 1;
            }
        }
        long num = strtol(line, &ptr, 10);
        if((num <= 0 && lineNum == TWO) || ( num < 0 && lineNum == ONE))
        {
            return ONE;
        }
        if(lineNum == ONE)
        {
            railWayLength = num;
        }
        else
        {
            numberOfParts = num;
        }
    }
    else if (lineNum == THREE)
    {
        partsTypes = calloc( numberOfParts + ONE , sizeof(char) );
        if(partsTypes == NULL)
        {
            return ONE;
        }
        char *comFinder;
        comFinder = strchr(line, ',');
        int i = 0;
        int commaCount = 0;
        while (comFinder != NULL)
        {
            commaCount++;
            comFinder = strchr(comFinder + 1, ',');
        }
        char *ptr = strtok(line, COMMA);
        while(ptr != NULL)
        {

            if(strlen(ptr) != ONE && ptr[strlen(ptr)-1] != ENTER)
            {
                return ONE;
            }
            strcpy(&partsTypes[i++], ptr);
            ptr = strtok(NULL, COMMA);
        }
        if(commaCount != i -1 )
        {
            return ONE;
        }
    }
    return 0;
}


/**
 * A method that creates a table in the size of railway length +1 * number of parts.
 */
int creatingTable()
{
    table = (int **)malloc(sizeof(int *)*(railWayLength + 1));
    if(table == NULL)
    {
        return ONE;
    }
    for(int i = 0; i < (int)railWayLength + 1; i++)
    {
        table[i] = (int *) malloc(sizeof(int) * numberOfParts);
        if(table[i] == NULL)
        {
            return ONE;
        }
    }
    for (int j = 0; j < railWayLength + 1 ; ++j)
    {
        for (int k = 0; k < numberOfParts; ++k)
        {
            if(j == 0)
            {
                table[j][k] = 0;
            }
            else
            {
                table[j][k] = INT_MAX;
            }
        }
    }
    return 0;
}

/**
 * The algorithm that returns the minimal price of building the railway.
 */
int algorithm(int partNum)
{
    if(creatingTable() == ONE)
    {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < railWayLength + 1; ++i)
    {
        for (int j = 0; j < partNum; ++j)
        {
            if (parts[j].partLen <= i)
            {
                long diff = i - parts[j].partLen;
                if (parts[j].partLen == i && table[i][parts[j].end] > parts[j].partPrice)
                {
                    table[i][parts[j].end] = parts[j].partPrice;
                }
                else if (parts[j].partLen < i && (table[diff][parts[j].start] != INT_MAX) &&
                         (table[diff][parts[j].start]
                         + parts[j].partPrice < table[i][parts[j].end]))
                {
                    table[i][parts[j].end] = table[diff][parts[j].start] + parts[j].partPrice;
                }
            }
        }
    }
    int min = table[railWayLength][0];
    for (int k = 0; k < numberOfParts; ++k)
    {
        if(table[railWayLength][k] < min)
        {
            min = table[railWayLength][k];
        }
    }
    if(min == INT_MAX || min < 0)
    {
        min = -ONE;
    }
    return(min);
}

/**
 * A method that reads the file line by line.
 * @param filePointer A pointer to the input file
 * @param outputFilePointer  A pointer to the output file.
 */
int readFile(FILE* filePointer, FILE* outputFilePointer)
{
    char curLine[LINE_LEN];
    int lineNum = ONE;
    int partNum = 0;
    int railValidationCheck = 0;
    int partValidationCheck = 0;
    parts = (Part *)calloc(ONE, sizeof(Part));
    if(parts == NULL)
    {
        return ONE;
    }
    while(fgets(curLine, LINE_LEN, filePointer) != NULL)
    {
        char *line = strtok(curLine, END_LINE);
        if(lineNum <= THREE )
        {
            railValidationCheck = railwayInfoValidation(lineNum, line);
        }
        else if(lineNum > THREE)
        {
            if(partNum  > 0 )
            {
                parts = (Part *)realloc(parts, sizeof(Part)*(partNum + ONE));
            }
            partValidationCheck = partInfoValidation(line, &parts[partNum]);
            partNum++;
        }
        if(railValidationCheck == ONE || partValidationCheck == ONE)
        {
            fprintf(outputFilePointer, INVALID_FILE_ERROR, lineNum);
            return ONE;
        }
        lineNum++;
    }

    if (lineNum == ONE)
    {
        fputs(EMPTY_FILE_ERROR, outputFilePointer);
        return ONE;
    }

    int min = algorithm(partNum);
    if(min == EXIT_FAILURE)
    {
        return TWO;
    }
    fprintf(outputFilePointer, MIN_MSG, min);
    return 0;
}

/**
 * A method that frees all the table allocated memory.
 */
void freeTable()
{
    for(int i = 0; i < railWayLength + ONE ; ++i)
    {
        free(table[i]);
        table[i] = NULL;
    }
    free(table);
}



/**
 * The main function of the program.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[])
{
    FILE* outputFilePointer = fopen(OUTPUT_FILE_NAME, WRITE);
    if (argc != TWO)
    {
        fputs(ARGS_ERROR, outputFilePointer);
        fclose(outputFilePointer);
        exit(EXIT_FAILURE);
    }
    FILE* filePointer = fopen(argv[1], READ);
    if(filePointer == NULL)
    {
        fputs(NO_FILE_ERROR, outputFilePointer);
        fclose(outputFilePointer);
        exit(EXIT_FAILURE);
    }
    else
    {
        int res = readFile(filePointer, outputFilePointer);
        if(res == 0 || res == TWO)
        {
            fclose(filePointer);
            fclose(outputFilePointer);
            freeTable();
            free(partsTypes);
            free(parts);
            if(res == 0)
            {
                exit(EXIT_SUCCESS);
            }
            else
            {
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            fclose(filePointer);
            fclose(outputFilePointer);
            free(parts);
            free(partsTypes);
            exit(EXIT_FAILURE);
        }
    }
}
