#ifndef FSPCIO_IMPORT
#define FSPCIO_IMPORT 1

#define MAX_INPUT_LEN 10000

#include "std.h"
#include "fspc_std.h"
#include "fspc_algorithms.h"

#pragma region Output
void print_str(char *s)
{
    printf("%s", s);
}

void print_int(int i)
{
    printf("%i", i);
}

void print_double(double d)
{
    printf("%d", d);
}

void print_bool(bool b)
{
    print_str(b ? "true" : "false");
}
#pragma endregion

#pragma region Input
char *scan_str(int size)
{
    char *str = salloc(size * sizeof(char));
    scanf("%s", &str);
    return str;
}

int scan_int()
{
    int val;
    scanf("%i", &val);

    return val;
}

double scan_double()
{
    double val;
    scanf("%lf", &val);
    return val;
}

bool scan_bool()
{
    return lowerstr(scan_str(5)) == "true" ? true : false;
}

char *get_line(char *output)
{
    int i = 0;
    char input[MAX_INPUT_LEN];

    char in;
    scanf("%c", &in);

    while (in != '\n' && i < MAX_INPUT_LEN)
    {
        input[i] = in;
        i++;
        scanf("%c", &in);
    }

    return substr(input, MAX_INPUT_LEN, 0, &i);
}

char *fgetall(FILE *file, int *len)
{
    fseek(file, 0L, SEEK_END);
    size_t sz = ftell(file);
    rewind(file);

    char *input = (char *)malloc(sz);

    int i = 0;
    for (i = 0; i < sz / sizeof(char); i++)
    {
        fscanf(file, "%c", &input[i]);
    }

    *len = i;

    return input;
}
#pragma endregion

#endif