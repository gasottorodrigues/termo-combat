#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../include/constants.h"
#include "../include/graphics.h"

int main(int argc, char *argv[])
{
    int cmd;
    int inExec = TRUE;

    while (inExec)
    {
        showMainMenu();
        scanf("%d", &cmd);

        switch (cmd)
        {
        case CMD_EXIT:
            inExec = FALSE;
            break;
        case CMD_HOST:
            break;

        case CMD_JOIN:
            break;

        case CMD_TUT:
            break;

        default:
            break;
        }
    }
}