#version 460 core
in vec2 texCoords;

out vec4 frag_Color;
uniform sampler2D tex;

void main()
{
	frag_Color = texture(tex, texCoords);
}