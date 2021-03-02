#pragma once
class Rect :public Object
{
    //버텍스(정점,꼭지점) 버퍼
    //버퍼 인터페이스 버퍼는 일반적으로 정점 또는 인덱스 데이터를 저장합니다.
    ID3D11Buffer*		VertexBuffer; 
    UINT				vertexCount;
    bool                _isFill;
    VertexPC*           Vertex;
public:

    COL_RECT            Col;


    HRESULT init(bool isFill = true,
        Vector2 pivot = Vector2(0,0));		//생성시 호출
    void release()override;		//소멸시 호출
    void update()override;		//갱신되는값이 있을때 호출
    void render()override;		//그려야 되는값이 있을때 호출


};

