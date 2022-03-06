#ifndef FSPCALGOS_IMPORT
#define FSPCALGOS_IMPORT 1

#include "std.h"
#include <ctype.h>

char *concatstr(char *a, char *b);
char *salloc(size_t size);
char *substr(char *str, int strLen, int startIndex, int *len);

char *lowerstr(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        str[i] = tolower(str[i]);
    }

    return str;
}

char *concatstr(char *a, char *b)
{
    size_t totalSize = (strlen(a) + strlen(b)) * sizeof(char) + 1;
    char *output = salloc(totalSize);

    memcpy(&output[0], a, (strlen(a)) * sizeof(char));
    memcpy(&output[strlen(a)], b, strlen(b) * sizeof(char));

    output[strlen(a) + strlen(b)] = '\0';

    return output;
}

char *salloc(size_t size)
{
    char *output;
    output = malloc(size);

    if (output == NULL)
    {
        printf("Unable to allocate string...");
        exit(-1);
    }

    return output;
}

char *substr(char *str, int strLen, int startIndex, int *len)
{
    if (strLen - startIndex < *len)
    {
        *len = strLen - startIndex;
    }

    if (len == 0)
    {
        return "";
    }

    char *output = salloc((*len + 1) * sizeof(char));

    memcpy(output, &str[startIndex], *len * sizeof(char));
    output[*len] = '\0';

    return output;
}

#endif