#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float light;

out vec2 v_TexCoord;
out float v_Light;

uniform mat4 u_MVP;

void main()
{

	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_Light = light;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_Light;

uniform sampler2D u_Texture;

void main()
{
	vec4 texelColor = texelFetch(u_Texture, ivec2(int(v_TexCoord.x), int(v_TexCoord.y)), 0);
	color = vec4(texelColor.x * v_Light, texelColor.y * v_Light, texelColor.z * v_Light, texelColor.w);
};
