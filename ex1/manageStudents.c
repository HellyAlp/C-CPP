#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <math.h>

#define TEN 10
#define NAME_REGEX "^[A-Za-z -]*$"
#define CITY_COUNTRY_REGEX "^[A-Za-z-]*$"
#define INPUT_PATTERN "%lu,%[^,],%lu,%lu,%[^,],%[^,]"
#define ID_ERROR "ERROR: id must be a 10 digits number that does not start with 0\nin line %d\n"
#define NAME_ERROR "ERROR: name can only contain alphabetic characters, whitespaces or '-'\nin line %d\n"
#define GRADE_ERROR "ERROR: grade must be an integer between 0 and 100\nin line %d\n"
#define AGE_ERROR "ERROR: age must be an integer between 18 and 120\nin line %d\n"
#define COUNTRY_ERROR "ERROR: country can only contain alphabetic characters or '-'\nin line %d\n"
#define CITY_ERROR "ERROR: city can only contain alphabetic characters or '-'\nin line %d\n"
#define FORMAT_ERROR "ERROR: info must match specified format\nin line %d\n"
#define EOF_ERROR "ERROR: could not read input\nin line %d"
#define USAGE_ERROR "USAGE: you can run the program only with one of the following arguments: best,merge or quick"
#define INSTRUCTION "Enter student info. To exit press q, then enter\n"
#define BEST_STUDENT_MESSAGE "best student info is: %s"
#define EXIT "q\n"
#define STRING_PRINT "%s"
#define INPUT_LENGTH 61
#define FILED_LENGTH 43
#define MAX_STUDENT_NUM 5500
#define MAX_GRADE 100
#define MAX_AGE 120
#define MIN_AGE 18
#define BEST "best"
#define MERGE "merge"
#define QUICK "quick"
#define OPTION_BEST 1
#define OPTION_MERGE 2
#define OPTION_QUICK 3
#define PROGRAM_FAILED 1
#define PROGRAM_SUCCEEDED 0


/**
 * A method that validates id format
 * @param id int
 * @return 0 if the id is valid and 1 if not.
 */
int idValidationCheck(unsigned long id)
{
    int count = 0;
    while( id != 0)
    {
        id /= TEN;
        ++count;
    }
    if(count == TEN)
    {
        return PROGRAM_SUCCEEDED;
    }
    else
    {
        return PROGRAM_FAILED;
    }
}

/**
 * A method that validates name format
 * @param name char array
 * @return 0 if the name is valid and 1 if not.
 */
int nameValidationCheck(char name[FILED_LENGTH])
{
    regex_t regex;
    regcomp(&regex, NAME_REGEX, 0);
    if(regexec(&regex, name, 0, NULL, 0) == 0)
    {
        return PROGRAM_SUCCEEDED;
    }
    return PROGRAM_FAILED;
}

/**
 * A method that validates city or country format
 * @param name char array
 * @return 0 if the  city or country is valid and 1 if not.
 */
int cityAndCountryValidationCheck(char name[FILED_LENGTH])
{
    regex_t regex;
    regcomp(&regex, CITY_COUNTRY_REGEX, 0);
    strtok(name, "\n");
    if(regexec(&regex, name, 0, NULL, 0) == 0)
    {
        return PROGRAM_SUCCEEDED;
    }
    return PROGRAM_FAILED;
}

/**
 * A method that validates that the user input is in the right format.
 * @param id int
 * @param name char array
 * @param grade int
 * @param age int
 * @param country char array
 * @param city char array
 * @param lineCount int representing the line number of the input.
 * @return 0 if the input is in the right format, 1 if not.
 */
int validationCheck(unsigned long id, char name[FILED_LENGTH], unsigned long grade, unsigned long age,
                    char country[FILED_LENGTH], char city[FILED_LENGTH], int lineCount)
{
    if (idValidationCheck(id) != 0)
    {
        printf(ID_ERROR, lineCount);
        return PROGRAM_FAILED;
    }
    else if(nameValidationCheck(name) != 0)
    {
        printf(NAME_ERROR, lineCount);
        return PROGRAM_FAILED;
    }
    else if( grade > MAX_GRADE)
    {
        printf(GRADE_ERROR, lineCount);
        return PROGRAM_FAILED;
    }
    else if(age < MIN_AGE || age > MAX_AGE)
    {
        printf(AGE_ERROR, lineCount);
        return PROGRAM_FAILED;
    }
    else if(cityAndCountryValidationCheck(country) != 0)
    {
        printf(COUNTRY_ERROR, lineCount);
        return PROGRAM_FAILED;
    }
    else if(cityAndCountryValidationCheck(city) != 0)
    {
        printf(CITY_ERROR, lineCount);
        return PROGRAM_FAILED;
    }
    return PROGRAM_SUCCEEDED;
}

/**
 * A new struct representing a student object defining its details.
 */
struct Student
{
    unsigned long id;
    char name[FILED_LENGTH];
    unsigned long grade;
    unsigned long age;
    char country[FILED_LENGTH];
    char city[FILED_LENGTH];
    double score;
    char studentInfo[INPUT_LENGTH];
};

/**
 * An helper function for the partition method.
 * This method swaps between two students in the array.
 * @param student1 pointer to Student struct object
 * @param student2 pointer to Student struct object
 */
void swap(struct Student* student1, struct Student* student2)
{
    struct Student tempStudent = *student1;
    *student1 = *student2;
    *student2 = tempStudent;
}

/**
 * An helper function for the quickSort function.
 * A function that defines a pivot and sorts the items smaller than the pivot to the right and bigger to the left.
 * @param array Array of students.
 * @param low int representing an index/
 * @param high int representing an index.
 * @return int a new pivot
 */
int partition (struct Student array[MAX_STUDENT_NUM], int low, int high)
{
    char pivot[FILED_LENGTH];
    strcpy(pivot, array[high].name);
    int i = (low - 1);
    for (int j = low; j <= high- 1; j++)
    {
        if (strcmp(array[j].name, pivot)<0)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

/**
 * A function that gets an array and sorts it by the quickSort algorithm.
 * @param array Array of students.
 * @param low int representing an index/
 * @param high int representing an index.
 */
void quickSort(struct Student array[MAX_STUDENT_NUM], int low, int high)
{
    if (low < high)
    {
        int pivot = partition(array, low, high);
        quickSort(array, low, pivot - 1);
        quickSort(array, pivot + 1, high);
    }
}

/**
 * A function that divide the big array into small arrays and than merge them.
 * @param array Array of students.
 * @param left start index
 * @param middle middle index
 * @param right right index.
 */
void merge(struct Student array[MAX_STUDENT_NUM], int left, int middle, int right)
{
    int leftArraySize = middle - left + 1;
    int rightArraySize = right - middle;
    struct Student leftArray[MAX_STUDENT_NUM];
    struct Student rightArray[MAX_STUDENT_NUM];
    for (int i = 0; i < leftArraySize; i++)
    {
        leftArray[i] = array[left + i];
    }
    for (int j = 0; j < rightArraySize; j++)
    {
        rightArray[j] = array[middle + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;
    while (i < leftArraySize && j < rightArraySize)
    {
        if (leftArray[i].grade <= rightArray[j].grade)
        {
            array[k] = leftArray[i];
            i++;
        }
        else
        {
            array[k] = rightArray[j];
            j++;
        }
        k++;
    }

    while (i < leftArraySize)
    {
        array[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < rightArraySize)
    {
        array[k] = rightArray[j];
        j++;
        k++;
    }
}

/**
 * This method sorts an array by the merge sort algorithm .
 * @param array Array of students we want to sort.
 * @param left starting point.
 * @param right ending point.
 */
void mergeSort(struct Student array[MAX_STUDENT_NUM], int left, int right)
{
    if (left < right)
    {
        int middle = left + (right-left) / 2;
        mergeSort(array, left, middle);
        mergeSort(array, middle + 1, right);
        merge(array, left, middle, right);
    }
}

/**
 * A method that gets an array and print to the stdout the student Info of all the students in the array.
 */
void printSortedResult(struct Student array[MAX_STUDENT_NUM], int lineCount)
{
    int i;
    for (i = 0; i < lineCount; i++)
    {
        printf(STRING_PRINT, array[i].studentInfo);
    }
    printf(STRING_PRINT, array[i].studentInfo);
}

/**
 * A method that generates the main loop of the program, ask the user for input and deals with it.
 * @param i 1 if the arg is best, 2 if the arg is merge, 3 if the arg is quick
 */
int generalLoop(int i)
{
    int lineCount = 0;
    int index = 0;
    int j = 0;
    int k = 0;
    double bestScore = 0;
    char bestStudentInfo[INPUT_LENGTH];
    struct Student uniStudents[MAX_STUDENT_NUM];
    struct Student currentStudent;
    printf(INSTRUCTION);

    if(fgets(currentStudent.studentInfo, INPUT_LENGTH, stdin) == NULL)
    {
        printf(EOF_ERROR, lineCount);
        return PROGRAM_FAILED;
    }
    while (strcmp(currentStudent.studentInfo, EXIT) != 0)
    {
        int len = sscanf(currentStudent.studentInfo, INPUT_PATTERN, &currentStudent.id,
                currentStudent.name, &currentStudent.grade, &currentStudent.age
                , currentStudent.country, currentStudent.city);
        if(len != 6)
        {
            printf(FORMAT_ERROR, lineCount);
            printf(INSTRUCTION);
            lineCount++;
            if(fgets(currentStudent.studentInfo, INPUT_LENGTH, stdin) == NULL)
            {
                printf(EOF_ERROR, lineCount);
                return PROGRAM_FAILED;
            }
            continue;
        }
        if(validationCheck(currentStudent.id, currentStudent.name, currentStudent.grade, currentStudent.age,
           currentStudent.country, currentStudent.city, lineCount) == 0)
        {
            j = 1;
            index++;
            strcpy(uniStudents[lineCount].studentInfo, currentStudent.studentInfo);
            strcpy(uniStudents[lineCount].name, currentStudent.name);
            uniStudents[lineCount].grade = currentStudent.grade;
            uniStudents[lineCount].score = (double )currentStudent.grade / (double )currentStudent.age;
        }
        else
        {
            printf(INSTRUCTION);
            lineCount++;
            if(fgets(currentStudent.studentInfo, INPUT_LENGTH, stdin) == NULL)
            {
                printf(EOF_ERROR, lineCount);
                return PROGRAM_FAILED;
            }
            continue;
        }
        if(i == 1 && (uniStudents[lineCount].score > bestScore ||  (uniStudents[lineCount].score == 0 && k == 0)))
        {
            k = 1;
            bestScore = (double) uniStudents[lineCount].score;
            strcpy(bestStudentInfo, uniStudents[lineCount].studentInfo);
        }
        printf(INSTRUCTION);
        lineCount++;
        if(fgets(currentStudent.studentInfo, INPUT_LENGTH, stdin) == NULL)
        {
            printf(EOF_ERROR, lineCount);
            return PROGRAM_FAILED;
        }
    }
    if(i == OPTION_BEST && j == 1)
    {
        printf(BEST_STUDENT_MESSAGE, bestStudentInfo);
    }
    else if (i == OPTION_MERGE)
    {
        mergeSort(uniStudents, 0, index - 1);
        printSortedResult(uniStudents, index );
    }
    else if (i == OPTION_QUICK)
    {
        quickSort(uniStudents, 0, index - 1);
        printSortedResult(uniStudents, index);
    }
    return PROGRAM_SUCCEEDED;
}


/**
* The programs main method.
 * It gets user input that has 3 options: "best","merge" and "quick"
*/
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf(USAGE_ERROR);
        return PROGRAM_FAILED;
    }
    if (strcmp(argv[1], BEST) == 0)
    {
        if(generalLoop(OPTION_BEST) == 1)
        {
            return PROGRAM_FAILED;
        }

    }
    else if (strcmp(argv[1], MERGE) == 0)
    {
        if(generalLoop(OPTION_MERGE) == 1)
        {
            return PROGRAM_FAILED;
        }
    }
    else if (strcmp(argv[1], QUICK) == 0)
    {
        if(generalLoop(OPTION_QUICK) == 1)
        {
            return PROGRAM_FAILED;
        }
    }
    else
    {
        printf(USAGE_ERROR);
        return PROGRAM_FAILED;
    }
    return PROGRAM_SUCCEEDED;
}
