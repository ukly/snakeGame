
#ifndef __STAGE__
#define __STAGE__
#include <ncursesw/curses.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <cstring>
#include "Object.h"
#define UP KEY_UP
#define DOWN KEY_DOWN
#define RIGHT KEY_RIGHT
#define LEFT KEY_LEFT
#define ENTER 10
#define ESC 27
#define PAUSE 112
#define RESUME 114
#define SKIP 115
#define EMPTY 0
#define WALL 1
#define IMMUNE_WALL 2
#define SNAKE_HEAD 3
#define SNAKE_BODY 4
#define GROWTH_ITEM 5
#define POISON_ITEM 6
#define GATE 7

using namespace std;

class Stage
{
public:
    Stage();
    ~Stage();
    void terminal();
    string menu();
    void play();
    void help();
    void option();

    int getStageNum() const { return STAGE_NUM; }
    int getMapRow() const { return MAP_ROW; }
    int getMapCol() const { return MAP_COL; }


    void setMap();
    void copyMap(int nStage);
    void drawMap();

    void appearItem();
    void appearGate();
    void disappearItem();
    void disappearGate();

    void makeSnake();
    void moveSnake();
    void enterGate(Object *head);
    int findRoot(Object *gate);
    void eatItem(int item);

    void setMission();
    bool completeMission();
    bool checkGameOver() { return finish; }
    void gameOver();
    void alert(int posY, int posX, const string msg, bool stopFlag);

private:
    WINDOW *game, *score, *mission, *info;
    WINDOW *manual, *description, *scrollBar;
    int condition[5];

    char checkedArray[4];
    int missionArray[4];
    const int SNAKE_MAX_LENGTH = 10;
    Object *snakeObject;
    Object *gate1;
    Object *gate2;

    const int STAGE_NUM = 5,
              MAP_ROW = 25,
              MAP_COL = 50,
              ROW_END = MAP_ROW - 1,
              COL_END = MAP_COL - 1;
    const string itemIndex = "  X0O+- ";

    int ***stage, **map;

    int dir, y, x,
        menuLastSelected, optLastSelected,
        level, speed, tcount,
        timeoutMs, msTime;
    const int speedArray[5] = {500, 400, 300, 200, 100},
              msArray[5] = {2, 3, 5, 8, 10};

    string manualTitle, menuTitle, shorTitle;
    string menuTxt[4], shorTxt[9];
    int Ysize, Xsize,
        Ystart, Xstart;
    int desYsize, desXsize,
        desYstart, desXstart;
    int txtLines, hidTxtLen;

    vector<string> manualTxt;

    vector<pair<int, int> > itemPos, gatePos;
    bool chkEnter, finish;
};
#endif