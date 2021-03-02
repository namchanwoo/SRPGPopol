#pragma once

enum BossState
{
    bNONE,
    bLOOK,
    bFOLLOW,
    bATTACK 
};

//9πÊ«‚
enum BossDir
{
    bDirR,
    bDirL,
    bDirRD,
    bDirLU,
    bDirU,
    bDirD,
    bDirLD,
    bDirRU,
    bDirN
};
class Enemy
{

private:
    Image* img;
    Vector2 pos;
    int    PlDir;
    int    BossState;
    Vector2 dir;
    float Seta45;
    float Seta225;
    Player* pl = nullptr;
    float seta;
    float time= 0.0f;
public:

    HRESULT init();
    void release();
    void update();
    void render();
    MAKESET(Player*, pl)
};

