float3 camera_position_local;
float3 light_direction_local;
float4 light_diffuse;

// material
float4 material_diffuse;
float4 material_emissive;
float4 material_specular;
float  marerial_power;

sampler color_texture;

struct PixelIn
{
	float4 position : POSITION0;
	float3 normal_local : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

float4 main(PixelIn pixel) : COLOR0
{
	pixel.normal_local = normalize(pixel.normal_local);
	float3 diffuse = (dot(pixel.normal_local, -light_direction_local) + 1.0f) * 0.5f; // material * lightcolor
	float4 color=tex2D(color_texture, pixel.uv)*pixel.color;
	float4 color2 = float4(diffuse*0.85f, 1.0f);// + float4(l, l, l, 0.0f);
	
	return color * color2+float4(0.15f,0.15f,0.15f,0.15f);
}