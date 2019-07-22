// HLSL 셰이더 프로그램

////////////////////////////////////////////////////////
// Filename : color.vs
////////////////////////////////////////////////////////

/*
	이 전역 변수들은 C++코드에서 접근하여 수정할 수 있다.
	int, float와 같은 타입의 변수들을 사용할 수 있고, 셰이더 프로그램을 사용하기 전에 외부적으로 설정할 수 있따.
	보통 대부분의 전역 변수들은 그것이 단지 하나뿐일지라도 'cbuffer'라고 불리는 버퍼 객체 타입에 넣게 된다.
	논리적으로 이 버퍼들을 조직하는 일은 셰이더의 효율적인 실행뿐만 아니라 그래픽 카드가 어떻게 이 버퍼들을 저장하는지와도 관련되기 떄문에 중요하다.	
*/

/////////////
// GLOBALS //
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

/*
	HLSL에서만 지원하는 float4와 같은 타입을 사용하여 셰이더 프로그래밍을 쉽고 가독성이 높게 만듬.
	같은 타입을 더 사용하고 싶다면 COLOR0이나 COLOR1처럼 변수 이름 끝에 숫자를 붙여주어야 한다.
*/

/////////////
// TYPEDEFS 
struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

/*
	ColorVertexShader : 정점셰이더, 정점버퍼의 모든 각 정점에 대해 호출된다.
	입력으로 들어오는 것은 VertexInputType과 같이 셰이더의 소스에서 정의한 자료구좌 그 구조가 맞아 떨어져야 한다.
	정점 셰이더의 처리 결과는 픽셀 셰이더로 보내지는데, 이 소스에서는 앞서 언급했던 PixelInputType 형태로 그 출력이 나오게 된다.
	
	정점셰이더가 출력으로 PixelInputType을 만들어낸다.
	입력된 정점들은 들어와서는 월드, 뷰, 사영행렬과 곱셈 연산을 하게 된다.
	정점을 3D 세계에서의 정점을 우리의 시야에 맞는 2D화면의 위치로 지정해 준다.
	출력으로 나온 데이터들이 색상을 할당받은 뒤에는 픽셀 셰이더의 입력 데이터로 사용된다.
	또한 여기서 위치 지정을 위해 xyz좌표만을 사용하기 때문에 입력 변수 w의 값은 1.0으로 설정함.
*/

//////////////////////////////////////
// Vertex Shader
PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	// 올바르게 행렬 연산을 하기 위하여 position 벡터를 w까지 있는 4성분이 있는 것으로 사용.
	input.position.w = 1.0f;
	
	// 정점의 위치를 월드, 뷰, 사영의 순으로 계산.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	//필셀 셰이더에서 사용하기 위해 입력 색상을 저장한다.
	output.color = input.color;
	
	return output;
}
