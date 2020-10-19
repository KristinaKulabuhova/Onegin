#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#define MIN(X, Y) X < Y ? X : Y

typedef struct Str
{
    char* buffer;
    size_t size;
} Str;

void Replace(Str *file, const char from, const char to)
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

size_t CountSymbol(Str *file, const char to)
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


int Compare(const void* p_first,const void* p_second) {
    char** str_first = (char**)p_first;
    char** str_second = (char**)p_second;
    return strcmp(*str_first, *str_second);
}

int CompareBackwards(const void* p_first,const void* p_second)
{
    char* str_first = *((char**) p_first);
    char* str_second = *((char**) p_second);
    size_t first_size = strlen(str_first);
    size_t second_size = strlen(str_second);
    for (int i = 1, j = 1; i < first_size && j < second_size; ++i, ++j)
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
    return 0;
}

char** GetArrayOfStringPointers(Str *file, size_t count_str) 
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
    pointers[j] = NULL;

    return pointers;
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
        exit(-1); 
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

void Write(Str* file, char name[], char** pointers) {
    FILE* out = fopen(name, "w");
    if (NULL == out)
    {
        perror("Cannot open the file");
        exit(-1); 
    }

    for (int i = 0; NULL != pointers[i]; ++i)
    {
        int fp = fprintf(out, "%s\n", pointers[i]); 
        if (fp < 0)
        {
            perror("Cannot write file");
            exit(-1);
        }
    }

    int close = fclose(out); 
    if (close == -1)
    {
        perror("Cannot close the file");
        exit(-1);
    }
} 

void Clear(Str* file, char** pointers)
{
    free (file->buffer);
    free(pointers);
}
    
int main() {
    Str file = ReadFile("my file");
    size_t count_str = CountSymbol(&file, '\n');
    char** pointers = GetArrayOfStringPointers(&file, count_str);
    qsort((void*)pointers, count_str, sizeof(char*), CompareBackwards); 
    Write(&file, "out file", pointers);
    Clear(&file, pointers);
}   

