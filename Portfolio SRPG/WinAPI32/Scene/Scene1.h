#pragma once
#define MAXPOSITION 5000.0f
#define MAXSCALE 500.0f

//���
class Scene1 :public Scene
{
private:
	//�����Ͽ� �ٲ��� Ÿ�ϸ�
    TileMap* bg;

    //Ÿ�ϸ����� �� ����̹���
    Image*   bg2;

	//�����Ϳ��� �� �������

	//���° �̹��� ����?
    int vecIdx;
	//�̹��� ���� �ε���
    int ImgIdxX;
    int ImgIdxY;

    //Ÿ�� ����
    int MaxX;
    int MaxY;

    //���콺 ��ŷ�Ҷ� Ÿ�� ����
    int TileState;

	//Ÿ�� ������
	Vector2 Scale;
	//Ÿ���� �����̵Ǵ� ���� �Ʒ��� ��ġ
	Vector2         LB;

    //Fade
    Color Fade;
    ID3D11Buffer*		FadeBuffer;    

	//Ÿ�ϸ� ������ ��ư �Լ�

	//Ÿ�ϸ� �̹��� ��ư
	void TileImgButton();
	//���� �̹��� �ε��� ��ư
	void VecIdxButton();
	//�������� ��ư
	void ResizeButton();

	//���̺� �ε� ��ư
	void TileMapSaveLoad();
	
   
public:

    Scene1();
    ~Scene1();
    HRESULT init()override;		//�����ǵ� �Լ�
    void release()override;		
    void update()override;		
    void render()override;		
    //���� ��� ����
    static int LoadCount;
    
};

