#include <stdio.h>
#include <string.h>
#define BUFF_LENGTH 256

int runAndGetCmdOutPut(const char * cmd, char* result)
{
    //return 0 for success and other values on failue.
    char buffer[BUFF_LENGTH];
    FILE *pipe = popen(cmd, "r");
    if (! pipe) return -1;
    while (!feof(pipe))
    {
        if(fgets(buffer, BUFF_LENGTH, pipe))
        {
            memcpy(result, buffer, strlen(buffer));
        }
    }
    pclose(pipe);
    return 0;
}