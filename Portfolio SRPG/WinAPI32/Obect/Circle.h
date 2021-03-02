#pragma once
//원 만들어서 그려오기 삼각형을 여러개 이어서
class Circle : public Object
{
    //무수히 많은 삼각형이 필요
    //? 몇개
    ID3D11Buffer*		VertexBuffer; 
    UINT				vertexCount;

    UINT                Slice;  //몇조각 내서 그릴건지
    //원을 이루는 정점
    VertexPC*           Vertex;
    bool                _isFill;
    
public:
    COL_CIRCLE          Col;
    

    HRESULT init(UINT slice,bool isFill = true);		//생성시 호출
    void release()override;		//소멸시 호출
    void update()override;		//갱신되는값이 있을때 호출
    void render()override;		//그려야 되는값이 있을때 호출

};

