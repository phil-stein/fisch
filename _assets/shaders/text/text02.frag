#version 460 core

out vec4 FragColor;

in vec2 uv_coords;

uniform vec3 tint;
uniform sampler2D tex;

void main()
{
  float r = 1 - texture(tex, uv_coords.xy).r;
  FragColor = vec4(vec3(r), r) * vec4(tint.rgb, 1.0);
  // FragColor = vec4(vec3(r), 1.0) * vec4(tint.rgb, 1.0);
  // FragColor = vec4(texture(tex, uv_coords.xy).rgb, 1.0) * vec4(tint.rgb, 1.0);
  
  // FragColor = vec4(uv_coords.xy, 0.0, 1.0);
  // FragColor = vec4(uv_coords.x* 86, 0.0, 0.0, 1.0);
}
