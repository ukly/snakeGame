#include "Stage.h"

Stage::Stage()
{
    srand((unsigned)time(0));
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    start_color();

    menuLastSelected = 0,
    speed = 4,
    optLastSelected = speed - 1,
    tcount = 0;
    manualTitle = "< 사용자 매뉴얼 >",
    menuTitle = "< 플레이 방법 >";
    menuTxt[0] = " - 위쪽 키(^): 위쪽으로 이동",
    menuTxt[1] = " - 아래쪽 키(v): 아래쪽으로 이동",
    menuTxt[2] = " - 왼쪽 키(<): 왼쪽으로 이동",
    menuTxt[3] = " - 오른쪽 키(>): 오른쪽으로 이동",
    menuTxt[4] = " - 'p': 게임 일시중지",
    menuTxt[5] = " - 'r': 게임 재개",
    menuTxt[6] = " - 'esc': 메인 메뉴로 돌아가기";
    
    init_pair(EMPTY, COLOR_WHITE, COLOR_BLACK);          
    init_pair(WALL, COLOR_BLACK, COLOR_WHITE);           
    init_pair(IMMUNE_WALL, COLOR_BLACK, COLOR_WHITE);    
    init_pair(SNAKE_HEAD, COLOR_BLUE, COLOR_BLUE);     
    init_pair(SNAKE_BODY, COLOR_GREEN, COLOR_GREEN);   
    init_pair(GROWTH_ITEM, COLOR_WHITE, COLOR_BLUE);  
    init_pair(POISON_ITEM, COLOR_WHITE, COLOR_RED);    
    init_pair(GATE, COLOR_CYAN, COLOR_CYAN);            
    init_pair(10, COLOR_WHITE, COLOR_BLACK);           
    init_pair(11, COLOR_BLACK, COLOR_BLUE);            
    init_pair(12, COLOR_BLUE, COLOR_BLACK);             
}

Stage::~Stage()
{
    delwin(description);
    delwin(manual);
    delwin(info);
    delwin(mission);
    delwin(score);
    delwin(game);
    endwin();
}

void Stage::terminal()
{

    cout << "\e[3;240;120t";
    cout << "\e[8;40;120t";
    system("resize -s 40 120");
    y = 40, x = 120;
    Ysize = y / 1.5,
    Xsize = x / 1.5,
    Ystart = y / 2 - Ysize / 2,
    Xstart = x / 2 - Xsize / 2,
    desYsize = Ysize - 6,
    desXsize = Xsize - 6,
    desYstart = Ystart + 3,
    desXstart = Xstart + 3,
    txtLines = 30,
    hidTxtLen = txtLines - desYsize > 0 ? txtLines - desYsize : 0;
}

string Stage::menu()
{
    clear();
    terminal();
    curs_set(0);
    string txt[5];
    txt[0] = "스네이크 게임";
    int focus = menuLastSelected;
    level = 0;
    while (1)
    {
        if (!focus)
            focus = 300;
        txt[1] = "시작하기";
        txt[2] = "설명";
        txt[3] = "옵션";
        txt[4] = "나가기";
        attron(COLOR_PAIR(10));
        mvprintw(y / 2, x / 2 - txt[0].length() / 2, txt[0].c_str());
        attroff(COLOR_PAIR(10));
        for (int i = 1; i <= sizeof(txt) / sizeof(txt[0]); i++)
        {
            if (i == abs(focus % 4 + 1))
            {
                attron(COLOR_PAIR(11));
                mvprintw(y / 2 + 3 * i, x / 2 - (txt[i].length() / 2), txt[i].c_str());
                attroff(COLOR_PAIR(11));
            }
            else
                mvprintw(y / 2 + 3 * i, x / 2 - (txt[i].length() / 2) - 1, txt[i].c_str());
        }
        switch (getch())
        {
        case UP:
            focus--;
            break;
        case DOWN:
            focus++;
            break;
        case ENTER:
            menuLastSelected = focus;
            return txt[abs(focus % 4 + 1)];
        }
    }
    return NULL;
}

void Stage::play()
{
    terminal();
    setMap();
    int n;
    timeoutMs = speedArray[speed - 1];
    for (int i = 0; i < STAGE_NUM; i++)
    {
        msTime = n = 0;
        dir = LEFT;
        copyMap(i);
        setMission();
        makeSnake(); 
        appearGate();

        drawMap();
        while (1)
        {
            switch (getch())
            {
            case LEFT:
                if (dir != RIGHT)
                    dir = LEFT;
                else gameOver();
                break;
            case UP:
                if (dir != DOWN)
                    dir = UP;
                else gameOver();
                break;
            case RIGHT:
                if (dir != LEFT)
                    dir = RIGHT;
                else gameOver();
                break;
            case DOWN:
                if (dir != UP)
                    dir = DOWN;
                else gameOver();
                break;
            case PAUSE:
                alert(y / 2 - 4, x / 2 - 34, "r을 눌러 게임을 재개하세요", TRUE);
                while (1)
                {
                    if (getch() == RESUME)
                        break;
                }
                break;
            case ESC:
                endwin();
                return;
            case SKIP:
                for(int i=0;i<4;i++){
                    condition[i] = missionArray[i];
                }
            }
            moveSnake();
            if (chkEnter) 
            {
                if (++n >= condition[0]) 
                {
                    disappearGate();
                    appearGate();
                    n = 0;
                    chkEnter = FALSE;
                }
            }
            if (++msTime % (msArray[speed - 1] * 5) == 0) 
            {
                disappearItem();
                appearItem();
            }
            if (condition[0] < 3) 
                gameOver();
            if (completeMission())
            {
                alert(y / 2 - 4, x / 2 - 40, "스테이지 클리어!", FALSE);
                break;
            }
            if (checkGameOver())
            {
                alert(y / 2 - 4, x / 2 - 25, "게임 오버!", FALSE);
                return;
            }
            drawMap();
            timeout(timeoutMs);
        }
        level++;
    }
    endwin();
}

void Stage::help()
{
    terminal();
    int ySize = 0;
    while (1)
    {
        manual = newwin(Ysize, Xsize, Ystart, Xstart);
        description = newwin(desYsize, desXsize, desYstart, desXstart);
        wattron(manual, COLOR_PAIR(10));
        box(manual, 0, 0);
        mvwprintw(manual, 0, Xsize / 2 - manualTitle.length() / 2, "%s", manualTitle.c_str());
        wattroff(manual, COLOR_PAIR(10));

        mvwprintw(description, 0 + ySize,
                  Xsize / 2 - menuTitle.length() / 2 - 3, "%s", menuTitle.c_str());
        for (int i = 0; i < 7; i++)
            mvwprintw(description, 2 + (i * 2) + ySize, Xsize / 2 - menuTxt[2].length() / 2 - 3, "%s", menuTxt[i].c_str());

        refresh();
        wrefresh(manual);
        wrefresh(description);
    RE:
        switch (getch())
        {
        case ESC:
            return;
        }
    }
}

void Stage::option()
{
    clear();
    terminal();
    string optTitle = "옵션";
    string txt[5];
    string optTxt[2];
    optTxt[0] = "속도";
    optTxt[1] = " 느리게 -- 빠르게 ";
    int focus = optLastSelected;
    level = 0;
    while (1)
    {
        if (!focus)
            focus = 500;
        for (int i = 0; i < sizeof(txt) / sizeof(txt[0]); i++)
            txt[i] = to_string(i + 1);

        attron(COLOR_PAIR(10));
        mvprintw(y / 2 - 2, x / 2 - optTitle.length() / 2 + 2, optTitle.c_str());
        attroff(COLOR_PAIR(10));
        for (int i = 0; i < sizeof(txt) / sizeof(txt[0]); i++)
        {
            if (i == abs(focus % 5))
            {
                attron(COLOR_PAIR(12));
                mvprintw(y / 2, x / 2 - sizeof(txt) / sizeof(txt[0]) + i * 3 - 1, txt[i].c_str());
                attroff(COLOR_PAIR(12));
            }
            else
                mvprintw(y / 2, x / 2 - sizeof(txt) / sizeof(txt[0]) + i * 3 - 1, txt[i].c_str());
        }

        mvprintw(y / 2 + 2, x / 2 - optTxt[1].length() / 2 + 4, optTxt[1].c_str());

        optLastSelected = focus;
        speed = atoi(txt[abs(focus % 5)].c_str());
        switch (getch())
        {
        case LEFT:
            focus--;
            break;
        case RIGHT:
            focus++;
            break;
        case ESC:
            return;
        }
    }
}

void Stage::setMap()
{
    int i, j, k;
    stage = new int **[STAGE_NUM];
    for (i = 0; i < STAGE_NUM; i++)
    {
        stage[i] = new int *[MAP_ROW];
        for (j = 0; j < MAP_ROW; j++)
        {
            stage[i][j] = new int[MAP_COL];
        }
    }

    for (i = 0; i < STAGE_NUM; i++)
    {
        for (j = 0; j < MAP_ROW; j++)
        {
            for (k = 0; k < MAP_COL; k++)
            {
                if (j==0 || k==0 || j == ROW_END || k == COL_END)
                    stage[i][j][k] = WALL;
                else
                    stage[i][j][k] = EMPTY;
            }
        }
        stage[i][0][0] = IMMUNE_WALL;
        stage[i][0][COL_END] = IMMUNE_WALL;
        stage[i][ROW_END][0] = IMMUNE_WALL;
        stage[i][ROW_END][COL_END] = IMMUNE_WALL;
        if (i == 1)
        {
            for (int z = 10; z < 40; z++){
                if(z <= 20){ continue; }
                stage[i][5][z] = WALL;
            }
        }
        if (i == 2)
        {
            for (int z = 10; z < 40; z++){
                if(z <= 20){ continue; }
                stage[i][10][z] = WALL;
            }
            for (int z = 10; z < 40; z++){
                if(z <= 20){ continue; }
                stage[i][15][z] = WALL;
            }
        }
        if (i == 3)
        {
            for (int z = 10; z < 40; z++){
                if(z <= 20){ continue; }
                stage[i][10][z] = WALL;
            }
            for (int z = 10; z < 40; z++){
                if(z <= 20){ continue; }
                stage[i][15][z] = WALL;
            }
            for (int z = 10; z < 40; z++){
                if(z <= 15){ continue; }
                stage[i][20][z] = WALL;
            }
        }
        if (i == 4)
        {
            for (int z = 10; z < 40; z++){
                if(z <= 11){ continue; }
                stage[i][10][z] = WALL;
            }
            for (int z = 10; z < 40; z++){
                if(z >= 39){ continue; }
                stage[i][15][z] = WALL;
            }
            for (int z = 10; z < 40; z++){
                if(z <= 11){ continue; }
                stage[i][20][z] = WALL;
            }
            for (int z = 10; z < 40; z++){
                if(z >= 39){ continue; }
                stage[i][5][z] = WALL;
            }
        }
    }
}

void Stage::copyMap(int nStage)
{
    map = new int *[MAP_ROW];
    for (int i = 0; i < MAP_COL; i++)
        map[i] = new int[MAP_COL];
    for (int i = 0; i < MAP_ROW; i++)
    {
        for (int j = 0; j < MAP_COL; j++)
            map[i][j] = stage[nStage][i][j];
    }
}

void Stage::drawMap()
{
    game = newwin(MAP_ROW, MAP_COL,
                  y / 2 - MAP_ROW / 2 , x / 2 - (MAP_COL / 2 + 16));
    for (int i = 0; i < MAP_ROW; i++)
    {
        for (int j = 0; j < MAP_COL; j++)
        {
            int index = map[i][j];
            wattron(game, COLOR_PAIR(index));
            mvwaddch(game, i, j, itemIndex[index]);
            wattroff(game, COLOR_PAIR(index));
        }
        printw("\n");
    }

    score = newwin(16, 30, y / 2 - (MAP_ROW / 2 + 4), x / 2 + MAP_COL / 2 - 7.4);
    wattron(score, COLOR_PAIR(10));
    box(score, 0, 0);
    mvwprintw(score, 0, 10, "[ 점수 ]");
    wattroff(score, COLOR_PAIR(10));
    mvwprintw(score, 3, 5, "B: %d / %d", condition[0], SNAKE_MAX_LENGTH);
    mvwprintw(score, 6, 5, "+: %d", condition[1]);
    mvwprintw(score, 9, 5, "-: %d", condition[2]);
    mvwprintw(score, 12, 5, "G: %d", condition[3]);

    mission = newwin(16, 30, y / 2 - (MAP_ROW / 2 + 4) + 17, x / 2 + MAP_COL / 2 - 7.4);
    wattron(mission, COLOR_PAIR(10));
    box(mission, 0, 0);
    mvwprintw(mission, 0, 9, "[ 미션 ]");
    wattroff(mission, COLOR_PAIR(10));
    mvwprintw(mission, 3, 5, "B: %d ( %c )", missionArray[0], checkedArray[0]);
    mvwprintw(mission, 6, 5, "+: %d ( %c )", missionArray[1], checkedArray[1]);
    mvwprintw(mission, 9, 5, "-: %d ( %c )", missionArray[2], checkedArray[2]);
    mvwprintw(mission, 12, 5, "G: %d ( %c )", missionArray[3], checkedArray[3]);

    info = newwin(4, 15, y / 2 - (MAP_ROW / 2 + 4), x / 2 + MAP_COL / 2 - 47.4);
    mvwprintw(info, 0, 1, "스테이지 %d/%d", level + 1, STAGE_NUM);
    mvwprintw(info, 2, 3, "< %02d:%02d >", msTime / (msArray[speed - 1] * 60), (msTime / msArray[speed - 1]) % 60);

    refresh();
    wrefresh(info);
    wrefresh(game);
    wrefresh(score);
    wrefresh(mission);
}

void Stage::appearItem()
{
    int appearNum = rand() % 3 + 1;
    for (int i = 0; i < appearNum; i++)
    {
        int itemType = rand() % 2 + GROWTH_ITEM;
        if (checkedArray[2] == 'v')
            itemType = GROWTH_ITEM;
        else if (condition[0] >= SNAKE_MAX_LENGTH)
            itemType = POISON_ITEM;
        while (1)
        {
            int y = rand() % (MAP_ROW - 2) + 1;
            int x = rand() % (MAP_COL - 2) + 1;
            if (map[y][x] == EMPTY &&
                map[y][x - 1] != GATE && map[y][x + 1] != GATE &&
                map[y + 1][x] != GATE && map[y - 1][x] != GATE)
            {
                map[y][x] = itemType;
                itemPos.push_back(make_pair(y, x));
                break;
            }
        }
    }
}

void Stage::appearGate()
{
    int n, y, x;
    for (int i = 0; i < 2; i++)
    {
        while (1)
        {
            n = rand() % (!level ? 4 : 5);
            y = rand() % (MAP_ROW - (i?3:2)) + (i?2:1);
            x = rand() % (MAP_COL - (i?3:2)) + (i?2:1);
            switch (n)
            {
            case 0: 
                y = 0;
                break;
            case 1: 
                x = 0;
                break;
            case 2: 
                x = COL_END;
                break;
            case 3: 
                y = ROW_END;
                break;
            case 4: 
                while (1)
                {
                    x = rand() % 30 + 10;
                    y = rand() % 15 + 5;
                    if (map[y][x] == WALL)
                        break;
                }
            }
            if (map[y][x] == WALL)
            {
                map[y][x] = GATE;
                gatePos.push_back(make_pair(y, x));
                break;
            }
        }
        if (i == 0)
            gate1 = new Object(y, x, GATE);
        if (i == 1)
            gate2 = new Object(y, x, GATE);
    }
}
void Stage::disappearItem()
{
    for (auto item : itemPos)
    {
        if (map[item.first][item.second] == GROWTH_ITEM || map[item.first][item.second] == POISON_ITEM)
            map[item.first][item.second] = EMPTY;
    }
    itemPos.clear();
}

void Stage::disappearGate()
{
    for (auto gate : gatePos)
    {
        if(map[gate.first][gate.second] == GATE)
            map[gate.first][gate.second] = WALL;
    }
    gatePos.clear();
}