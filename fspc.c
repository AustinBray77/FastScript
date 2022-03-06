#include "FSPC_STD/std.h"
#include "FSPC_STD/fspc_io.h"
#include "FSPC_STD/fspc_algorithms.h"
#include <Windows.h>
#include <WinBase.h>

#pragma region Compiler_Functions
const char semicolon = ';';
char *cwd = NULL;

int check_expression(char *exp, int len)
{
    if (exp[0] == '#')
    {
        return 1;
    }

    return 0;
}

void generate_file(char *outputName, char *includes, char *fSegment)
{
    FILE *fp;
    fp = fopen(outputName, "w");

    fputs(includes, fp);
    fputs(fSegment, fp);
    fputs("\nreturn 0;\n}\n", fp);

    fclose(fp);
}

void setcwd()
{
    int len = GetCurrentDirectory(0, NULL);

    cwd = salloc((len + 1) * sizeof(char));
    GetCurrentDirectory(len, cwd);
    cwd[len - 1] = '\\';
    cwd[len] = '\0';
}

char *format_filepath(char *input)
{
    char *output = concatstr(cwd, input);

    // free(cwd);

    return output;
}

void end_compile(int errorCode)
{
    if (errorCode == -1)
    {
        printf("Build Compiled With Error(s)!");
    }
    else
    {
        printf("Build Compiled Successfully!");
    }

    exit(errorCode);
}

#pragma endregion

int main(int argc, char *args[])
{
    setcwd();

    if (argc == 1)
    {
        printf("Not enough arguments passed...");
        end_compile(-1);
    }

    char *inputName, *outputName;

    if (argc >= 2)
    {
        inputName = args[1];
        outputName = args[1];
    }

    if (argc >= 3)
    {
        outputName = args[2];
    }

    inputName = format_filepath(inputName);
    outputName = format_filepath(outputName);

    char *includes = "#include \"fspc_std.h\"\n";
    char *fSegment = "\nint main() {\n";

    int includesSize = 27, fSegmentSize = 14;

    FILE *file;

    file = fopen(inputName, "r");

    if (file == NULL)
    {
        printf("Unable to open file...");
        exit(0);
    }

    int len;
    char *data = fgetall(file, &len);
    char *subs;

    for (int i = 0, x = 0; i < len; i++)
    {
        if (data[i] == ';')
        {
            int subLen = i - x;

            subs = substr(data, len, x, &subLen);
            int type = check_expression(subs, subLen);

            switch (type)
            {
            case 0:
                fSegment = concatstr(fSegment, concatstr(subs, ";"));
                break;
            case 1:
                includes = concatstr(includes, subs);
                break;
            }

            if (subLen > 0)
            {
                free(subs);
            }

            x = i + 1;
        }
    }

    fclose(file);

    generate_file(concatstr(outputName, ".c"), includes, fSegment);

    char *command = concatstr(concatstr("gcc -fdiagnostics-color=always -g ", concatstr(outputName, ".c")), concatstr(" -o ", concatstr(outputName, ".exe")));

    command = concatstr(command, concatstr(" -I ", concatstr(cwd, "\\FSPC_STD")));

    end_compile(system(command));

    return 0;
}