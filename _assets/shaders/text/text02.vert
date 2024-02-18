#version 460 core

  layout (location = 0) in vec2 aPos; 
  layout (location = 1) in vec2 aUV;   
  
  
  out vec2 uv_coords;
  
  uniform float scl;
  uniform vec2  pos;
  uniform mat4 proj;          // matrix for camera projection
  // uniform mat4 view;          // matrix for view transformation
  // uniform mat4 model;         // matrix for applying the objects transform
  
  void main() 
  { 
    // gl_Position = proj * view * model * vec4(aPos.x, aPos.y, -1.0, 1.0);     
    // gl_Position = proj * view * vec4(aPos.x, aPos.y, 0.0, 1.0);     
    // gl_Position = proj * model * vec4(aPos.x, aPos.y, -1.0, 1.0);     
    
    gl_Position = proj * vec4((aPos.x * scl) + pos.x, (aPos.y * scl) + pos.y, -1.0, 1.0);     
    // gl_Position = proj * vec4( (aPos.x + pos.x) * scl, (aPos.y + pos.y) * scl, -1.0, 1.0);     

    // float uv_u = ( 1.0 / 86.0 ) * ((aUV.x) -1.0);
    float uv_u = aUV.x;
    float uv_v = aUV.y;
    // uv_u *= 2.0;
    // uv_v *= 2.0;
    uv_coords  = vec2(uv_u, uv_v);
  }
