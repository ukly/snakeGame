#include <iostream>
#include "Stage.h"
using namespace std;

void Stage::setMission()
{
    finish = chkEnter = FALSE;
    memset(condition, 0, sizeof(condition));
    memset(missionArray, 0, sizeof(missionArray));
    memset(checkedArray, ' ', sizeof(checkedArray));

    missionArray[0] = 6;
    missionArray[1] = 1;
    missionArray[2] = 1;
    missionArray[3] = 1;
}
void Stage::alert(int posY, int posX, const string msg, bool stopFlag)
{
    WINDOW *alert = newwin(7, msg.length() * 2, posY, posX);
    box(alert, 0, 0);
    wattron(alert, COLOR_PAIR(0));
    wbkgd(alert, COLOR_PAIR(2));
    mvwprintw(alert, 3, msg.length() / 2, msg.c_str());
    wrefresh(alert);
    if (!stopFlag)
        usleep(1750000);
}


bool Stage::completeMission()
{
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (condition[i] >= missionArray[i])
        {
            checkedArray[i] = 'v';
            count++;
        }
        else if (!i)
            checkedArray[i] = ' ';
    }
    if (count == 4)
        return TRUE;
    return FALSE;
}
void Stage::gameOver()
{
    finish = true;
}
