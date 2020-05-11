#version 420
		
in vec3 Position;
in vec2 TexCoord;
uniform mat4 Mvpm;
		
out vec2 tex_coord;
		
void main( void )
{
	vec4 outPos = Mvpm * vec4(Position, 1.0);
	tex_coord = TexCoord;
	gl_Position = outPos;
}