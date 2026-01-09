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
* cat_image.c
* Image implementation.
*/

#include "cat/utility/cat_image.h"
#include "cat/cat_platform.inl"

#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#else // #ifdef _WIN32
#endif // #else // #ifdef _WIN32


//// Define to use column-major processing.
//// Image processing is typically line-by-line, which is row-major.
//#define CAT_IMAGE_USE_COL_MAJOR


cat_implementation_begin;


static inline size_t cat_image_internal_offset_next_row(size_t const image_width, size_t const image_height)
{
#ifdef CAT_IMAGE_USE_COL_MAJOR
    unused2(image_width, image_height);
    return 1;
#else // #ifdef CAT_IMAGE_USE_COL_MAJOR
    unused(image_height);
    return image_width;
#endif // #else // #ifdef CAT_IMAGE_USE_COL_MAJOR
}

static inline size_t cat_image_internal_offset_next_col(size_t const image_width, size_t const image_height)
{
#ifdef CAT_IMAGE_USE_COL_MAJOR
    unused(image_width);
    return image_height;
#else // #ifdef CAT_IMAGE_USE_COL_MAJOR
    unused2(image_width, image_height);
    return 1;
#endif // #else // #ifdef CAT_IMAGE_USE_COL_MAJOR
}

static inline size_t cat_image_internal_offset_end(size_t const image_width, size_t const image_height)
{
    return (image_width * image_height);
}

static inline size_t cat_image_internal_index(size_t const image_width, size_t const image_height, size_t const pos_x, size_t const pos_y)
{
    // Col-major: height * horizontal + vertical.
    // Row-major: width * vertical + horizontal.
    return (
        cat_image_internal_offset_next_row(image_width, image_height) * pos_y +
        cat_image_internal_offset_next_col(image_width, image_height) * pos_x
    );
}


static inline uint16_t cat_image_internal_pixel_color_r(cat_pixel_color_t const pixel_color)
{
    return ((pixel_color >> 48) & 0xFFFF);
}

static inline uint16_t cat_image_internal_pixel_color_g(cat_pixel_color_t const pixel_color)
{
    return ((pixel_color >> 32) & 0xFFFF);
}

static inline uint16_t cat_image_internal_pixel_color_b(cat_pixel_color_t const pixel_color)
{
    return ((pixel_color >> 16) & 0xFFFF);
}

static inline uint16_t cat_image_internal_pixel_color_a(cat_pixel_color_t const pixel_color)
{
    return (pixel_color & 0xFFFF);
}


cat_impl bool cat_image_create(cat_image_t* const p_image, uint32_t const image_width, uint32_t const image_height)
{
    size_t data_size = 0;
    assert_or_bail(p_image) false;
    assert_or_bail(p_image->image_width == 0) false;
    assert_or_bail(p_image->image_height == 0) false;
    assert_or_bail(p_image->p_image_pixels == NULL) false;
    assert_or_bail((image_width > 0) && (image_width <= UINT16_MAX)) false;
    assert_or_bail((image_height > 0) && (image_height <= UINT16_MAX)) false;

    data_size = sizeof(cat_pixel_color_t) * (uint32_t)cat_image_internal_offset_end((uint16_t)image_width, (uint16_t)image_height);
    p_image->p_image_pixels = (cat_pixel_color_t*)malloc(data_size);
    if (p_image->p_image_pixels == NULL)
        return false;
    
    memset(p_image->p_image_pixels, 0x00, data_size);
    p_image->image_width  = image_width;
    p_image->image_height = image_height;

    return true;
}

cat_impl bool cat_image_destroy(cat_image_t* const p_image)
{
    assert_or_bail(p_image) false;
    assert_or_bail(p_image->image_width != 0) false;
    assert_or_bail(p_image->image_height != 0) false;
    assert_or_bail(p_image->p_image_pixels != NULL) false;

    free(p_image->p_image_pixels);
    p_image->p_image_pixels = NULL;
    p_image->image_width    = 0;
    p_image->image_height   = 0;
     
    return true;
}

cat_impl bool cat_image_save_netpbm(cat_image_t const* const p_image, cstr_t const directory, cstr_t const file_name_noext, bool const using_alpha)
{
    char file_path_noext[256] = "";
    char file_path[256] = "";
    FILE* fp = NULL;
    uint32_t row = UINT32_MAX;
    uint32_t col = UINT32_MAX;

    cat_pixel_color_t pixel_color;
    uint16_t rgb[3], a;

    assert_or_bail(p_image) false;
    assert_or_bail(p_image->image_width != 0) false;
    assert_or_bail(p_image->image_height != 0) false;
    assert_or_bail(p_image->p_image_pixels != NULL) false;
    assert_or_bail(directory != NULL) false;
    assert_or_bail(file_name_noext != NULL) false;

    // Ensure directory exists using platform API.
#ifdef _WIN32
    CreateDirectoryA(directory, NULL);
#else // #ifdef _WIN32
#endif // #else // #ifdef _WIN32

    // Create file path without extension.
    snprintf(file_path_noext, sizeof(file_path_noext), "%s/%s", directory, file_name_noext);

    // Create full file path for color image.
    snprintf(file_path, sizeof(file_path), "%s.ppm", file_path_noext);
    fp = fopen(file_path, "wb");
    if (fp == NULL)
        return false;
    
    // PPM binary header.
    fprintf(fp, "P6\n%"PRIu32" %"PRIu32"\n65535\n", p_image->image_width, p_image->image_height);
    for (row = 0; (row < p_image->image_height) != 0; ++row)
    {
        for (col = 0; (col < p_image->image_width) != 0; ++col)
        {
            cat_image_get_pixel(p_image, &pixel_color, col, row);
            rgb[0] = cat_image_internal_pixel_color_r(pixel_color);
            rgb[1] = cat_image_internal_pixel_color_g(pixel_color);
            rgb[2] = cat_image_internal_pixel_color_b(pixel_color);
            fwrite(rgb, sizeof(uint16_t), 3, fp);
        }
    }

    fclose(fp);
    fp = NULL;

    if (!using_alpha)
        return true;

    // Create full file path for alpha image.
    snprintf(file_path, sizeof(file_path), "%s.pgm", file_path_noext);
    fp = fopen(file_path, "wb");
    if (fp == NULL)
        return false;

    // PGM binary header.
    fprintf(fp, "P5\n%"PRIu32" %"PRIu32"\n65535\n", p_image->image_width, p_image->image_height);
    for (row = 0; (row < p_image->image_height) != 0; ++row)
    {
        for (col = 0; (col < p_image->image_width) != 0; ++col)
        {
            cat_image_get_pixel(p_image, &pixel_color, col, row);
            a = cat_image_internal_pixel_color_a(pixel_color);
            fwrite(&a, sizeof(uint16_t), 1, fp);
        }
    }

    fclose(fp);
    fp = NULL;

    return true;
}

cat_impl cat_pixel_color_t cat_image_pixel_color(uint16_t const r, uint16_t const g, uint16_t const b, uint16_t const a)
{
    cat_pixel_color_t pixel_color = a;
    pixel_color |= (cat_pixel_color_t)b << 16;
    pixel_color |= (cat_pixel_color_t)g << 32;
    pixel_color |= (cat_pixel_color_t)r << 48;
    return pixel_color;
}

cat_impl bool cat_image_set_pixel(cat_image_t const* const p_image, int32_t const pos_x, int32_t const pos_y, cat_pixel_color_t const pixel_color)
{
    bool const all_x = (pos_x < 0);
    bool const all_y = (pos_y < 0);
    assert_or_bail(p_image) false;
    assert_or_bail(p_image->image_width != 0) false;
    assert_or_bail(p_image->image_height != 0) false;
    assert_or_bail(p_image->p_image_pixels != NULL) false;
    assert_or_bail(all_x || ((uint32_t)pos_x < p_image->image_width)) false;
    assert_or_bail(all_y || ((uint32_t)pos_y < p_image->image_height)) false;

    if (all_x && all_y)
    {
        size_t const offset_start = 0;
        size_t const offset_end   = cat_image_internal_offset_end(p_image->image_width, p_image->image_height);
        cat_pixel_color_t      *       p_pixel     = &p_image->p_image_pixels[offset_start];
        cat_pixel_color_t const* const p_pixel_end = &p_image->p_image_pixels[offset_end];
        while (p_pixel < p_pixel_end)
        {
            *(p_pixel++) = pixel_color;
        }
    }
    else if (all_x)
    {
        size_t const offset_start = cat_image_internal_index(p_image->image_width, p_image->image_height, 0, pos_y);
        size_t const offset_end   = offset_start + cat_image_internal_offset_next_row(p_image->image_width, p_image->image_height);
        cat_pixel_color_t      *       p_pixel     = &p_image->p_image_pixels[offset_start];
        cat_pixel_color_t const* const p_pixel_end = &p_image->p_image_pixels[offset_end];
        while (p_pixel < p_pixel_end)
        {
            *p_pixel = pixel_color;
            p_pixel += cat_image_internal_offset_next_col(p_image->image_width, p_image->image_height);
        }
    }
    else if (all_y)
    {
        size_t const offset_start = cat_image_internal_index(p_image->image_width, p_image->image_height, pos_x, 0);
        size_t const offset_end   = offset_start + cat_image_internal_offset_next_col(p_image->image_width, p_image->image_height);
        cat_pixel_color_t      *       p_pixel     = &p_image->p_image_pixels[offset_start];
        cat_pixel_color_t const* const p_pixel_end = &p_image->p_image_pixels[offset_end];
        while (p_pixel < p_pixel_end)
        {
            *p_pixel = pixel_color;
            p_pixel += cat_image_internal_offset_next_row(p_image->image_width, p_image->image_height);
        }
    }
    else
    {
        size_t const offset = cat_image_internal_index(p_image->image_width, p_image->image_height, pos_x, pos_y);
        cat_pixel_color_t* const p_pixel = &p_image->p_image_pixels[offset];
        *p_pixel = pixel_color;
    }
    return true;
}

cat_impl bool cat_image_get_pixel(cat_image_t const* const p_image, cat_pixel_color_t* const p_pixel_color, int32_t const pos_x, int32_t const pos_y)
{
    assert_or_bail(p_image) false;
    assert_or_bail(p_image->image_width != 0) false;
    assert_or_bail(p_image->image_height != 0) false;
    assert_or_bail(p_image->p_image_pixels != NULL) false;
    assert_or_bail((pos_x >= 0) && ((uint32_t)pos_x < p_image->image_width)) false;
    assert_or_bail((pos_y >= 0) && ((uint32_t)pos_y < p_image->image_height)) false;
    assert_or_bail(p_pixel_color) false;

    {
        size_t const offset = cat_image_internal_index(p_image->image_width, p_image->image_height, pos_x, pos_y);
        cat_pixel_color_t const pixel = p_image->p_image_pixels[offset];
        *p_pixel_color = pixel;
    }
    return true;
}


#include "cat/utility/cat_time.h"
#include "cat/utility/cat_console.h"
#define _USE_MATH_DEFINES
#include <math.h>

cat_noinl void cat_image_test(void)
{
    cat_time_t t0 = 0;
    cat_time_t t1 = 0;

    uint16_t const image_width  = 2048;
    uint16_t const image_height = 1024;
    cat_image_t image = { 0 };

    cat_console_clear();
    t0 = cat_platform_time();
    cat_image_create(&image, image_width, image_height);
    {
        uint16_t row;
        for (row = 0; (row < image_height) != 0; ++row)
        {
            double   const f_row = (double)row / (double)image_height;
            uint16_t const g     = (uint16_t)(65536.0 * f_row);
            uint16_t const a     = (uint16_t)(65536.0 * (sin(f_row * M_PI * 2.0) * 0.5 + 0.5));

            uint16_t col;
            for (col = 0; (col < image_width) != 0; ++col)
            {
                double   const f_col = (double)col / (double)image_width;
                uint16_t const r     = (uint16_t)(65536.0 * f_col);
                uint16_t const b     = (uint16_t)(65536.0 * (cos(f_col * M_PI * 2.0) * 0.5 + 0.5));

                cat_pixel_color_t const pixel_color = cat_image_pixel_color(r, g, b, a);
                cat_image_set_pixel(&image, col, row, pixel_color);
            }
        }
    }
    cat_image_save_netpbm(&image, "data", "cat_image_test", true);
    cat_image_destroy(&image);
    t1 = cat_platform_time();
    printf("\nImage: \n Draw time: %"PRIi64, (t1 - t0));
    
    cat_platform_sleep(cat_platform_time_rate());
}


cat_implementation_end;