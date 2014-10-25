//-------------------------
//Font.vs
//-------------------------
//This is the font vertex shader
//This will handle the rendering of font characters

cbuffer PerFrameBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//Typedefs
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

//Vertex Shader
PixelInputType FontVertexShader(VertexInputType input)
{
	PixelInputType output;

	//Change the position of the vector to be 4 units from proper matrix calculations
	input.position.w = 1.0f;

	//Calculate the position of the vertex against the world, view and projection matrices
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Store the Texture coordinates from the pixel shader
	output.tex = input.tex;

	return output;
}