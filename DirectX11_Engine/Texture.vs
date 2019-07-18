//////////////
// Texture.vs
//////////////

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};
/*
	텍스쳐에서는 U와 V좌표만을 사용하기 떄문에 float2 자료형을 이용한다.
	정점 셰이더와 픽셀 셰이더에서 텍스처 좌표를 나타내기 위해 TEXCOORD0이라는 것을 사용한다.
	여러 개의 텍스쳐 좌표가 가능하다면 그 값을 0부터 아무 숫자로나 지정할 수도 있다.
*/

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
	
	// Calculate the position of the vertex against the world, view, projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	// 컬러 정점 셰이더와 텍스쳐 정점 셰이더의 차이점은 색상을 그대로 전달하는 것이 아니라, 텍스쳐의 좌표들을 복사해서 픽셀 셰이더로 전달하는 것.
	
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
	
	return output;
}