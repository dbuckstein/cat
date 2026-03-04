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

/*
* cat_vector.c
* Vector math implementation.
*/

#include "cat/utility/cat_vector.h"
#include "cat/cat_platform.inl"


cat_implementation_begin;


#ifdef CAT_RELEASE
#define vec_assert_or_bail(expr) (void)
#else // #ifdef CAT_RELEASE
#define vec_assert_or_bail(expr) assert_or_bail(expr)
#endif // #else // #ifdef CAT_RELEASE


cat_impl float frandf(void)
{
    return (float)rand() / (float)(RAND_MAX + 1);
}
cat_impl float frandcf(void)
{
    return (float)rand() / (float)RAND_MAX;
}
cat_impl bool fcmpf(float const x_lh, float const x_rh)
{
    float x_sum = 0.0F;
    if (x_lh == x_rh)
        return true;
    x_sum = (fabsf(x_lh) + fabsf(x_rh));
    return (fabsf(x_lh - x_rh) <= (max(1.0F, x_sum) * FLT_EPSILON));
}
cat_impl bool fcmp0f(float const x)
{
    return (fabsf(x) <= FLT_EPSILON);
}
cat_impl float frecipf(float const x)
{
    vec_assert_or_bail(!fcmp0f(x)) FLT_INVALID;
    return (1.0F / x);
}
cat_impl float fsqf(float const x)
{
    return (x * x);
}
cat_impl float fcbf(float const x)
{
    return (x * x * x);
}
cat_impl float fsqrtf(float const x)
{
    vec_assert_or_bail(x >= 0.0F) FLT_INVALID;
    return sqrtf(x);
}
cat_impl float fcbrtf(float const x)
{
    return cbrtf(x);
}
cat_impl float fsqrtinvf(float const x)
{
    vec_assert_or_bail(x > FLT_EPSILON) FLT_INVALID;
    return (1.0F / sqrtf(x));
}
cat_impl float fcbrtinvf(float const x)
{
    vec_assert_or_bail(!fcmp0f(x)) FLT_INVALID;
    return (1.0F / cbrtf(x));
}
cat_impl float fclampf(float const x, float const x_lo, float const x_hi)
{
    return (x >= x_lo ? x > x_hi ? x_hi : x : x_lo);
}
cat_impl bool frangef(float const x, float const x_lo, float const x_hi)
{
    return ((x > x_lo) && (x < x_hi));
}
cat_impl bool frangecf(float const x, float const x_lo, float const x_hi)
{
    return ((x >= x_lo) && (x <= x_hi));
}
cat_impl float fmadf(float const x_orig, float const x_diff, float const u)
{
    return fmaf(u, x_diff, x_orig);
}
cat_impl float fsgnf(float const x)
{
    return (x >= 0.0F ? x > 0.0F ? +1.0F : 0.0F : -1.0F);
}
cat_impl float fabssgnf(float* const p_sgn_out, float const x)
{
    vec_assert_or_bail(p_sgn_out) FLT_INVALID;
    *p_sgn_out = fsgnf(x);
    return fabsf(x);
}
cat_impl float fsgnabsf(float* const p_abs_out, float const x)
{
    vec_assert_or_bail(p_abs_out) FLT_INVALID;
    *p_abs_out = fabsf(x);
    return fsgnf(x);
}
cat_impl float fdeg2radf(float const x_deg)
{
    return (x_deg * DBL2FLT(M_PI) / 180.0F);
}
cat_impl float frad2degf(float const x_rad)
{
    return (x_rad * 180.0F / DBL2FLT(M_PI));
}
cat_impl float fsincosf(float* const p_cos_out, float const x_rad)
{
    vec_assert_or_bail(p_cos_out) FLT_INVALID;
    *p_cos_out = cosf(x_rad);
    return sinf(x_rad);
}
cat_impl float fcossinf(float* const p_sin_out, float const x_rad)
{
    vec_assert_or_bail(p_sin_out) FLT_INVALID;
    *p_sin_out = sinf(x_rad);
    return cosf(x_rad);
}
cat_impl void fswapf(float* const x_lh, float* const x_rh)
{
    assert(x_lh);
    assert(x_rh);
    float const tmp = *x_lh;
    *x_lh = *x_rh;
    *x_rh = tmp;
}


cat_impl pcvec3f_t vec3f_clr(pvec3f_t const v_inout)
{
    vec_assert_or_bail(v_inout) NULL;
    memclr(v_inout, sizeof(vec3f_t));
    return v_inout;
}
cat_impl pcvec3f_t vec3f_set(pvec3f_t const v_out, float const x, float const y, float const z)
{
    vec_assert_or_bail(v_out) NULL;
    v_out->x = x;
    v_out->y = y;
    v_out->z = z;
    return v_out;
}
cat_impl pcvec3f_t vec3f_cpyf(pvec3f_t const v_out, pcvec3f_t const v)
{
    vec_assert_or_bail(v_out) NULL;
    vec_assert_or_bail(v) NULL;
    v_out->x = v->x;
    v_out->y = v->y;
    v_out->z = v->z;
    return v_out;
}
cat_impl pcvec3f_t vec3f_cpy(pvec3f_t const v_out_unique, pcvec3f_t const v)
{
    vec_assert_or_bail(v_out_unique) NULL;
    vec_assert_or_bail(v) NULL;
    vec_assert_or_bail(v_out_unique != v) NULL;
    return vec3f_cpyf(v_out_unique, v);
}
cat_impl pcvec3f_t vec3f_add(pvec3f_t const v_out, pcvec3f_t const v_lh, pcvec3f_t const v_rh)
{
    vec_assert_or_bail(v_out) NULL;
    vec_assert_or_bail(v_lh) NULL;
    vec_assert_or_bail(v_rh) NULL;
    v_out->x = v_lh->x + v_rh->x;
    v_out->y = v_lh->y + v_rh->y;
    v_out->z = v_lh->z + v_rh->z;
    return v_out;
}
cat_impl pcvec3f_t vec3f_sub(pvec3f_t const v_out, pcvec3f_t const v_lh, pcvec3f_t const v_rh)
{
    vec_assert_or_bail(v_out) NULL;
    vec_assert_or_bail(v_lh) NULL;
    vec_assert_or_bail(v_rh) NULL;
    v_out->x = v_lh->x - v_rh->x;
    v_out->y = v_lh->y - v_rh->y;
    v_out->z = v_lh->z - v_rh->z;
    return v_out;
}
cat_impl pcvec3f_t vec3f_mul(pvec3f_t const v_out, pcvec3f_t const v_lh, float const s_rh)
{
    vec_assert_or_bail(v_out) NULL;
    vec_assert_or_bail(v_lh) NULL;
    v_out->x = v_lh->x * s_rh;
    v_out->y = v_lh->y * s_rh;
    v_out->z = v_lh->z * s_rh;
    return v_out;
}
cat_impl pcvec3f_t vec3f_div(pvec3f_t const v_out, pcvec3f_t const v_lh, float const s_rh)
{
    vec_assert_or_bail(v_out) NULL;
    vec_assert_or_bail(v_lh) NULL;
    v_out->x = v_lh->x / s_rh;
    v_out->y = v_lh->y / s_rh;
    v_out->z = v_lh->z / s_rh;
    return v_out;
}
cat_impl pcvec3f_t vec3f_mad(pvec3f_t const v_out, pcvec3f_t const v_orig, pcvec3f_t const v_diff, float const u)
{
    vec_assert_or_bail(v_out) NULL;
    vec_assert_or_bail(v_orig) NULL;
    vec_assert_or_bail(v_diff) NULL;
    v_out->x = fmadf(v_orig->x, v_diff->x, u);
    v_out->y = fmadf(v_orig->y, v_diff->y, u);
    v_out->z = fmadf(v_orig->z, v_diff->z, u);
    return v_out;
}
cat_impl pcvec3f_t vec3f_mulc(pvec3f_t const v_out, pcvec3f_t const v_lh, pcvec3f_t const v_rh)
{
    vec_assert_or_bail(v_out) NULL;
    vec_assert_or_bail(v_lh) NULL;
    v_out->x = v_lh->x * v_rh->x;
    v_out->y = v_lh->y * v_rh->y;
    v_out->z = v_lh->z * v_rh->z;
    return v_out;
}
cat_impl pcvec3f_t vec3f_divc(pvec3f_t const v_out, pcvec3f_t const v_lh, pcvec3f_t const v_rh)
{
    vec_assert_or_bail(v_out) NULL;
    vec_assert_or_bail(v_lh) NULL;
    v_out->x = v_lh->x / v_rh->x;
    v_out->y = v_lh->y / v_rh->y;
    v_out->z = v_lh->z / v_rh->z;
    return v_out;
}
cat_impl pcvec3f_t vec3f_negate(pvec3f_t const v_out, pcvec3f_t const v)
{
    vec_assert_or_bail(v_out) NULL;
    vec_assert_or_bail(v) NULL;
    v_out->x = -v->x;
    v_out->y = -v->y;
    v_out->z = -v->z;
    return v_out;
}
cat_impl float vec3f_dot(pcvec3f_t const v_lh, pcvec3f_t const v_rh)
{
    vec_assert_or_bail(v_lh) FLT_INVALID;
    vec_assert_or_bail(v_rh) FLT_INVALID;
    return (v_lh->x * v_rh->x + v_lh->y * v_rh->y + v_lh->z * v_rh->z);
}
cat_impl float vec3f_lensq(pcvec3f_t const v)
{
    vec_assert_or_bail(v) FLT_INVALID;
    return vec3f_dot(v, v);
}
cat_impl float vec3f_len(pcvec3f_t const v)
{
    vec_assert_or_bail(v) FLT_INVALID;
    return sqrtf(vec3f_lensq(v));
}
cat_impl float vec3f_proj(pvec3f_t const v_out, pcvec3f_t const v_fixed, pcvec3f_t const v)
{
    float lensq_fixed = 0.0F;
    float proj_factor = 0.0F;
    vec_assert_or_bail(v_out) FLT_INVALID;
    vec_assert_or_bail(v_fixed) FLT_INVALID;
    vec_assert_or_bail(v) FLT_INVALID;
    lensq_fixed = vec3f_lensq(v_fixed);
    if (!fcmp0f(lensq_fixed))
    {
        proj_factor = vec3f_dot(v_fixed, v) / lensq_fixed;
        vec3f_mul(v_out, v_fixed, proj_factor);
    }
    else
    {
        vec3f_clr(v_out);
        proj_factor = 0.0F;
    }
    return proj_factor;
}
cat_impl float vec3f_ortho(pvec3f_t const v_out, pcvec3f_t const v_fixed, pcvec3f_t const v)
{
    float lensq_fixed = 0.0F;
    float proj_factor = 0.0F;
    vec_assert_or_bail(v_out) FLT_INVALID;
    vec_assert_or_bail(v_fixed) FLT_INVALID;
    vec_assert_or_bail(v) FLT_INVALID;
    lensq_fixed = vec3f_lensq(v_fixed);
    if (!fcmp0f(lensq_fixed))
    {
        proj_factor = vec3f_dot(v_fixed, v) / lensq_fixed;
        vec3f_mad(v_out, v, v_fixed, -proj_factor);
    }
    else
    {
        vec3f_cpy(v_out, v);
        proj_factor = 0.0F;
    }
    return proj_factor;
}
cat_impl pcvec3f_t vec3f_cross(pvec3f_t const v_out_unique, pcvec3f_t const v_lh, pcvec3f_t const v_rh)
{
    vec_assert_or_bail(v_out_unique) NULL;
    vec_assert_or_bail(v_lh) NULL;
    vec_assert_or_bail(v_rh) NULL;
    vec_assert_or_bail(v_out_unique != v_lh) NULL;
    vec_assert_or_bail(v_out_unique != v_rh) NULL;
    v_out_unique->x = v_lh->y * v_rh->z - v_lh->z * v_rh->y;
    v_out_unique->y = v_lh->z * v_rh->x - v_lh->x * v_rh->z;
    v_out_unique->z = v_lh->x * v_rh->y - v_lh->y * v_rh->x;
    return v_out_unique;
}
cat_impl float vec3f_triple(pcvec3f_t const v_dot, pcvec3f_t const v_cross_lh, pcvec3f_t const v_cross_rh)
{
    vec3f_t cross = vec3s(0.0F);
    vec_assert_or_bail(v_dot) FLT_INVALID;
    vec_assert_or_bail(v_cross_lh) FLT_INVALID;
    vec_assert_or_bail(v_cross_rh) FLT_INVALID;
    return vec3f_dot(v_dot, vec3f_cross(&cross, v_cross_lh, v_cross_rh));
}
cat_impl float vec3f_resize(pvec3f_t const v_out, pcvec3f_t const v, float const len)
{
    float len_orig = 0.0F;
    float len_scale = 0.0F;
    vec_assert_or_bail(v_out) FLT_INVALID;
    vec_assert_or_bail(v) FLT_INVALID;
    len_orig = vec3f_lensq(v);
    if ((len_orig <= FLT_EPSILON) != 0)
    {
        vec3f_clr(v_out);
        return 0.0F;
    }
    len_orig = sqrtf(len_orig);
    len_scale = len / len_orig;
    v_out->x = v->x * len_scale;
    v_out->y = v->y * len_scale;
    v_out->z = v->z * len_scale;
    return len_orig;
}
cat_impl float vec3f_resize_inplace(pvec3f_t const v_inout, float const len)
{
    vec_assert_or_bail(v_inout) FLT_INVALID;
    return vec3f_resize(v_inout, v_inout, len);
}
cat_impl float vec3f_normalize(pvec3f_t const v_out, pcvec3f_t const v)
{
    vec_assert_or_bail(v_out) FLT_INVALID;
    vec_assert_or_bail(v) FLT_INVALID;
    return vec3f_resize(v_out, v, 1.0F);
}
cat_impl float vec3f_normalize_inplace(pvec3f_t const v_inout)
{
    vec_assert_or_bail(v_inout) FLT_INVALID;
    return vec3f_normalize(v_inout, v_inout);
}


static inline float mat3f_internal_r0mulv(pcmat3f_t const m_lh, pcvec3f_t const v_rh)
{
    return (m_lh->x.x * v_rh->x + m_lh->y.x * v_rh->y + m_lh->z.x * v_rh->z);
}
static inline float mat3f_internal_r1mulv(pcmat3f_t const m_lh, pcvec3f_t const v_rh)
{
    return (m_lh->x.y * v_rh->x + m_lh->y.y * v_rh->y + m_lh->z.y * v_rh->z);
}
static inline float mat3f_internal_r2mulv(pcmat3f_t const m_lh, pcvec3f_t const v_rh)
{
    return (m_lh->x.z * v_rh->x + m_lh->y.z * v_rh->y + m_lh->z.z * v_rh->z);
}


cat_impl pcmat3f_t mat3f_clr(pmat3f_t const m_inout)
{
    vec_assert_or_bail(m_inout) NULL;
    vec3f_clr(&m_inout->x);
    vec3f_clr(&m_inout->y);
    vec3f_clr(&m_inout->z);
    return m_inout;
}
cat_impl pcmat3f_t mat3f_id(pmat3f_t const m_inout)
{
    vec_assert_or_bail(m_inout) NULL;
    vec3f_set(&m_inout->x, 1.0F, 0.0F, 0.0F);
    vec3f_set(&m_inout->y, 0.0F, 1.0F, 0.0F);
    vec3f_set(&m_inout->z, 0.0F, 0.0F, 1.0F);
    return m_inout;
}
cat_impl float mat3f_det(pcmat3f_t const m)
{
    vec_assert_or_bail(m) FLT_INVALID;
    return vec3f_triple(&m->x, &m->y, &m->z);
}
cat_impl pcmat3f_t mat3f_transpose(pmat3f_t const m_out_unique, pcmat3f_t const m)
{
    vec_assert_or_bail(m_out_unique) NULL;
    vec_assert_or_bail(m) NULL;
    vec_assert_or_bail(m_out_unique != m) NULL;
    vec3f_set(&m_out_unique->x, m->x.x, m->y.x, m->z.x);
    vec3f_set(&m_out_unique->y, m->x.y, m->y.y, m->z.y);
    vec3f_set(&m_out_unique->z, m->x.z, m->y.z, m->z.z);
    return m_out_unique;
}
cat_impl pcmat3f_t mat3f_transpose_inplace(pmat3f_t const m_inout)
{
    vec_assert_or_bail(m_inout) NULL;
    fswapf(&m_inout->y.z, &m_inout->z.y);
    fswapf(&m_inout->z.x, &m_inout->x.z);
    fswapf(&m_inout->x.y, &m_inout->y.x);
    return m_inout;
}
cat_impl float mat3f_inv(pmat3f_t const m_out_unique, pcmat3f_t const m)
{
    float det = 0.0F;
    float det_inv = 0.0F;
    vec3f_t cross_yz = vec3s(0.0F), cross_zx = vec3s(0.0F), cross_xy = vec3s(0.0F);
    vec_assert_or_bail(m_out_unique) FLT_INVALID;
    vec_assert_or_bail(m) FLT_INVALID;
    vec_assert_or_bail(m_out_unique != m) FLT_INVALID;
    vec3f_cross(&cross_yz, &m->y, &m->z);
    det = vec3f_dot(&m->x, &cross_yz);
    if (!fcmp0f(det))
    {
        vec3f_cross(&cross_zx, &m->z, &m->x);
        vec3f_cross(&cross_xy, &m->x, &m->y);
        det_inv = frecipf(det);
        vec3f_set(&m_out_unique->x, cross_yz.x * det_inv, cross_zx.x * det_inv, cross_xy.x * det_inv);
        vec3f_set(&m_out_unique->y, cross_yz.y * det_inv, cross_zx.y * det_inv, cross_xy.y * det_inv);
        vec3f_set(&m_out_unique->z, cross_yz.z * det_inv, cross_zx.z * det_inv, cross_xy.z * det_inv);
    }
    else
    {
        mat3f_clr(m_out_unique);
        det = 0.0F;
    }
    return det;
}
cat_impl float mat3f_inv_inplace(pmat3f_t const m_inout)
{
    float det = 0.0F;
    float det_inv = 0.0F;
    vec3f_t cross_yz = vec3s(0.0F), cross_zx = vec3s(0.0F), cross_xy = vec3s(0.0F);
    vec_assert_or_bail(m_inout) FLT_INVALID;
    vec3f_cross(&cross_yz, &m_inout->y, &m_inout->z);
    det = vec3f_dot(&m_inout->x, &cross_yz);
    if (!fcmp0f(det))
    {
        vec3f_cross(&cross_zx, &m_inout->z, &m_inout->x);
        vec3f_cross(&cross_xy, &m_inout->x, &m_inout->y);
        det_inv = frecipf(det);
        vec3f_set(&m_inout->x, cross_yz.x * det_inv, cross_zx.x * det_inv, cross_xy.x * det_inv);
        vec3f_set(&m_inout->y, cross_yz.y * det_inv, cross_zx.y * det_inv, cross_xy.y * det_inv);
        vec3f_set(&m_inout->z, cross_yz.z * det_inv, cross_zx.z * det_inv, cross_xy.z * det_inv);
    }
    else
    {
        mat3f_clr(m_inout);
        det = 0.0F;
    }
    return det;
}
cat_impl pcvec3f_t mat3f_mulv(pvec3f_t const v_out_unique, pcmat3f_t const m_lh, pcvec3f_t const v_rh)
{
    vec_assert_or_bail(v_out_unique) NULL;
    vec_assert_or_bail(m_lh) NULL;
    vec_assert_or_bail(v_rh) NULL;
    vec_assert_or_bail(v_out_unique != v_rh) NULL;
    vec_assert_or_bail(v_out_unique != &m_lh->x) NULL;
    vec_assert_or_bail(v_out_unique != &m_lh->y) NULL;
    vec_assert_or_bail(v_out_unique != &m_lh->z) NULL;
    return vec3f_set(v_out_unique,
        mat3f_internal_r0mulv(m_lh, v_rh),
        mat3f_internal_r1mulv(m_lh, v_rh),
        mat3f_internal_r2mulv(m_lh, v_rh)
    );
}
cat_impl pcvec3f_t mat3f_tmulv(pvec3f_t const v_out_unique, pcmat3f_t const mt_lh, pcvec3f_t const v_rh)
{
    vec_assert_or_bail(v_out_unique) NULL;
    vec_assert_or_bail(mt_lh) NULL;
    vec_assert_or_bail(v_rh) NULL;
    vec_assert_or_bail(v_out_unique != v_rh) NULL;
    vec_assert_or_bail(v_out_unique != &mt_lh->x) NULL;
    vec_assert_or_bail(v_out_unique != &mt_lh->y) NULL;
    vec_assert_or_bail(v_out_unique != &mt_lh->z) NULL;
    return vec3f_set(v_out_unique,
        vec3f_dot(&mt_lh->x, v_rh),
        vec3f_dot(&mt_lh->y, v_rh),
        vec3f_dot(&mt_lh->z, v_rh)
    );
}
cat_impl float mat3f_imulv(pvec3f_t const v_out_unique, pcmat3f_t const mi_lh, pcvec3f_t const v_rh)
{
    float det = 0.0F;
    float det_inv = 0.0F;
    vec3f_t cross_yz = vec3s(0.0F), cross_zx = vec3s(0.0F), cross_xy = vec3s(0.0F);
    vec_assert_or_bail(v_out_unique) FLT_INVALID;
    vec_assert_or_bail(mi_lh) FLT_INVALID;
    vec_assert_or_bail(v_rh) FLT_INVALID;
    vec_assert_or_bail(v_out_unique != v_rh) FLT_INVALID;
    vec_assert_or_bail(v_out_unique != &mi_lh->x) FLT_INVALID;
    vec_assert_or_bail(v_out_unique != &mi_lh->y) FLT_INVALID;
    vec_assert_or_bail(v_out_unique != &mi_lh->z) FLT_INVALID;
    vec3f_cross(&cross_yz, &mi_lh->y, &mi_lh->z);
    det = vec3f_dot(&mi_lh->x, &cross_yz);
    if (!fcmp0f(det))
    {
        vec3f_cross(&cross_zx, &mi_lh->z, &mi_lh->x);
        vec3f_cross(&cross_xy, &mi_lh->x, &mi_lh->y);
        det_inv = frecipf(det);
        vec3f_set(v_out_unique,
            vec3f_dot(&cross_yz, v_rh) * det_inv,
            vec3f_dot(&cross_zx, v_rh) * det_inv,
            vec3f_dot(&cross_xy, v_rh) * det_inv
        );
    }
    else
    {
        vec3f_clr(v_out_unique);
        det = 0.0F;
    }
    return det;
}

cat_impl pcmat3f_t mat3f_mulm(pmat3f_t const m_out_unique, pcmat3f_t const m_lh, pcmat3f_t const m_rh)
{
    vec_assert_or_bail(m_out_unique) NULL;
    vec_assert_or_bail(m_lh) NULL;
    vec_assert_or_bail(m_rh) NULL;
    vec_assert_or_bail(m_out_unique != m_lh) NULL;
    vec_assert_or_bail(m_out_unique != m_rh) NULL;
    mat3f_mulv(&m_out_unique->x, m_lh, &m_rh->x);
    mat3f_mulv(&m_out_unique->y, m_lh, &m_rh->y);
    mat3f_mulv(&m_out_unique->z, m_lh, &m_rh->z);
    return m_out_unique;
}

cat_impl pcmat3f_t mat3f_tmulm(pmat3f_t const m_out_unique, pcmat3f_t const mt_lh, pcmat3f_t const m_rh)
{
    vec_assert_or_bail(m_out_unique) NULL;
    vec_assert_or_bail(mt_lh) NULL;
    vec_assert_or_bail(m_rh) NULL;
    vec_assert_or_bail(m_out_unique != mt_lh) NULL;
    vec_assert_or_bail(m_out_unique != m_rh) NULL;
    mat3f_tmulv(&m_out_unique->x, mt_lh, &m_rh->x);
    mat3f_tmulv(&m_out_unique->y, mt_lh, &m_rh->y);
    mat3f_tmulv(&m_out_unique->z, mt_lh, &m_rh->z);
    return m_out_unique;
}

cat_impl float mat3f_imulm(pmat3f_t const m_out_unique, pcmat3f_t const mi_lh, pcmat3f_t const m_rh)
{
    float det = 0.0F;
    float det_inv = 0.0F;
    vec3f_t cross_yz = vec3s(0.0F), cross_zx = vec3s(0.0F), cross_xy = vec3s(0.0F);
    vec_assert_or_bail(m_out_unique) FLT_INVALID;
    vec_assert_or_bail(mi_lh) FLT_INVALID;
    vec_assert_or_bail(m_rh) FLT_INVALID;
    vec_assert_or_bail(m_out_unique != mi_lh) FLT_INVALID;
    vec_assert_or_bail(m_out_unique != m_rh) FLT_INVALID;
    vec3f_cross(&cross_yz, &mi_lh->y, &mi_lh->z);
    det = vec3f_dot(&mi_lh->x, &cross_yz);
    if (!fcmp0f(det))
    {
        vec3f_cross(&cross_zx, &mi_lh->z, &mi_lh->x);
        vec3f_cross(&cross_xy, &mi_lh->x, &mi_lh->y);
        det_inv = frecipf(det);
        vec3f_set(&m_out_unique->x,
            vec3f_dot(&cross_yz, &m_rh->x) * det_inv,
            vec3f_dot(&cross_zx, &m_rh->x) * det_inv,
            vec3f_dot(&cross_xy, &m_rh->x) * det_inv
        );
        vec3f_set(&m_out_unique->y,
            vec3f_dot(&cross_yz, &m_rh->y) * det_inv,
            vec3f_dot(&cross_zx, &m_rh->y) * det_inv,
            vec3f_dot(&cross_xy, &m_rh->y) * det_inv
        );
        vec3f_set(&m_out_unique->z,
            vec3f_dot(&cross_yz, &m_rh->z) * det_inv,
            vec3f_dot(&cross_zx, &m_rh->z) * det_inv,
            vec3f_dot(&cross_xy, &m_rh->z) * det_inv
        );
    }
    else
    {
        mat3f_clr(m_out_unique);
        det = 0.0F;
    }
    return det;
}


cat_implementation_end;