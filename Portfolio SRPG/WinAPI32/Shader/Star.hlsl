//hlsl �� ���̴� ���

//���̴��� �׷���ī���� ���ɿ� ������ �޴´�(Ư�� �ȼ����̴�����)

//���̴��� ������ gpu�� �ϰ�
//�޸𸮸� �׷���ī�� �޸𸮿� �Ҵ�

//gpu�� ���� ������ �ϴ�������
//cpu���� ������ ������

//??? �� �𵨸�ŭ�̳� �޸𸮸� �����ϴ°�
//�ȼ����̴��� �޸𸮸� ��û �Ա⶧����



//���̴� ��°� �Լ���.


//hlsl
//�ø�ƽ
//�������



//�������
cbuffer VS_W : register(b0)
{
    //���켱 ���
    row_major matrix World;
}
cbuffer VS_V : register(b1)
{
    row_major matrix View;
}
cbuffer VS_P : register(b2)
{
    row_major matrix Proj;
}
cbuffer PS_Fade : register(b1)
{
    float4 Fade;
}

struct VertexInput
{
    float4 Position : POSITION0;
    //float4 Color : Color0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
};


//�ȼ����̴����� �� ������� �÷�
cbuffer PS_Color : register(b0)
{
    float4 Color;
}



//���ؽ� ���̴�
//��ȯ��  �Լ��̸�(�Ű�����)

// VertexInput(in) ->  VS  -> PixelInput (out)
PixelInput VS(VertexInput input)
{
    PixelInput output;
    //����->���� mul���ϱ�
    output.Position = mul(input.Position, World);
    
    //����->��
    output.Position = mul(output.Position, View);
    
    //��->��������
    output.Position = mul(output.Position, Proj);
    
    //L -> W -> V -> P ������ȯ

    return output; //���ϵǴ� �ܰ������ ���������ʹ�.
}
//�ȼ� ���̴�
float4 PS(PixelInput input) : SV_TARGET
{
    return Color * Fade; //r g b a
}