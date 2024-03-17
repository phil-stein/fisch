#version 460 core
layout (location = 0) out vec4 color;
layout (location = 1) out vec4 material;
layout (location = 2) out vec4 normal;
layout (location = 3) out vec4 position;

out vec4 FragColor;

//passed from vertex-shader
in VS_OUT
{
  vec2 uv_coords;
  vec3 frag_pos;
  vec3 normal;
  mat3 TBN;
} _in;

uniform vec3 tint;
uniform sampler2D tex;

void main()
{
  position = vec4(_in.frag_pos, 1.0);
  normal   = vec4(_in.normal, 1.0);
  
  material.r = 1.0; // roughness
  material.g = 0.0; // metallic
  material.b = 1.0; // emissive/unlit
  material.a = 1.0; // idk, looks cool when 0.0 though
  
  // FragColor = texture(tex, _in.uv_coords.xy) * vec4(tint.rgb, 1.0);
  color = texture(tex, _in.uv_coords.xy) * vec4(tint.rgb, 1.0);

}
