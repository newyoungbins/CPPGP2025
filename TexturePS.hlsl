// �ؽ�ó ������ ����� slot0�� ���� ��ġ�� �����.
//Texture2D tex : register(t0); 
Texture2D tex;

SamplerState splr;  // ��������� 0�� ���� �ؽ�ó�� ���÷� ����

float4 main(float2 tc : TexCoord) : SV_TARGET
{
    return tex.Sample(splr, tc);
}