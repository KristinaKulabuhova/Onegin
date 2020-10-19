#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

#define MIN(X, Y) (X) < (Y) ? (X) : (Y)

typedef struct Str
{
    char* buffer;
    size_t size;
} Str;

typedef struct Pointer
{
    char** buffer;
    size_t size;
} Pointer;

void Replace(Str* file, const char from, const char to)
{
    size_t count_str = 0;
    for (int i = 0; i < file->size; ++i) 
    {
        if (from == file->buffer[i]) 
        {
            file->buffer[i] = to;
        }
    }
}

size_t CountSymbol(Str* file, const char to)
{
    size_t count_str = 0;
    for (int i = 0; i < file->size; ++i) 
    {
        if (to == file->buffer[i]) 
        {
            ++count_str;
        }
    }
    return count_str;
}

 int cmp(const void* p_first,const void* p_second)
 {
    char* str_first = *((char**) p_first);
    char* str_second = *((char**) p_second);
    size_t first_size = strlen(str_first);
    size_t second_size = strlen(str_second);
    int i = 0;
    int j = 0;
    for (; i < first_size && j < second_size; ++i, ++j)
    {
        while (ispunct(str_first[i]) || isspace(str_first[i]))
        {
            ++i;
        }
        while (ispunct(str_second[j]) || isspace(str_second[j]))
        {
            ++j;
        }
        if (str_second[j] < str_first[first_size - i]) 
        {
            return 1;
        }
        if (str_second[j] > str_first[i])
        {
            return -1;
        }
    }
    return i - j;    
 }

int Compare(const void* p_first,const void* p_second) {
    char* str_first = *(char**)p_first;
    char*str_second = *(char**)p_second;
    return cmp(str_first, str_second);
}

int CompareBackwards(const void* p_first,const void* p_second)
{
    char* str_first = *((char**) p_first);
    char* str_second = *((char**) p_second);
    size_t first_size = strlen(str_first);
    size_t second_size = strlen(str_second);
    int i = 1;
    int j = 1;
    for (; i <= first_size && j <= second_size; ++i, ++j)
    {
        while (ispunct(str_first[first_size - i]) || isspace(str_first[first_size - i]))
        {
            ++i;
        }
        while (ispunct(str_second[second_size - j]) || isspace(str_second[second_size - j]))
        {
            ++j;
        }
        if (str_second[second_size - j] < str_first[first_size - i]) 
        {
            return 1;
        }
        if (str_second[second_size - j] > str_first[first_size - i])
        {
            return -1;
        }
    }
    return i - j;
}

size_t CountOfSameSymbol(char* str_first, char* str_second)
{
    size_t count = 0;
    size_t first_size = strlen(str_first);
    size_t second_size = strlen(str_second);
    int i = 1;
    int j = 1;
    while (ispunct(str_first[first_size - i]) || isspace(str_first[first_size - i]))
    {
        ++i;
    }
    while (ispunct(str_second[second_size - j]) || isspace(str_second[second_size - j]))
    {
        ++j;
    }
    for (; str_second[second_size - j] == str_first[first_size - i] && j < second_size && i < first_size; ++i, ++j)
    {
        ++count;
    }
    return count;
}

size_t Random(Pointer* buffer_of_strings, bool* is_this_string_in_poem) 
{
    size_t index = 0;
    do 
    {
        index = rand() % buffer_of_strings->size;
    } while (is_this_string_in_poem[index] != false);
    return index;
}

void SelectingLinesForRhyme(bool* is_this_string_in_poem, size_t index, Pointer* buffer_of_strings, Pointer* rhym, size_t* count_bool, int* i)
{
    is_this_string_in_poem[index] = true;
    ++(*count_bool);
    size_t count_one = 0;
    size_t count_two = 0;
    if (index >= 0 && index < buffer_of_strings->size - 1) 
    {
        count_two = CountOfSameSymbol(buffer_of_strings->buffer[index], buffer_of_strings->buffer[index + 1]);
    }
    if (index > 0 && index <= buffer_of_strings->size - 1)
    {
        count_one = CountOfSameSymbol(buffer_of_strings->buffer[index], buffer_of_strings->buffer[index - 1]);
    }
    if (count_one > count_two && count_one > 0) {
        rhym->buffer[*i] = buffer_of_strings->buffer[index - 1];
        rhym->buffer[*i + 1] = buffer_of_strings->buffer[index];
        is_this_string_in_poem[index - 1] = true;
        ++(*count_bool);
    }
    else if (count_one <= count_two && count_two > 0)
    {
        rhym->buffer[*i] = buffer_of_strings->buffer[index];
        rhym->buffer[*i + 1] = buffer_of_strings->buffer[index + 1];
        is_this_string_in_poem[index + 1] = true;
        ++(*count_bool);
    }
    else
    {
        if (*count_bool < buffer_of_strings->size)
        {
            *i -= 2;
        }
    }
}

Pointer Rhyming(Pointer* buffer_of_strings, int count_poem, int count_string_in_poem) 
{
    ++count_string_in_poem; // string ""
    size_t count_one = 0;
    size_t count_two = 0;
    size_t index = 0;
    size_t count_string = count_poem * count_string_in_poem;

    if (count_poem < 0 || (count_string_in_poem - 1) * count_poem > buffer_of_strings->size) 
    {
        printf("%s", "Cannot do this");
        exit(1);
    }

    bool* is_this_string_in_poem = calloc(buffer_of_strings->size, sizeof(true));
    size_t count_bool = 0;
    Pointer rhym = {calloc(count_string, sizeof(char*)), count_string};

    for (int j = 0; j < count_poem; ++j)
    {
        if (j > 0) 
        {
            rhym.buffer[j * count_string_in_poem - 1] = "\n";
        }
        for (int i = j * count_string_in_poem; i < j * count_string_in_poem + count_string_in_poem - 2; i += 2) 
        {
            index = Random(buffer_of_strings, is_this_string_in_poem);
            SelectingLinesForRhyme(is_this_string_in_poem, index, buffer_of_strings, &rhym, &count_bool, &i);
        }
    }
free(is_this_string_in_poem);
return rhym;
}

Pointer GetArrayOfStringPointers(Str *file, size_t count_str) 
{
    Replace(file, '\n', '\0');
    char** pointers = (char**) calloc(count_str + 1, sizeof(char*));

    pointers[0] = &(file->buffer[0]);
    int j = 1;
    for (int i = 1; i < file->size; ++i) 
    {
        if (file->buffer[i] == '\0')
        {
            ++i;
            pointers[j] = &(file->buffer[i]);
            ++j;

        }
    }
    Pointer buffer_of_strings = {pointers, --j};

    return buffer_of_strings;
}

size_t GetSize(FILE* name) 
{
    fseek(name, 0, SEEK_END);
    size_t file_size = ftell(name);
    fseek(name, 0, SEEK_SET);
    return file_size;
}

Str ReadFile(const char* name) 
{
    FILE* in = fopen(name, "r");
    if (NULL == in)
    {
        perror("Cannot open the file");
        exit(-1);
    }

    size_t file_size = GetSize(in);
    char* buffer = (char*)calloc(file_size, sizeof(char)); 
    fread(buffer, sizeof(char), file_size, in); 
    
    if (ferror(in))
    {
        perror("Cannot read file");
        exit(1); 
    }

    Str string = {buffer, file_size};
    
    int close = fclose(in); 
    if (close == -1)
    {
        perror("Cannot close the file");
        exit(-1);
    }
    
    return string;
}

void Write(const char* name, Pointer* buffer_of_string) {
    FILE* out = fopen(name, "w");
    if (NULL == out)
    {
        perror("Cannot open the file");
        exit(-1); 
    }

    for (int i = 0; buffer_of_string->buffer[i] != NULL; ++i)
    {
        int fp = fprintf(out, "%s\n", buffer_of_string->buffer[i]); 
        if (fp < 0)
        {
            perror("Cannot write file");
            exit(1);
        }
    }

    int close = fclose(out); 
    if (close == -1)
    {
        perror("Cannot close the file");
        exit(1);
    }
} 

void Clear(Str* file, Pointer* pointers, Pointer* rhym)
{
    free (file->buffer);
    free(pointers->buffer);
    free(rhym->buffer);
}
    
int main() {
    int number_of_stanzas = 0;
    int number_of_lines = 0;

    srand(time(NULL));

    printf("%s", "Enter the number of stanzas: ");
    scanf("%d", &number_of_stanzas);
    printf("%s", "Enter the number of lines in the stanza: ");
    scanf("%d", &number_of_lines);

    Str file = ReadFile("my file");
    size_t count_str = CountSymbol(&file, '\n');
    Pointer buffer_of_string = GetArrayOfStringPointers(&file, count_str);
    qsort((void*)buffer_of_string.buffer, count_str, sizeof(char*), CompareBackwards); 
    Pointer rhym = Rhyming(&buffer_of_string, number_of_stanzas, number_of_lines);
    Write("out file", &buffer_of_string);
    Write("rhyming", &rhym);

    Clear(&file, &buffer_of_string, &rhym);
}