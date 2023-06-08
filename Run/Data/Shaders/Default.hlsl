//------------------------------------------------------------------------------------------------
struct vs_input_t
{
	float3 localPosition : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

//------------------------------------------------------------------------------------------------
struct v2p_t
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

//------------------------------------------------------------------------------------------------
cbuffer CameraConstants : register(b2)
{
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
};

//------------------------------------------------------------------------------------------------
cbuffer ModelConstants : register(b3)
{
	float4x4 modelMatrix;
	float4 modelColor;
};

//------------------------------------------------------------------------------------------------
Texture2D diffuseTexture : register(t0);

//------------------------------------------------------------------------------------------------
SamplerState diffuseSampler : register(s0);

//------------------------------------------------------------------------------------------------
v2p_t VertexMain(vs_input_t input)
{
	v2p_t v2p;
	float4 modelPosition = mul(modelMatrix, float4(input.localPosition, 1));
	float4 viewPosition = mul(viewMatrix, modelPosition);
	float4 clipPosition = mul(projectionMatrix, viewPosition);
	v2p.position = clipPosition; // float4(ndc, 1);
	v2p.color = input.color * modelColor;
	v2p.uv = input.uv;
	return v2p;
}

//------------------------------------------------------------------------------------------------
float4 PixelMain(v2p_t input) : SV_Target0
{
	float4 color = diffuseTexture.Sample(diffuseSampler, input.uv);
	color *= input.color;
	clip(color.a - 0.01f);
	return color;
}


