#pragma once

//9방향
enum PlayerDir
{
    DirR,
    DirU,
    DirL,
    DirD,
    DirRD,
    DirLD,
    DirLU,
    DirRU,
    DirN
};


class Player
{
private:
    Image* img;
    Image* img2;
    Vector2 pos;
    int    PlDir;
    Vector2 dir;
    float Seta45;

    //플레이어의 발을중심으로 한 4개점
    Vector2 foot;
public:

    HRESULT init();
    void release();
    void update();
    void render();
    //캡슐화 public과 다를것이 없다.
    MAKEREFGET(Vector2 ,pos)
    MAKEREFGET(Vector2 ,dir)
    MAKEREFGET(Vector2 ,foot)
    //MAKEREFGET(int  , PlDir)
};

