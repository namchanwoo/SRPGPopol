#pragma once

//9����
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

    //�÷��̾��� �����߽����� �� 4����
    Vector2 foot;
public:

    HRESULT init();
    void release();
    void update();
    void render();
    //ĸ��ȭ public�� �ٸ����� ����.
    MAKEREFGET(Vector2 ,pos)
    MAKEREFGET(Vector2 ,dir)
    MAKEREFGET(Vector2 ,foot)
    //MAKEREFGET(int  , PlDir)
};

