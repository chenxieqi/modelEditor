float4x4 world_view_projection;
float4x4 world_inverse_transpose;

struct VertexIn
{
	float3 position_local : POSITION0;
	float3 normal_local : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

struct VertexOut
{
	float4 position : POSITION0;
	float3 normal_local : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

VertexOut main(VertexIn vertex)
{
	VertexOut result;
	result.position = mul(float4(vertex.position_local, 1.0f), world_view_projection);
	result.uv = vertex.uv;
	result.color=vertex.color;
	//result.normal_local=vertex.normal_local;
	result.normal_local = mul(float4(vertex.normal_local, 0.0f), world_inverse_transpose).xyz;
	result.normal_local = normalize(result.normal_local);

	return result;
}