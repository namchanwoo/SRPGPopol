#pragma once

enum AnimState
{
    ANI_STOP,		//����
    ANI_LOOP,		//���
    ANI_REVERSE		//���������
};


//�ػ󵵴� �����ȼ��� �����ȼ���
//���÷��̿����� ũ�Ⱑ �� �ٸ���.

//�׸����ϵ� �ػ󵵷� �Ǿ��ִ�.
//�׸����ϰ�� ũ�Ⱑ �� �������̹Ƿ�
//��ֶ������ Uv��ǥ�� ������ �̹�����ǥ�� ����ش�.

class Image:public Object
{
    //���ؽ�(����,������) ����
    //���� �������̽� ���۴� �Ϲ������� ���� �Ǵ� �ε��� �����͸� �����մϴ�.
    ID3D11Buffer*		VertexBuffer; 
    UINT				vertexCount;
    VertexPT*           Vertex;

   

    //���÷�
    ID3D11SamplerState*			Sampler;
   
    //�߰� ���������� �յڻ��� �����ٳ�
    ID3D11BlendState*       blendState[2];

   
    ID3D11Buffer*		        UVBuffer;

public:
    D3D11_SAMPLER_DESC			SamplerDesc;
    Vector4				        UV;     //�����ϰ���� uv��
                                        //���̴����� ������ �ڿ���
    ID3D11ShaderResourceView*	srv;

    //���ҵ� ����
    UINT				MaxFrameX = 1;
    UINT				MaxFrameY = 1;

    //���� �ε���
    UINT				CurrentFrameX = 0;
    UINT				CurrentFrameY = 0;
    //�ִϸ��̼� ����� �ʿ��� �ð�����
    float				time = 0.0f;



    HRESULT init(_tstring file,   string vs = "VS", string ps = "PS");		//������ ȣ��
    void release()override;		//�Ҹ�� ȣ��
    void update()override;		//���ŵǴ°��� ������ ȣ��
    void render()override;		//�׷��� �Ǵ°��� ������ ȣ��
    void CreateSampler();
    void PlayAnimation(AnimState as, bool isFrameX, float nexttime);
};

