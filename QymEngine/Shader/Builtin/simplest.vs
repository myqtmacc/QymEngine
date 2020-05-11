#version 420

in vec3 Position;
in vec2 TexCoord;

out vec2 tex_coord;

void main( void )
{
	vec4 outPos = vec4(Position, 1.0);
	tex_coord = TexCoord;
	gl_Position = outPos;
}