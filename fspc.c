#include "FSPC_STD/std.h"
#include "FSPC_STD/io_fspc.h"
#include "FSPC_STD/algorithms_fspc.h"
#include <Windows.h>
#include <WinBase.h>

#pragma region Compiler_Functions
const char semicolon = ';';
char *cwd = NULL;

int check_expression(char *exp, int len)
{
    int ulen = 6;
    if (exp[0] == '#')
    {
        return 1;
    }
    else if (!strcmp(substr(exp, len, 0, &ulen), "using "))
    {
        return 2;
    }

    return 0;
}

void generate_file(char *outputName, char *includes, char *fSegment)
{
    FILE *fp;
    fp = fopen(outputName, "w");

    fputs(includes, fp);
    fputs(fSegment, fp);

    if (outputName[strlen(outputName) - 1] == 'c')
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

void compile_file(char *dir, char *fileName, char *outputName, int fType)
{
    char *includes = "#include \"std_fspc.h\"\n";
    char *fSegment = fType == 0 ? "\nint main() {\n" : "";

    FILE *file;

    printf("Opening File %s...\n", concatstr(dir, fileName));

    file = fopen(format_filepath(concatstr(dir, fileName)), "r");

    if (file == NULL)
    {
        printf("Unable to open file %s...\n", format_filepath(concatstr(dir, fileName)));
        end_compile(-1);
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

            for (int j = 0, y = 0; j < subLen - y; j++)
            {
                if (subs[j] == '\n')
                {
                    int frontLen = j;
                    int backLen = i - j - 1;
                    subs = concatstr(substr(subs, subLen - y, 0, &frontLen), substr(subs, subLen - y, j + 1, &backLen));
                }
            }

            int type = check_expression(subs, subLen);

            switch (type)
            {
            case 0:
                fSegment = concatstr(fSegment, concatstr(subs, ";\n"));
                break;
            case 1:
                includes = concatstr(includes, concatstr(subs, "\n"));
                break;
            case 2:
                int usingLen = subLen - 6;
                char *hName = substr(subs, len, 6, &usingLen);
                compile_file(dir, concatstr(hName, ".fsh"), hName, 1);
                includes = concatstr(includes, concatstr("#include \"./", concatstr(hName, "_fspc.h\"\n")));
                free(hName);
                break;
            }

            free(subs);
            x = i + 1;
        }
    }

    fclose(file);

    switch (fType)
    {
    case 0:
        outputName = concatstr(concatstr(dir, outputName), ".c");
        break;
    case 1:
        outputName = concatstr(concatstr(dir, outputName), "_fspc.h");
        break;
    }

    generate_file(format_filepath(outputName), includes, fSegment);

    free(data);
}

#pragma endregion

int main(int argc, char *args[])
{
    setcwd();

    char *dir, *inputName, *outputName;

    if (argc < 4)
    {
        printf("Not enough arguments passed...");
        end_compile(-1);
    }
    else
    {
        dir = args[1];
        inputName = args[2];
        outputName = args[3];
    }

    compile_file(dir, inputName, outputName, 0);

    char *endFile = format_filepath(concatstr(dir, outputName));

    char *command = concatstr(concatstr("gcc -fdiagnostics-color=always -g ", concatstr(endFile, ".c")), concatstr(" -o ", concatstr(endFile, ".exe")));

    command = concatstr(command, concatstr(" -I ", concatstr(cwd, "\\FSPC_STD")));

    end_compile(system(command));

    return 0;
}