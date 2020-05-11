#version 420
		
in vec3 Position;
in vec2 TexCoord;
uniform mat4 Mvpm;
uniform mat4 Texm;
		
out vec2 tex_coord;
		
void main( void )
{
	vec4 outPos = Mvpm * vec4(Position, 1.0);
	//	vec4 outPos = vec4(Position.xy, 0.0, 1.0);
	tex_coord = (Texm * vec4(TexCoord, 0.0, 1.0)).xy;
	//	tex_coord = TexCoord;
	gl_Position = outPos;
}