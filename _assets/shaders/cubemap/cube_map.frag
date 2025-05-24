#version 460 core

out vec4 FragColor;
	
in vec3 dir;
uniform samplerCube cube_map;

void main()
{
	// FragColor = texture(cube_map, dir);
  FragColor = vec4( texture(cube_map, dir).rgb, 1.0 );
  // color = textureLod(cube_map, dir, 1.0);
}
