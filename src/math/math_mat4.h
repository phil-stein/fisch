#ifndef MATH_MAT_MATH_H
#define MATH_MAT_MATH_H

#ifdef __cplusplus
extern "C"
{
#endif


#include "math_inc.h"

// @DOC: mat4 row goes down (column-major i think)
//
//       mat4[0]	  mat4[1]		  mat4[2]		  mat4[3]
//       mat4[0][0]	mat4[1][0]	mat4[2][0]	mat4[3][0]
//       mat4[0][1]	mat4[1][1]	mat4[2][1]	mat4[3][1]
//       mat4[0][2]	mat4[1][2]	mat4[2][2]	mat4[3][2]
//       mat4[0][3]	mat4[1][3]	mat4[2][3]	mat4[3][3]
typedef float mat4[4][4];

// @DOC: print mat4 type
#define P_MAT4(m)	printf("%s:\n", #m);			\
			printf("%.2f, %.2f, %.2f, %.2f\n", 	\
			m[0][0], m[1][0], m[2][0], m[3][0]); 	\
			printf("%.2f, %.2f, %.2f, %.2f\n", 	\
			m[0][1], m[1][1], m[2][1], m[3][1]); 	\
			printf("%.2f, %.2f, %.2f, %.2f\n", 	\
			m[0][2], m[1][2], m[2][2], m[3][2]); 	\
			printf("%.2f, %.2f, %.2f, %.2f\n", 	\
			m[0][3], m[1][3], m[2][3], m[3][3])



// @DOC: make model matrix with no translation, rotation or scale
//       m: this matrix will be set to identity matrix
M_INLINE void mat4_make_identity(mat4 m)
{
	vec4 r0 = { 1.0f, 0.0f, 0.0f, 0.0f };
	vec4 r1 = { 0.0f, 1.0f, 0.0f, 0.0f };
	vec4 r2 = { 0.0f, 0.0f, 1.0f, 0.0f };
	vec4 r3 = { 0.0f, 0.0f, 0.0f, 1.0f };

	vec4_copy(r0, m[0]);
	vec4_copy(r1, m[1]);
	vec4_copy(r2, m[2]);
	vec4_copy(r3, m[3]);
}

// @DOC: set all values to zero in matrix
//       m: this matrix will be set to zero
M_INLINE void mat4_make_zero(mat4 m)
{
	vec4 v0 = { 0.0f, 0.0f, 0.0f, 0.0f };

	vec4_copy(v0, m[0]);
	vec4_copy(v0, m[1]);
	vec4_copy(v0, m[2]);
	vec4_copy(v0, m[3]);
}

// @DOC: set postion value in a model matrix
//       x:   x component od position
//       y:   y component od position
//       z:   z component od position
//       out: this matrix will have its pos set 
M_INLINE void mat4_set_pos(float x, float y, float z, mat4 out)
{
  out[3][0] = x;
  out[3][1] = y;
  out[3][2] = z;
}

// @DOC: get postion value out of a model matrix
//       m:   model matrix to extract pos from
//       out: will be set to position value
M_INLINE void mat4_get_pos(mat4 m, vec3 out)
{
  out[0] = m[3][0];
  out[1] = m[3][1];
  out[2] = m[3][2];
}

// @DOC: copy one matrix into another
//       m:   matrix to source from
//       out: matrix to copy into
M_INLINE void mat4_copy(mat4 m, mat4 out)
{
  vec4_copy(m[0], out[0]);
  vec4_copy(m[1], out[1]);
  vec4_copy(m[2], out[2]);
  vec4_copy(m[3], out[3]);
}

// @DOC: move model matrix by dist
//       m:    model matrix to be moved
//       dist: distance to move m by
M_INLINE void mat4_translate(mat4 m, vec3 dist)
{
	vec4 v1, v2, v3;
	
	vec4_mul_f(m[0], dist[0], v1);
	vec4_mul_f(m[1], dist[1], v2);
	vec4_mul_f(m[2], dist[2], v3);

	vec4_add(v1, m[3], m[3]);
	vec4_add(v2, m[3], m[3]);
	vec4_add(v3, m[3], m[3]);
}

// @DOC: scale model matrix
//       m:     input matrix
//       scale: scale to be applied to out
//       out:   gets set to m scaled by scle
M_INLINE void mat4_scale(mat4 m, vec3 scale, mat4 out)
{
	vec4_mul_f(m[0], scale[0], out[0]);
	vec4_mul_f(m[1], scale[1], out[1]);
	vec4_mul_f(m[2], scale[2], out[2]);

	vec4_copy(m[3], out[3]);
}

// @DOC: scale model matrix, using float val
//       m:     input matrix
//       scale: scale to be applied to out
//       out:   gets set to m scaled by scle
M_INLINE void mat4_scale_f(mat4 m, float scale, mat4 out)
{
	vec4_mul_f(m[0], scale, out[0]);
	vec4_mul_f(m[1], scale, out[1]);
	vec4_mul_f(m[2], scale, out[2]);

	vec4_copy(m[3], out[3]);
}

// @DOC: multiply two matrices 
//       a, b: matrices to be multiplied a * b
//       out:  matrix filed with the result of mul
M_INLINE void mat4_mul(mat4 a, mat4 b, mat4 out)
{
  // taken straight from cglm
  float a00 = a[0][0], a01 = a[0][1], a02 = a[0][2], a03 = a[0][3],
        a10 = a[1][0], a11 = a[1][1], a12 = a[1][2], a13 = a[1][3],
        a20 = a[2][0], a21 = a[2][1], a22 = a[2][2], a23 = a[2][3],
        a30 = a[3][0], a31 = a[3][1], a32 = a[3][2], a33 = a[3][3],

        b00 = b[0][0], b01 = b[0][1], b02 = b[0][2], b03 = b[0][3],
        b10 = b[1][0], b11 = b[1][1], b12 = b[1][2], b13 = b[1][3],
        b20 = b[2][0], b21 = b[2][1], b22 = b[2][2], b23 = b[2][3],
        b30 = b[3][0], b31 = b[3][1], b32 = b[3][2], b33 = b[3][3];

  out[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
  out[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
  out[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
  out[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
  out[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
  out[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
  out[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
  out[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
  out[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
  out[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
  out[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
  out[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
  out[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
  out[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
  out[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
  out[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
}

// @DOC: multiply matrix by vec4
//       m:   matrix
//       v:   vector4
//       out: filled with the result
M_INLINE void mat4_mul_v(mat4 m, vec4 v, vec4 out)
{
  vec4 tmp;
  tmp[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3];
  tmp[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3];
  tmp[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3];
  tmp[3] = m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3];
  vec4_copy(tmp, out);
}

// @DOC: inverse a mat4 
//       mat: matrix to be inversed
//       out: result is stored here
M_INLINE void mat4_inverse(mat4 mat, mat4 out)
{
  // taken straight from cglm
  float t[6];
  float det;
  float a = mat[0][0], b = mat[0][1], c = mat[0][2], d = mat[0][3],
  e = mat[1][0], f = mat[1][1], g = mat[1][2], h = mat[1][3],
  i = mat[2][0], j = mat[2][1], k = mat[2][2], l = mat[2][3],
  m = mat[3][0], n = mat[3][1], o = mat[3][2], p = mat[3][3];

  t[0] = k * p - o * l; t[1] = j * p - n * l; t[2] = j * o - n * k;
  t[3] = i * p - m * l; t[4] = i * o - m * k; t[5] = i * n - m * j;

  out[0][0] =  f * t[0] - g * t[1] + h * t[2];
  out[1][0] =-(e * t[0] - g * t[3] + h * t[4]);
  out[2][0] =  e * t[1] - f * t[3] + h * t[5];
  out[3][0] =-(e * t[2] - f * t[4] + g * t[5]);

  out[0][1] =-(b * t[0] - c * t[1] + d * t[2]);
  out[1][1] =  a * t[0] - c * t[3] + d * t[4];
  out[2][1] =-(a * t[1] - b * t[3] + d * t[5]);
  out[3][1] =  a * t[2] - b * t[4] + c * t[5];

  t[0] = g * p - o * h; t[1] = f * p - n * h; t[2] = f * o - n * g;
  t[3] = e * p - m * h; t[4] = e * o - m * g; t[5] = e * n - m * f;

  out[0][2] =  b * t[0] - c * t[1] + d * t[2];
  out[1][2] =-(a * t[0] - c * t[3] + d * t[4]);
  out[2][2] =  a * t[1] - b * t[3] + d * t[5];
  out[3][2] =-(a * t[2] - b * t[4] + c * t[5]);

  t[0] = g * l - k * h; t[1] = f * l - j * h; t[2] = f * k - j * g;
  t[3] = e * l - i * h; t[4] = e * k - i * g; t[5] = e * j - i * f;

  out[0][3] =-(b * t[0] - c * t[1] + d * t[2]);
  out[1][3] =  a * t[0] - c * t[3] + d * t[4];
  out[2][3] =-(a * t[1] - b * t[3] + d * t[5]);
  out[3][3] =  a * t[2] - b * t[4] + c * t[5];

  det = 1.0f / (a * out[0][0] + b * out[1][0]
      + c * out[2][0] + d * out[3][0]);

  mat4_scale_f(out, det, out);
}

// @DOC: get front vector from model matrix
//       taken from: https://stackoverflow.com/questions/53608944/getting-a-forward-vector-from-rotation-and-position 
//       model: model matrix
//       out:   gets set to forward dir s
M_INLINE void mat4_get_forward(mat4 model, vec3 out)
{
  mat4 model_inv;
  mat4_inverse(model, model_inv);
  model_inv[2][0] *= -1.0f;
  model_inv[2][1] *= -1.0f;
  // model_inv[2][1] = 0.0f; // *= -1.0f;
  vec3_normalize(model_inv[2], out);
}
// @DOC: get vectors to all four directions from model matrix
M_INLINE void mat4_get_directions(mat4 model, vec3 front, vec3 back, vec3 left, vec3 right)
{
  mat4_get_forward(model, front);
 
  vec3_cross(front, VEC3_Y(1), right);
  vec3_mul_f(right, -1.0f, left);
  vec3_mul_f(front, -1.0f, back);
}

// @DOC: make model matrix rotated by angle around axis
//       m:     matrix to be filled
//       angle: angle in radians
//       axis:  axis round which to rotate
M_INLINE void mat4_rotate_make(mat4 m, float angle, vec3 axis)
{
	// @NOTE: yoinked straight from cglm, look this up
	vec3 axisn, v, vs;
	float c = cosf(angle);

	vec3_normalize(axis, axisn);
	vec3_mul_f(axisn, 1.0f - c, v);
	vec3_mul_f(axisn, sinf(angle), vs);

	vec3_mul_f(axisn, v[0], m[0]);
	vec3_mul_f(axisn, v[1], m[1]);
	vec3_mul_f(axisn, v[2], m[2]);

	m[0][0] += c;       m[1][0] -= vs[2];   m[2][0] += vs[1];
	m[0][1] += vs[2];   m[1][1] += c;       m[2][1] -= vs[0];
	m[0][2] -= vs[1];   m[1][2] += vs[0];   m[2][2] += c;

	m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

// @TODO: @DOC: 
M_INLINE void mat4_mul_rot(mat4 m1, mat4 m2, mat4 out)
{
	// @NOTE: yoinked straight from cglm, look this up
	float a00 = m1[0][0], a01 = m1[0][1], a02 = m1[0][2], a03 = m1[0][3];
	float a10 = m1[1][0], a11 = m1[1][1], a12 = m1[1][2], a13 = m1[1][3];
	float a20 = m1[2][0], a21 = m1[2][1], a22 = m1[2][2], a23 = m1[2][3];
	float a30 = m1[3][0], a31 = m1[3][1], a32 = m1[3][2], a33 = m1[3][3];

	float b00 = m2[0][0], b01 = m2[0][1], b02 = m2[0][2];
	float b10 = m2[1][0], b11 = m2[1][1], b12 = m2[1][2];
	float b20 = m2[2][0], b21 = m2[2][1], b22 = m2[2][2];

	out[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
	out[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
	out[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
	out[0][3] = a03 * b00 + a13 * b01 + a23 * b02;

	out[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
	out[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
	out[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
	out[1][3] = a03 * b10 + a13 * b11 + a23 * b12;

	out[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
	out[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
	out[2][2] = a02 * b20 + a12 * b21 + a22 * b22;
	out[2][3] = a03 * b20 + a13 * b21 + a23 * b22;

	out[3][0] = a30;
	out[3][1] = a31;
	out[3][2] = a32;
	out[3][3] = a33;
}

// @DOC: rotate model matrix deg around axis
//       m:    matrix to rotate
//       deg:  degree in radians to rotate
//       axis: axis around which to rotate
M_INLINE void mat4_rotate(mat4 m, float deg, vec3 axis)
{
	// @NOTE: yoinked straight from cglm, look this up
	mat4 r;
	mat4_rotate_make(r, deg, axis);
	mat4_mul_rot(m, r, m);
}

// @DOC: rotate model matrix deg around axis, at point
//       m:     matrix to rotate
//       point: point around which to rotate
//       deg:   degree in radians to rotate
//       axis:  axis around which to rotate 
M_INLINE void mat4_rotate_at(mat4 m, vec3 point, float deg, vec3 axis)
{
	// @NOTE: yoinked straight from cglm, look this up
	vec3 p_inv;
	vec3_negate(point, p_inv);

	mat4_translate(m, point);
	mat4_rotate(m, deg, axis);
	mat4_translate(m, p_inv);
}

// @DOC: make a lookat matrix
//       pos: pos of camera
//       @TODO:
//       up:  up direction of camera
//       out: gets set to lookat matrix 
M_INLINE void mat4_lookat(vec3 pos, vec3 center, vec3 up, mat4 out) 
{
	// @NOTE: yoinked straight from cglm, look this up
	vec3 f, u, s;

	vec3_sub(center, pos, f);
	vec3_normalize(f, f);

	vec3_cross(f, up, s);
	vec3_normalize(s, s);
	vec3_cross(s, f, u);

	out[0][0] =  s[0];
	out[0][1] =  u[0];
	out[0][2] = -f[0];
	out[1][0] =  s[1];
	out[1][1] =  u[1];
	out[1][2] = -f[1];
	out[2][0] =  s[2];
	out[2][1] =  u[2];
	out[2][2] = -f[2];
	out[3][0] = -vec3_dot(s, pos);
	out[3][1] = -vec3_dot(u, pos);
	out[3][2] =  vec3_dot(f, pos);
	out[0][3] =  out[1][3] = out[2][3] = 0.0f;
	out[3][3] =  1.0f;

}

// @TODO: remove all calcs not needed in 2d
// @TODO: @DOC:
// helper function, making it easier for 2d
M_INLINE void mat4_lookat_2d(vec2 pos, float zoom, mat4 out)
{
	vec3 center;
	center[0] = pos[0];
	center[1] = pos[1];
	center[2] = -1.0f;

	mat4_lookat(VEC3_XYZ(pos[0], pos[1], -zoom), center, VEC3_Y(1), out);
}

// @DOC: make a perspective projection matrix
//       fovy:     field of view in radians
//       aspect:   window width / window height
//       near_val: near clipping plane
//       far_val:  far clipping plane
//       out:      mat4 that will be filled 
M_INLINE void mat4_perspective(float fovy, float aspect, float near_val, float far_val, mat4  out) 
{
	// @NOTE: yoinked straight from cglm, look this up
	float f, fn;

  	mat4_make_zero(out);

  	f  = 1.0f / tanf(fovy * 0.5f);
  	fn = 1.0f / (near_val - far_val);

  	out[0][0] = f / aspect;
  	out[1][1] = f;
  	out[2][2] = (near_val + far_val) * fn;
  	out[2][3] = -1.0f;
  	out[3][2] = 2.0f * near_val * far_val * fn;
}

// @DOC: make a orthographic projection matrix
//       @TODO: 
//       left: 
//       right:
//       bottom:
//       top:
//       near_val: near clipping plane
//       far_val:  far clipping plane
//       out:      mat4 that will be filled 
M_INLINE void mat4_ortho(float left, float right, float bottom,  float top, float near_val, float far_val, mat4  out) 
{
	// @NOTE: yoinked straight from cglm, look this up
  float rl, tb, fn;
  mat4_make_zero(out);  
  rl = 1.0f / (right  - left);
  tb = 1.0f / (top    - bottom);   
  fn =-1.0f / (far_val - near_val);
  out[0][0] = 2.0f * rl;
  out[1][1] = 2.0f * tb;
  out[2][2] = 2.0f * fn;
  out[3][0] =-(right  + left)    * rl;
  out[3][1] =-(top    + bottom)  * tb;
  out[3][2] = (far_val + near_val) * fn;
  out[3][3] = 1.0f; 
}

// @DOC: make a model matrix
//       pos:   position
//       rot:   rotation in degree
//       scale: scale
//       model: this mat4 will be filled
M_INLINE void mat4_make_model(vec3 pos, vec3 rot, vec3 scale, mat4 model)
{
	mat4_make_identity(model);
	float x = rot[0];  m_deg_to_rad(&x);
	float y = rot[1];  m_deg_to_rad(&y);
	float z = rot[2];  m_deg_to_rad(&z);
	
	mat4_rotate_at(model, pos, x, VEC3_X(1));
	mat4_rotate_at(model, pos, y, VEC3_Y(1));
	mat4_rotate_at(model, pos, z, VEC3_Z(1));
	
	mat4_translate(model, pos);

	mat4_scale(model, scale, model);
}

// @DOC: make model matrix, helper function, removes calcs not needed or 2d
//       pos:  position
//       size: size
//       rot:  rotation in degree
//       model: this mat4 will be filled
M_INLINE void mat4_make_model_2d(vec2 pos, vec2 size, float rot, mat4 model)
{
	vec3 pos3  = { pos[0], pos[1], 0.0f };
	vec3 size3 = { size[0], 1.0f, size[1] };
	// mat4_make_model(model, pos3, rot3, size3);
	mat4_make_identity(model);
	m_deg_to_rad(&rot);
	f32 x = -90.0f; m_deg_to_rad(&x);
	
	mat4_rotate_at(model, pos3, x,   VEC3_X(1));
  mat4_rotate_at(model, pos3, rot, VEC3_Z(1));
	
	mat4_translate(model, pos3);

	mat4_scale(model, size3, model);
}

#ifdef __cplusplus
} // extern C
#endif

#endif

