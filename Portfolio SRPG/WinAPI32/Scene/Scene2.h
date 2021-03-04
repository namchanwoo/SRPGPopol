#pragma once


//장면
class Scene2 :public Scene
{
private:
	//접근하여 바꿔줄 타일맵
	TileMapEdit* edit;


	//타일맵으로 쓸 배경이미지
	Image*   bg1;

	//에디터에서 쓸 멤버변수

	//몇번째 이미지 인지?
	int vecIdx;
	//이미지 분할 인덱스
	int ImgIdxX;
	int ImgIdxY;

	//타일 갯수
	int MaxX;
	int MaxY;

	//마우스 피킹할때 타일 상태
	int TileState;

	//타일 스케일
	Vector2 Scale;
	//타일의 기준이되는 왼쪽 아래점 위치
	Vector2         LB;

	//Fade
	Color Fade;
	ID3D11Buffer*		FadeBuffer;

	


public:

	Scene2();
	~Scene2();
	HRESULT init()override;		//재정의된 함수
	void release()override;
	void update()override;
	void render()override;
	

};

