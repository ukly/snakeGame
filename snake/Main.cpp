#include <iostream>
using namespace std;

#include "Stage.h"

int main()
{
    setlocale(LC_ALL, "ko_KR.utf8");
    Stage stage = Stage();
    string game;
    while(true)
    {
        game = stage.menu();
        clear();
        if(game == "시작하기")
            stage.play();
        else if(game == "설명")
            stage.help();
        else if(game == "옵션")
            stage.option();
        else
            break;
    }
}