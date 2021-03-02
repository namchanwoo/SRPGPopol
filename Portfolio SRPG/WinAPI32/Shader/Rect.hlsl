
//�������
//����ü �� ���

//��� -> �������ʴ� ��
//�������� ���� �����ϴ� ���� ���� ��� ���
//�Ϲݻ������ b �ؽ��� t ���÷� s
//16����Ʈ �����θ� ũ�⸦ ������ִ�.
cbuffer VS_W : register(b0) //register(b0) �̰� ��� b0��ġ
{
    //���켱 ���
    //��� ���� ����� �ƴ϶�
    //4x4 -> 16���� 8����Ʈ
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

//�ȼ����̴����� �� ������� �÷�
cbuffer PS_Color : register(b0)
{
    //4 ���� 8����Ʈ
    float4 Color;
}
cbuffer PS_Fade : register(b1)
{
    float4 Fade;
}


//POSITION0 -> �ø�ƽ [n] n �ε���
//gpu�� ������ü�� �������� 
//������ ��ȯ���� �뵵������ ���� ������ �ʿ��ϴ�
struct VertexInput
{
    float4 Position : POSITION0;
    //float4 Position2 : POSITION1;
    float4 Color : Color0;
};

struct PixelInput
{
    float4 Position : SV_POSITION; //ȭ����ǥ�� ������
    float4 Color : Color0;
};

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
    
    //���� 6���� ���� �Ѱ���
    output.Color = input.Color;

    return output; //���ϵǴ� �ܰ������ ���������ʹ�.
}

//RS
//->SV_POSITION �� ȭ����ǥ��� ��ȯ





//�ȼ� ���̴�
float4 PS(PixelInput input) : SV_TARGET //SV_TARGET �� Ÿ���̵� ���� 
{
    //�׷� �߰��� ���ܳ� �ȼ��� ��� ���� �������°�?
    
    //���� ������ �� ���հ��� ��������
    
    //�ȼ�����ŭ �þ
    
    
    //�����Ͷ������� ��ģ ��ũ�� ��ǥ
    //�ȼ��� ��ǥ�� 300�̻��̸�
    //if (input.Position.y > 500)
    //{
    //    return float4(1, 0, 0, 1);
    //}
    //else if (input.Position.y > 400)
    //{
    //    return float4(1, 1, 0, 1);
    //}
    //else if (input.Position.y > 300)
    //{
    //    return float4(1, 0, 1, 1);
    //}
    
    return Color * Fade; //r g b a
}