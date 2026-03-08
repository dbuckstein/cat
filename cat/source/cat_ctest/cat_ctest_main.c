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
* cat_ctest_main.c
* Console program entry point.
*/

#include "cat/cat.h"

#include "cat/cat_platform.inl"

// Disable QSpectre mitigation only if it becomes a real problem!
#ifdef _WIN32
#ifdef CAT_RELEASE
//#pragma warning(disable: 5045)
#endif // #ifdef CAT_RELEASE
#endif // #ifdef _WIN32


// ****TO-DO: OPTIONAL
// 0) Check your settings:
//  -> Release build config is faster.
//  -> For testing, reduce image resolution; use full size for final output.
//  -> If "QSpectre mitigation" warnings occur, uncomment warning hack above.
//  -> Use a program such as GIMP for previewing resulting images (leave open).


////////////////////////////////////////////////////////////////////////////////
/// SETTINGS

#define USING_ANTIALIASING
#define USING_ANTIALIASING_RANDOM

///
////////////////////////////////////////////////////////////////////////////////


#ifdef USING_ANTIALIASING
#ifdef USING_ANTIALIASING_RANDOM
#define MSAA_KERNEL_SAMPLES 256
static vec2f_t msaa_kernel_lens[MSAA_KERNEL_SAMPLES];
static vec2f_t msaa_kernel_view[array_count(msaa_kernel_lens)];
#else // #ifdef USING_ANTIALIASING_RANDOM
static vec2f_t const msaa_kernel_lens[] = {
    { +0.50F,  0.00F },
    { +0.40F, +0.25F },
    { +0.35F, +0.35F },
    { +0.25F, +0.40F },
    {  0.00F, +0.50F },
    { -0.25F, +0.40F },
    { -0.35F, +0.35F },
    { -0.40F, +0.25F },
    { -0.50F,  0.00F },
    { -0.40F, -0.25F },
    { -0.35F, -0.35F },
    { -0.25F, -0.40F },
    {  0.00F, -0.50F },
    { +0.25F, -0.40F },
    { +0.35F, -0.35F },
    { +0.40F, -0.25F },
};
static vec2f_t msaa_kernel_view[array_count(msaa_kernel_lens)];
#endif // #else // #ifdef USING_ANTIALIASING_RANDOM
#else // #ifdef USING_ANTIALIASING
static vec2f_t const msaa_kernel_lens[1] = { vec2s(0.0F) };
static vec2f_t const msaa_kernel_view[1] = { vec2s(0.0F) };
#endif // #else // #ifdef USING_ANTIALIASING
static_assert(array_count(msaa_kernel_lens) == array_count(msaa_kernel_view), "Sampling kernels have differing sizes.");


cat_decl void vec3f_color(uint16_t* const pr, uint16_t* const pg, uint16_t* const pb, pcvec3f_t const v)
{
    assert_or_bail(pr);
    assert_or_bail(pg);
    assert_or_bail(pb);
    assert_or_bail(v);
    assert_or_bail(pr != pg);
    assert_or_bail(pg != pb);
    assert_or_bail(pb != pr);
    *pr = (uint16_t)(fclampf(v->x, 0.0F, 1.0F) * 65535.9F);
    *pg = (uint16_t)(fclampf(v->y, 0.0F, 1.0F) * 65535.9F);
    *pb = (uint16_t)(fclampf(v->z, 0.0F, 1.0F) * 65535.9F);
}


typedef struct scalar_s
{
    real_t x;
    real_t xx;
    real_t _x;
    real_t _xx;
} scalar_t;
cat_decl void scalar_init(scalar_t* const p_scalar, real_t const x)
{
    assert(p_scalar);
    p_scalar->x = x;
    p_scalar->xx = fsqf(x);
    p_scalar->_x = frecipf(p_scalar->x);
    p_scalar->_xx = frecipf(p_scalar->xx);
}


////////////////////////////////////////////////////////////////////////////////
/// RAY DEFINITION

// Ray descriptor.
typedef struct ray_s
{
    vec3_t origin;
    vec3_t direction;
    scalar_t magnitude;
} ray_t;
cat_decl void ray_init_direct(ray_t* const p_ray, vec3_t const* const p_origin, vec3_t const* const p_direction)
{
    assert(p_origin);
    assert(p_direction);
    vec3f_cpy(&p_ray->origin, p_origin);
    vec3f_cpy(&p_ray->direction, p_direction);

    //// Arbitrary length.
    //scalar_init(&p_ray->magnitude, vec3f_len(&p_ray->direction));

    // Unit length.
    vec3f_normalize_inplace(&p_ray->direction);
    scalar_init(&p_ray->magnitude, 1.0F);
}
cat_decl void ray_init_target(ray_t* const p_ray, vec3_t const* const p_origin, vec3_t const* const p_target)
{
    assert(p_origin);
    assert(p_target);
    vec3f_cpy(&p_ray->origin, p_origin);
    vec3f_sub(&p_ray->direction, p_target, p_origin);

    //// Arbitrary length.
    //scalar_init(&p_ray->magnitude, vec3f_len(&p_ray->direction));

    // Unit length.
    vec3f_normalize_inplace(&p_ray->direction);
    scalar_init(&p_ray->magnitude, 1.0F);
}
cat_decl void ray_compute_position(pvec3_t const p_position_out, ray_t const* const p_ray, real_t const param)
{
    assert(p_position_out);
    assert(p_ray);
    vec3f_mad(p_position_out, &p_ray->origin, &p_ray->direction, param);
}
static real_t const ray_param_min = DBL2RE(0.0001);
static real_t const ray_param_max = DBL2RE(1000.0);


///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// OBJECT DEFINITIONS

typedef struct object_s
{
    mat3_t rotate;
    vec3_t center;
} object_t;

// Initialize object in world given:
//  -> yaw and pitch angles in degrees
//  -> position in north/east/down global frame aligning with local forward/right/down
cat_decl bool object_init_yaw_pitch_ned_frd(object_t* const p_obj_out,
    real_t const yaw_deg, real_t const pitch_deg,
    real_t const north, real_t const east, real_t const down
)
{
    real_t const yaw_rad = fdeg2radf(yaw_deg);
    real_t const pitch_rad = fdeg2radf(pitch_deg);
    real_t sy = 0.0F, cy = 0.0F, sp = 0.0F, cp = 0.0F;
    assert_or_bail(p_obj_out) false;

    sy = fsincosf(&cy, yaw_rad);
    sp = fsincosf(&cp, pitch_rad);

    // Yaw-pitch rotation construction in NED frame:
    //                 { +cy  -sy  0 }   { +cp  0  +sp }
    // Ryp = Ry * Rp = { +sy  +cy  0 } * {  0   1   0  }
    //                 {  0    0   1 }   { -sp  0  +cp }
    // 
    //    { +cy*cp  -sy  +cy*sp }
    //  = { +sy*cp  +cy  +sy*sp }
    //    {    -sp   0      +cp }
    vec3f_set(&p_obj_out->rotate.x, +cy * cp, +sy * cp, -sp);
    vec3f_set(&p_obj_out->rotate.y, -sy, +cy, 0.0F);
    vec3f_set(&p_obj_out->rotate.z, +cy * sp, +sy * sp, +cp);

    vec3f_set(&p_obj_out->center, north, east, down);

    return true;
}

// Convert vector in global frame (XYZ=FRD=NED) to image frame (XYZ=RDF).
cat_decl pcvec3f_t object_ned2img(pvec3f_t const v_img_out, pcvec3f_t const v_ned)
{
    assert_or_bail(v_img_out) NULL;
    assert_or_bail(v_ned) NULL;
    // { R }   { 0  1  0 }   { F }
    // { D } = { 0  0  1 } * { R }
    // { F }   { 1  0  0 }   { D }
    return vec3f_set(v_img_out, v_ned->y, v_ned->z, v_ned->x);
}

// Convert vector in image frame (XYZ=RDF) to global frame (XYZ=FRD=NED).
cat_decl pcvec3f_t object_img2ned(pvec3f_t const v_ned_out, pcvec3f_t const v_img)
{
    assert_or_bail(v_ned_out) NULL;
    assert_or_bail(v_img) NULL;
    // { F }   { 0  0  1 }   { R }
    // { R } = { 1  0  0 } * { D }
    // { D }   { 0  1  0 }   { F }
    return vec3f_set(v_ned_out, v_img->z, v_img->x, v_img->y);
}


// Ray hit solution.
typedef struct ray_hit_s
{
    vec3_t position;
    vec3_t normal;
    vec3_t tangent;
} ray_hit_t;
cat_decl bool hemisphere_tangent(vec3_t* const v_tangent_out, vec3_t const* const v_normal, ray_t const* const p_ray, object_t const* const p_object)
{
    vec3_t cross;
    assert(v_tangent_out);
    assert(v_normal);
    assert(p_ray);
    assert(p_object);

    // If incident ray is parallel to normal, choose object basis.
    vec3f_cross(&cross, v_normal, &p_ray->direction);
    if ((vec3f_lensq(&cross) <= FLT_EPSILON) != 0)
    {
        // If first basis is parallel to normal, choose second.
        vec3f_cross(&cross, v_normal, &p_object->rotate.x);
        if ((vec3f_lensq(&cross) <= FLT_EPSILON) != 0)
        {
            // Select second basis.
            vec3f_ortho(v_tangent_out, v_normal, &p_object->rotate.y);
        }
        else
        {
            // Select first basis.
            vec3f_ortho(v_tangent_out, v_normal, &p_object->rotate.x);
        }
    }
    else
    {
        // Select incident ray.
        vec3f_ortho(v_tangent_out, v_normal, &p_ray->direction);
    }

    // Negate and normalize result.
    vec3f_resize_inplace(v_tangent_out, -1.0F);

    // Done.
    return true;
}
cat_decl void hemisphere_basis(mat3f_t* const m_basis, ray_hit_t const* const p_ray_hit)
{
    assert(m_basis);
    assert(p_ray_hit);
    m_basis->z = p_ray_hit->normal;
    m_basis->x = p_ray_hit->tangent;
    vec3f_cross(&m_basis->y, &m_basis->z, &m_basis->x);
}


///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// SHAPE DEFINITIONS

typedef struct sphere_s
{
    vec3_t origin;
    scalar_t radius;
} sphere_t;
cat_decl void sphere_init(sphere_t* const p_sphere, vec3_t const* const p_origin, real_t const radius)
{
    assert(p_sphere);
    assert(p_origin);
    p_sphere->origin = *p_origin;
    scalar_init(&p_sphere->radius, fabsf(radius));
}


typedef struct sphere_ray_data_s
{
    real_t param_near;
    real_t param_far;
} sphere_ray_data_t;

cat_decl bool sphere_test_ray(sphere_ray_data_t* const p_data_out, ray_t const* const p_ray, sphere_t const* const p_sphere)
{
    assert_or_bail(p_data_out) false;
    assert_or_bail(p_ray) false;
    assert_or_bail(p_sphere) false;

    // ****TO-DO: REQUIRED
    // 1) Solve sphere-ray intersection.
    //  -> Feel free to extend data structure above if needed.
    //  -> Draw a diagram to visualize the problem or write your process here.
    //  -> Hint: begin with the mathematical condition describing when a point
    //      on the sphere and a point on the ray are equal.
    {
        // |P(t) - Q| = r
        // |P(0) + pt - Q|^2 = r^2
        //  s = P(0) - Q
        // (s + pt).(s + pt) = r^2
        // s.s + 2s.pt + p.pt^2 - r^2 = 0
        // 0 = (p.p)t^2 + 2(p.s)t + ((s.s) - r^2)
        // t = (-b +/- sqrt(b^2 - 4ac))/2a
        //  a = p.p
        //  b = 2p.s
        //  c = s.s - r^2
        // t = (-2p.s +/- sqrt(4(p.s)^2 - 4(p.p)(s.s - r^2)))/2(p.p)
        // t_near = (p.(-s) - sqrt((p.s)^2 - (p.p)(s.s - r^2)))/(p.p)

        vec3_t s;
        real_t a, b, c, d;

        vec3f_sub(&s, &p_sphere->origin, &p_ray->origin);
        a = p_ray->magnitude.xx;
        b = vec3f_dot(&p_ray->direction, &s);
        c = vec3f_lensq(&s) - p_sphere->radius.xx;
        d = b * b - c;

        if (d < 0.0F)
            return false;

        d = fsqrtf(d);
        p_data_out->param_near = (b - d) / a;
        p_data_out->param_far  = (b + d) / a;
        
        return true;
    }

    //unused(p_data_out);
    //unused(p_ray);
    //unused(p_sphere);
    //return false;
}
cat_decl void sphere_compute_normal(vec3_t* const v_normal_out, vec3_t const* const v_position, sphere_t const* const p_sphere)
{
    assert_or_bail(v_normal_out);
    assert_or_bail(v_position);
    assert_or_bail(p_sphere);

    vec3f_sub(v_normal_out, v_position, &p_sphere->origin);
    vec3f_mul(v_normal_out, v_normal_out, p_sphere->radius._x);
}


// Scene descriptor.
typedef struct scene_s
{
    object_t camera_global;//< Camera object in global space.

    object_t pointlight_global;//< Point light object in global space.
    object_t pointlight_camera;//< Point light object in camera space.

    object_t shapes_global[4];//< Shape objects in global space.
    object_t shapes_camera[4];//< Shape objects in camera space.
    sphere_t sphere_scene[4]; //< Shape descriptors.
} scene_t;

void scene_init(scene_t* const p_scene)
{
    object_t* obj_camera_itr = p_scene->shapes_camera;
    object_t const* const obj_camera_end = obj_camera_itr + array_count(p_scene->shapes_camera);
    object_t const* obj_global_itr = p_scene->shapes_global;
    vec3f_t tmp;

    assert_or_bail(p_scene);

    float const planet_radius = 10000.0F;
    float const sphere1_radius = 3.0F;
    float const sphere2_radius = 6.0F;
    float const sphere3_radius = 1.0F;

    // Object positioning in world.
    object_init_yaw_pitch_ned_frd(&p_scene->shapes_global[0], 0.0F, 0.0F, 0.0F, 0.0F, 0.0F);//< Planet.
    object_init_yaw_pitch_ned_frd(&p_scene->shapes_global[1], 0.0F, 0.0F, 0.0F, +12.0F, -(planet_radius + sphere1_radius));
    object_init_yaw_pitch_ned_frd(&p_scene->shapes_global[2], 0.0F, 0.0F, 0.0F, 0.0F, -(planet_radius + sphere2_radius));
    object_init_yaw_pitch_ned_frd(&p_scene->shapes_global[3], 0.0F, 0.0F, 0.0F, -12.0F, -(planet_radius + sphere3_radius));

    // Point light positioning in world.
    object_init_yaw_pitch_ned_frd(&p_scene->pointlight_global, 0.0F, 0.0F, 0.0F, 0.0F, -(planet_radius + 50.0F));

    // Camera positioning in world.
    object_init_yaw_pitch_ned_frd(&p_scene->camera_global, 45.0F, -15.0F, -25.0F, -25.0F, -(planet_radius + 15.0F));

    // Convert objects to camera space.
    while (obj_camera_itr != obj_camera_end)
    {
        assert((obj_camera_itr - p_scene->shapes_camera) == (obj_global_itr - p_scene->shapes_global));
        mat3f_tmulm(&obj_camera_itr->rotate, &p_scene->camera_global.rotate, &obj_global_itr->rotate);
        vec3f_sub(&tmp, &obj_global_itr->center, &p_scene->camera_global.center);
        mat3f_tmulv(&obj_camera_itr->center, &p_scene->camera_global.rotate, &tmp);
        ++obj_camera_itr;
        ++obj_global_itr;
    }

    // Convert point light to camera space.
    {
        mat3f_tmulm(&p_scene->pointlight_camera.rotate, &p_scene->camera_global.rotate, &p_scene->pointlight_global.rotate);
        vec3f_sub(&tmp, &p_scene->pointlight_global.center, &p_scene->camera_global.center);
        mat3f_tmulv(&p_scene->pointlight_camera.center, &p_scene->camera_global.rotate, &tmp);
    }

    // Generate shapes based on final target space.
    sphere_init(&p_scene->sphere_scene[0], &p_scene->shapes_camera[0].center, planet_radius);
    sphere_init(&p_scene->sphere_scene[1], &p_scene->shapes_camera[1].center, sphere1_radius);
    sphere_init(&p_scene->sphere_scene[2], &p_scene->shapes_camera[2].center, sphere2_radius);
    sphere_init(&p_scene->sphere_scene[3], &p_scene->shapes_camera[3].center, sphere3_radius);
}


cat_decl bool ray_vs_scene(ray_hit_t* const p_ray_hit, ray_t const* const p_ray, scene_t const* const p_scene)
{
    real_t param = ray_param_max;
    int idx_hit = -1;

    int const n_sphere = array_count(p_scene->shapes_camera);
    int i;

    assert_or_bail(p_ray_hit) false;
    assert_or_bail(p_ray) false;
    assert_or_bail(p_scene) false;

    // Iterate spheres.
    for (i = 0; (i < n_sphere) != 0; ++i)
    {
        sphere_ray_data_t data;
        if (!sphere_test_ray(&data, p_ray, &p_scene->sphere_scene[i]))
            continue;
        if (!frangef(data.param_near, ray_param_min, param))
            continue;

        // Save hit.
        param = data.param_near;
        idx_hit = i;
    }

    // Check if anything hit.
    if (idx_hit == -1)
        return false;

    // Resolve hit.
    ray_compute_position(&p_ray_hit->position, p_ray, param);
    sphere_compute_normal(&p_ray_hit->normal, &p_ray_hit->position, &p_scene->sphere_scene[idx_hit]);
    hemisphere_tangent(&p_ray_hit->tangent, &p_ray_hit->normal, p_ray, &p_scene->shapes_camera[idx_hit]);

    // Done.
    return true;
}


cat_decl void randv_sphere_sweep(vec3f_t* const v_out, real_t const azim_sweep_deg, real_t const elev_sweep_deg)
{
    real_t const azim = frandf() * azim_sweep_deg;
    real_t const elev = frandf() * elev_sweep_deg;
    real_t sa, ca, se, ce;

    assert(v_out);

    sa = fsincosf(&ca, fdeg2radf(azim));
    se = fsincosf(&ce, fdeg2radf(elev));
    vec3f_set(v_out, se * ca, se * sa, ce);
}
cat_decl void randv_sphere(vec3f_t* const v_out)
{
    assert(v_out);
    randv_sphere_sweep(v_out, 360.0F, 180.0F);
}
cat_decl void randv_hemisphere(vec3f_t* const v_out)
{
    assert(v_out);
    randv_sphere_sweep(v_out, 360.0F, 90.0F);
}
cat_decl void randv_lambertian(vec3f_t* const v_out)
{
    assert(v_out);
    randv_sphere(v_out);
    v_out->z += 1.0F;
}
cat_decl void ray_hemisphere(ray_t* const p_ray, ray_hit_t const* const p_ray_hit)
{
    vec3f_t hemisphere_direction;
    vec3f_t ray_direction;
    mat3f_t basis;
    assert(p_ray);
    assert(p_ray_hit);
    hemisphere_basis(&basis, p_ray_hit);

    randv_hemisphere(&hemisphere_direction);
    mat3f_mulv(&ray_direction, &basis, &hemisphere_direction);
    ray_init_direct(p_ray, &p_ray_hit->position, &ray_direction);
}
cat_decl void ray_lambertian(ray_t* const p_ray, ray_hit_t const* const p_ray_hit)
{
    vec3f_t hemisphere_direction;
    vec3f_t ray_direction;
    mat3f_t basis;
    assert(p_ray);
    assert(p_ray_hit);
    hemisphere_basis(&basis, p_ray_hit);

    randv_lambertian(&hemisphere_direction);
    mat3f_mulv(&ray_direction, &basis, &hemisphere_direction);
    ray_init_direct(p_ray, &p_ray_hit->position, &ray_direction);
}
cat_decl void brdf_default(vec3f_t* const brdf_out, vec3f_t const* const v_position, vec3f_t const* const v_normal, vec3f_t const* const v_view, vec3f_t const* const v_light)
{
    // ****TO-DO: REQUIRED
    // 2) Compute local output color.
    //  -> Use desired combination of the input variables:
    //      "v_position" - hit position
    //      "v_normal"   - normal at hit position
    //      "v_view"     - view or target direction (outbound light direction, points away from position)
    //      "v_light"    - light or source direction (inbound light direction, points towards source)
    //  -> You may implement additional support functions.
    //  -> Bonus opportunities for creativity.
    {
        vec3_t albedo;
        real_t dot_nl;

        // Use normal for albedo.
        real_t const r = fmadf(0.5F, v_normal->x, 0.5F);
        real_t const g = fmadf(0.5F, v_normal->y, 0.5F);
        real_t const b = fmadf(0.5F, v_normal->z, 0.5F);
        vec3f_set(&albedo, r, g, b);

        // Lambertian coefficient: k_diffuse = (n.l)
        dot_nl = vec3f_dot(v_normal, v_light);

        // Result.
        vec3f_mul(brdf_out, &albedo, dot_nl);

        unused(v_position);
        unused(v_view);
    }

    //// TEST
    //{
    //    real_t test = v_position->x * 0.01F;
    //    vec3f_set(brdf_out, test, test, test);
    //}

    //unused(brdf_out);
    //unused2(v_position, v_normal);
    //unused2(v_view, v_light);
}


cat_decl void color_bg(vec3f_t* const p_color_out, ray_t const* const p_ray)
{
    assert(p_color_out);
    assert(p_ray);

    {
        float const r = 0.5F;
        float const g = 0.5F;
        float const b = fmadf(0.5F, fclampf(p_ray->direction.z * -2.0F, -1.0F, +1.0F), 0.5F);
        vec3f_set(p_color_out, r, g, b);
    }
}

cat_decl void trace_ray_vs_scene(vec3f_t* const color_out, ray_t const* const p_ray, scene_t const* const p_scene, uint32_t const recursive_depth)
{
    // ****TO-DO: REQUIRED
    // 3) Implement the trace algorithm.
    //  -> Test scene; generate color if hit, otherwise output background.
    //  -> This may be recursive.
    //  -> Bonus opportunities for creativity.
    {
        color_bg(color_out, p_ray);
        if (recursive_depth == 0)
            return;
    }

    // TEST
    {
        ray_hit_t ray_hit;
        if (ray_vs_scene(&ray_hit, p_ray, p_scene))
        {
            ray_t ray;
            ray_lambertian(&ray, &ray_hit);
            brdf_default(color_out, &ray_hit.position, &ray_hit.normal, NULL, &ray.direction);
        }
    }

    //unused(color_out);
    //unused(p_ray);
    //unused(p_scene);
    //unused(recursive_depth);
    //color_bg(color_out, p_ray);
}


///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// SOLUTION

static inline size_t progress_begin(char const progress_name[])
{
    size_t i;
    printf("\n%s:\n", progress_name);
    for (i = 0; (i <= 100) != 0; ++i)
        printf(" ");
    printf("]\r[");
    return 0;
}
static inline size_t progress_print(size_t const progress, size_t const progress_count, size_t const progress_total)
{
    size_t i;
    size_t const updated = (100 * progress_count) / progress_total;
    if ((updated > progress) != 0)
    {
        for (i = progress; (i < updated) != 0; ++i)
            printf(">");
    }
    return updated;
}
static inline void progress_end(size_t const progress)
{
    size_t i;
    for (i = progress; (i < 100) != 0; ++i)
        printf(" ");
    printf("]%"PRIu64"%%\n", (uint64_t)progress);
}

cat_noinl void cat_raytracing_image(void)
{
    cat_time_t t0 = 0;
    cat_time_t t1 = 0;

    cat_image_t image = { 0 };
    cat_viewport_t viewport = { 0 };
    float const viewport_height = 1.5F;
    float const viewport_distance = 2.0F;

    uint16_t row;
    uint16_t col;
    float rowf;
    float colf;
    size_t progress;
    size_t progress_count;
    size_t progress_total;

    bool const using_defocus_lens = true;
    float const defocus_lens_radius = using_defocus_lens ? 1.0F : 0.0F;
    float focal_distance = 0.0F;

    // Objects and shapes.
    scene_t scene;
    static_assert(array_count(scene.shapes_global) == array_count(scene.shapes_camera), "Array count mismatch.");
    static_assert(array_count(scene.shapes_global) == array_count(scene.sphere_scene), "Array count mismatch.");


    // Ray.
    ray_t ray;
    uint32_t const recursive_depth = 10;

    // Sampling kernels.
    uint16_t pixel_sample = 0;
    uint16_t const samples_per_pixel = array_count(msaa_kernel_lens);
#ifdef USING_ANTIALIASING
#ifdef USING_ANTIALIASING_RANDOM
    for (pixel_sample = 0; (pixel_sample < samples_per_pixel) != 0; ++pixel_sample)
    {
        real_t const radial_lens = frandf();
        real_t const angle_lens = fdeg2radf(frandf() * 360.0F);
        msaa_kernel_lens[pixel_sample].x = cosf(angle_lens) * radial_lens;
        msaa_kernel_lens[pixel_sample].y = sinf(angle_lens) * radial_lens;
        msaa_kernel_view[pixel_sample].x = frandf() - 0.5F;
        msaa_kernel_view[pixel_sample].y = frandf() - 0.5F;
    }
#else // #ifdef USING_ANTIALIASING_RANDOM
    memcpy(msaa_kernel_view, msaa_kernel_lens, sizeof(msaa_kernel_lens));
#endif // #else // #ifdef USING_ANTIALIASING_RANDOM
#else // #ifdef USING_ANTIALIASING
#endif // #else // #ifdef USING_ANTIALIASING


    // Set up scene.
    scene_init(&scene);

    // Focus object.
    focal_distance = fabsf(scene.shapes_camera[2].center.x);//< Choose forward distance to focus object.


    // Start rendering.
    cat_console_clear();
    t0 = cat_platform_time();
    cat_image_create(&image, 1280, 720);
    cat_viewport_init(&viewport, viewport_height, cat_image_aspect(&image), viewport_distance);

    progress = progress_begin("cat ray-tracing image");
    progress_count = 0;
    progress_total = image.image_height * image.image_width;

    // Iterate rows.
    for (row = 0; (row < image.image_height) != 0; ++row)
    {
        rowf = (float)row + 0.5F;

        // Iterate columns.
        for (col = 0; (col < image.image_width) != 0; ++col)
        {
            colf = (float)col + 0.5F;

            // Compute color.
            {
                // Final color.
                uint16_t r = 0, g = 0, b = 0;
                uint16_t const a = 0;

                // Total color.
                vec3_t pixel_color_total = vec3s(INT2RE(0));

                // Iterate samples.
                for (pixel_sample = 0; (pixel_sample < samples_per_pixel) != 0; ++pixel_sample)
                {
                    // This sample color.
                    vec3_t pixel_color_sample = vec3s(INT2RE(0));

                    vec3f_t const lens_pos_img = vec3(
                        msaa_kernel_lens[pixel_sample].x * defocus_lens_radius,
                        msaa_kernel_lens[pixel_sample].y * defocus_lens_radius,
                        0.0F
                    );
                    vec3f_t const frag_pos_img = vec3(
                        colf + msaa_kernel_view[pixel_sample].x,
                        rowf + msaa_kernel_view[pixel_sample].y,
                        0.0F
                    );
                    vec3f_comp_t frag_pos_near;
                    vec3f_t frag_pos_proj;
                    vec3f_t frag_pos_view;
                    vec3f_t lens_pos_view;

                    cat_viewport_pos_precise(frag_pos_near.v, &viewport, &image, frag_pos_img.x, frag_pos_img.y);
                    frag_pos_proj.x = frag_pos_near.c.x * focal_distance / frag_pos_near.c.z;
                    frag_pos_proj.y = frag_pos_near.c.y * focal_distance / frag_pos_near.c.z;
                    frag_pos_proj.z = focal_distance;
                    object_img2ned(&frag_pos_view, &frag_pos_proj);
                    object_img2ned(&lens_pos_view, &lens_pos_img);

                    // Generate ray.
                    ray_init_target(&ray, &lens_pos_view, &frag_pos_view);

                    // Trace.
                    trace_ray_vs_scene(&pixel_color_sample, &ray, &scene, recursive_depth);

                    // Accumulate color.
                    vec3f_add(&pixel_color_total, &pixel_color_total, &pixel_color_sample);
                }

                // Average color.
                vec3f_div(&pixel_color_total, &pixel_color_total, (real_t)samples_per_pixel);

                // Range correction.
                pixel_color_total.x = fsqrtf(pixel_color_total.x);
                pixel_color_total.y = fsqrtf(pixel_color_total.y);
                pixel_color_total.z = fsqrtf(pixel_color_total.z);

                // Convert color to integer representation.
                vec3f_color(&r, &g, &b, &pixel_color_total);

                // Output final pixel color.
                cat_image_set_pixel(&image, col, row, cat_image_pixel_color(r, g, b, a));

                // Progress bar.
                progress = progress_print(progress, ++progress_count, progress_total);
            }
        }
    }
    progress_end(progress);

    cat_image_save_netpbm(&image, "data", "cat_raytracing_image", false);
    cat_image_destroy(&image);
    t1 = cat_platform_time();
    printf("\nImage: \n Draw time: %"PRIi64, (t1 - t0));

    cat_platform_sleep(cat_platform_time_rate());
}


//extern int cat_test_all(int const argc, char const* const argv[]);
int cat_test_raytracing(int const argc, char const* const argv[])
{
    cat_raytracing_image();
    unused2(argc, argv);
    return 0;
}

int main(int const argc, char const* const argv[])
{
    int result = 0;
#ifdef _WIN32
    _set_error_mode(_OUT_TO_MSGBOX);
#endif // #ifdef _WIN32
    result |= cat_test_raytracing(argc, argv);
    //result |= cat_test_all(argc, argv);
    return result;
}
