#version 460 core
in vec3 color;
in vec2 texCoords;

out vec4 fColor;
uniform sampler2D tex;

void main()
{
	fColor = texture(tex, texCoords);
}