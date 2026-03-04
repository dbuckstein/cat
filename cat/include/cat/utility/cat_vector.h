////////////////////////////////////////////////////////////////////////////////
/// Copyright 2025-2026 Daniel S. Buckstein
/// 
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
/// 
///     http://www.apache.org/licenses/LICENSE-2.0
/// 
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
////////////////////////////////////////////////////////////////////////////////

/*! \file cat_vector.h
*   \brief Vector math interface.
*/

#ifndef _CAT_VECTOR_H_
#define _CAT_VECTOR_H_


#include "cat/cat_platform.h"

#include <float.h>

#define _USE_MATH_DEFINES
#include <math.h>


cat_interface_begin;


typedef struct vec2f_s
{
    float x, y;
} vec2f_t;
#define pvec2f_t vec2f_t*
#define pcvec2f_t vec2f_t const*

typedef struct vec3f_s
{
    float x, y, z;
} vec3f_t;
#define pvec3f_t vec3f_t*
#define pcvec3f_t vec3f_t const*


typedef struct mat2f_s
{
    vec2f_t x, y;
} mat2f_t;
#define pmat2f_t mat2f_t*
#define pcmat2f_t mat2f_t const*

typedef struct mat3f_s
{
    vec3f_t x, y, z;
} mat3f_t;
#define pmat3f_t mat3f_t*
#define pcmat3f_t mat3f_t const*


typedef union vec2f_comp_s
{
    float v[2];
    vec2f_t c;
} vec2f_comp_t;
typedef union vec3f_comp_s
{
    float v[3];
    vec3f_t c;
} vec3f_comp_t;


#define vec2(x,y)   { (x), (y) }
#define vec2s(xy)   { (xy), (xy) }

#define vec3(x,y,z) { (x), (y), (z) }
#define vec3s(xyz)  { (xyz), (xyz), (xyz) }
#define vec3v(xy,z) { (xy).x, (xy).y, (z) }


#define FLT_INVALID (-FLT_MAX)
#define DBL2FLT(x)  tokcat(x,F)
#define INT2FLT(x)  tokcat(x,.0F)


#define real_t      float
#define vec2_t      vec2f_t
#define pvec2_t     pvec2f_t
#define pcvec2_t    pcvec2f_t
#define vec3_t      vec3f_t
#define pvec3_t     pvec3f_t
#define pcvec3_t    pcvec3f_t
#define mat2_t      mat2f_t
#define pmat2_t     pmat2f_t
#define pcmat2_t    pcmat2f_t
#define mat3_t      mat3f_t
#define pmat3_t     pmat3f_t
#define pcmat3_t    pcmat3f_t

#define RE_EPSILON  FLT_EPSILON
#define RE_INVALID  FLT_INVALID
#define DBL2RE      DBL2FLT
#define INT2RE      INT2FLT


cat_decl float frandf(void);
cat_decl float frandcf(void);
cat_decl bool fcmpf(float const x_lh, float const x_rh);
cat_decl bool fcmp0f(float const x);
cat_decl float frecipf(float const x);
cat_decl float fsqf(float const x);
cat_decl float fcbf(float const x);
cat_decl float fsqrtf(float const x);
cat_decl float fcbrtf(float const x);
cat_decl float fsqrtinvf(float const x);
cat_decl float fcbrtinvf(float const x);
cat_decl float fclampf(float const x, float const x_lo, float const x_hi);
cat_decl bool frangef(float const x, float const x_lo, float const x_hi);
cat_decl bool frangecf(float const x, float const x_lo, float const x_hi);
cat_decl float fmadf(float const x_orig, float const x_diff, float const u);
cat_decl float fsgnf(float const x);
cat_decl float fabssgnf(float* const p_sgn_out, float const x);
cat_decl float fsgnabsf(float* const p_abs_out, float const x);
cat_decl float fdeg2radf(float const x_deg);
cat_decl float frad2degf(float const x_rad);
cat_decl float fsincosf(float* const p_cos_out, float const x_rad);
cat_decl float fcossinf(float* const p_sin_out, float const x_rad);
cat_decl void fswapf(float* const x_lh, float* const x_rh);

cat_decl pcvec3f_t vec3f_clr(pvec3f_t const v_inout);
cat_decl pcvec3f_t vec3f_set(pvec3f_t const v_out, float const x, float const y, float const z);
cat_decl pcvec3f_t vec3f_cpyf(pvec3f_t const v_out, pcvec3f_t const v);
cat_decl pcvec3f_t vec3f_cpy(pvec3f_t const v_out_unique, pcvec3f_t const v);
cat_decl pcvec3f_t vec3f_add(pvec3f_t const v_out, pcvec3f_t const v_lh, pcvec3f_t const v_rh);
cat_decl pcvec3f_t vec3f_sub(pvec3f_t const v_out, pcvec3f_t const v_lh, pcvec3f_t const v_rh);
cat_decl pcvec3f_t vec3f_mul(pvec3f_t const v_out, pcvec3f_t const v_lh, float const s_rh);
cat_decl pcvec3f_t vec3f_div(pvec3f_t const v_out, pcvec3f_t const v_lh, float const s_rh);
cat_decl pcvec3f_t vec3f_mad(pvec3f_t const v_out, pcvec3f_t const v_orig, pcvec3f_t const v_diff, float const u);
cat_decl pcvec3f_t vec3f_mulc(pvec3f_t const v_out, pcvec3f_t const v_lh, pcvec3f_t const v_rh);
cat_decl pcvec3f_t vec3f_divc(pvec3f_t const v_out, pcvec3f_t const v_lh, pcvec3f_t const v_rh);
cat_decl pcvec3f_t vec3f_negate(pvec3f_t const v_out, pcvec3f_t const v);
cat_decl float vec3f_dot(pcvec3f_t const v_lh, pcvec3f_t const v_rh);
cat_decl float vec3f_lensq(pcvec3f_t const v);
cat_decl float vec3f_len(pcvec3f_t const v);
cat_decl float vec3f_proj(pvec3f_t const v_out, pcvec3f_t const v_fixed, pcvec3f_t const v);
cat_decl float vec3f_ortho(pvec3f_t const v_out, pcvec3f_t const v_fixed, pcvec3f_t const v);
cat_decl pcvec3f_t vec3f_cross(pvec3f_t const v_out_unique, pcvec3f_t const v_lh, pcvec3f_t const v_rh);
cat_decl float vec3f_triple(pcvec3f_t const v_dot, pcvec3f_t const v_cross_lh, pcvec3f_t const v_cross_rh);
cat_decl float vec3f_resize(pvec3f_t const v_out, pcvec3f_t const v, float const len);
cat_decl float vec3f_resize_inplace(pvec3f_t const v_inout, float const len);
cat_decl float vec3f_normalize(pvec3f_t const v_out, pcvec3f_t const v);
cat_decl float vec3f_normalize_inplace(pvec3f_t const v_inout);

cat_decl pcmat3f_t mat3f_clr(pmat3f_t const m_inout);
cat_decl pcmat3f_t mat3f_id(pmat3f_t const m_inout);
cat_decl float mat3f_det(pcmat3f_t const m);
cat_decl pcmat3f_t mat3f_transpose(pmat3f_t const m_out_unique, pcmat3f_t const m);
cat_decl pcmat3f_t mat3f_transpose_inplace(pmat3f_t const m_inout);
cat_decl float mat3f_inv(pmat3f_t const m_out_unique, pcmat3f_t const m);
cat_decl float mat3f_inv_inplace(pmat3f_t const m_inout);
cat_decl pcvec3f_t mat3f_mulv(pvec3f_t const v_out_unique, pcmat3f_t const m_lh, pcvec3f_t const v_rh);
cat_decl pcvec3f_t mat3f_tmulv(pvec3f_t const v_out_unique, pcmat3f_t const mt_lh, pcvec3f_t const v_rh);
cat_decl float mat3f_imulv(pvec3f_t const v_out_unique, pcmat3f_t const mi_lh, pcvec3f_t const v_rh);
cat_decl pcmat3f_t mat3f_mulm(pmat3f_t const m_out_unique, pcmat3f_t const m_lh, pcmat3f_t const m_rh);
cat_decl pcmat3f_t mat3f_tmulm(pmat3f_t const m_out_unique, pcmat3f_t const mt_lh, pcmat3f_t const m_rh);
cat_decl float mat3f_imulm(pmat3f_t const m_out_unique, pcmat3f_t const mi_lh, pcmat3f_t const m_rh);


cat_interface_end;


#endif // #ifndef _CAT_VECTOR_H_