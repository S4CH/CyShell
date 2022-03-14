#include "Headers.h"
#include "echo.h"

void echo(char **input)
{
    int i=1;
    while(input[i]!=NULL)
    {
        printf("%s ", input[i]);
    }
    printf("\n");
}

void echo_str(char* input)
{
    //char filtered[1000];
    //strcpy(filtered, input);

    char*filtered = strtok(input, ">");

    const char del[] =" \t";
    char*tok;
    tok = strtok(filtered,del);

    while(tok!=NULL)
    {
        printf("%s ",tok);
        tok = strtok(0,del);
    }
    printf("\n");
}
