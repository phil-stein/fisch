#version 460 core

	out vec4 FragColor;

  //passed from vertex-shader
  in VS_OUT
  {
    vec2 tex_coords;
    vec3 frag_pos;
  } _in;
	
	uniform vec3 color;
  uniform bool draw_rect; // draw circle or full rect
	
	void main()
	{
    vec2 coords = _in.tex_coords;
    // coords.x = 1 - coords.x;
    coords.y = 1 - coords.y;

    if (draw_rect) 
    {
      FragColor = vec4(color, 1.0);
      return;
    }

    // vec2 coords_mapped = vec2( abs(coords.x * 2 -1), abs(coords.y * 2 -1) );
    vec2 coords_mapped = vec2( 
        ( abs(coords.x * 2 -1) ), 
        ( abs(coords.y * 2 -1) ));// * abs(1 - ratio));
    float dist = distance(vec2(0, 0), coords_mapped); 
    
    // // fade out
    // FragColor = vec4(color, 1 - dist);
    
    if (dist > 1.0f)
    {
      FragColor = vec4(color, 0.0);
    }
    else
    {
      // FragColor = vec4(texture(tex, coords).x) * vec4(tint, 1.0);
      FragColor = vec4(color, 1.0);
	    // FragColor = vec4(1, 1, 0, 1);
    }
  }
