#include "Headers.h"
#include "Username.h"
#include "curdir.h"

void Username()
{
    char Username[1000];
    struct passwd *p = getpwuid(getuid());
    
    gethostname(Username,1000);
    printf("<");
    printf("\033[0;35m"); 
    printf("%s@%s", p->pw_name, Username);
    printf(":");
    printf("\033[0m");
}
