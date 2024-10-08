#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../include/constants.h"
#include "../include/graphics.h"
#include "../include/gameEngine.h"

int main(int argc, char *argv[])
{
    int cmd;
    bool inExec = true;
    gameStateType *state;

    while (inExec)
    {
        gx_mainMenu();
        scanf("%d", &cmd);

        switch (cmd)
        {
        case CMD_EXIT:
            egn_endGame(state);
            inExec = false;
            break;
        case CMD_HOST:
            state = egn_setGameState();
            egn_startMatch(state);
            break;

        case CMD_JOIN:
            break;

        case CMD_TUT:
            break;

        default:
            break;
        }
    }

    gx_exit();
}