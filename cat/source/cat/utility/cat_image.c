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


cat_implementation_begin;


static inline size_t cat_image_internal_index(size_t const image_width, size_t const pos_x, size_t const pos_y)
{
    return (image_width * pos_y + pos_x);
}


cat_impl bool cat_image_create(cat_image_t* const p_image, uint32_t const image_width, uint32_t const image_height)
{
    assert_or_bail(p_image) false;
    assert_or_bail(p_image->image_width == 0) false;
    assert_or_bail(p_image->image_height == 0) false;
    assert_or_bail(p_image->p_image_pixels == NULL) false;
    assert_or_bail(image_width > 0) false;
    assert_or_bail(image_height > 0) false;
    assert_or_bail(image_width <= 32768) false;
    assert_or_bail(image_height <= 32768) false;

    return true;
}

cat_impl bool cat_image_destroy(cat_image_t* const p_image)
{
    assert_or_bail(p_image) false;
    assert_or_bail(p_image->image_width != 0) false;
    assert_or_bail(p_image->image_height != 0) false;
    assert_or_bail(p_image->p_image_pixels != NULL) false;

    return true;
}

cat_impl bool cat_image_save_netpbm(cat_image_t const* const p_image, cstr_t const directory, cstr_t const file_name, bool const using_alpha)
{
    assert_or_bail(p_image) false;
    assert_or_bail(p_image->image_width != 0) false;
    assert_or_bail(p_image->image_height != 0) false;
    assert_or_bail(p_image->p_image_pixels != NULL) false;
    assert_or_bail(directory != NULL) false;
    assert_or_bail(file_name != NULL) false;

    if (using_alpha)
    {
    }
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

    if (all_x && all_y)
    {
        cat_pixel_color_t const* const p_pixel_end = p_image->p_image_pixels + (size_t)p_image->image_width * (size_t)p_image->image_height;
        cat_pixel_color_t* p_pixel = p_image->p_image_pixels;
        while (p_pixel < p_pixel_end)
        {
            *(p_pixel++) = pixel_color;
        }
    }
    else if (all_x)
    {
    }
    else if (all_y)
    {
    }
    else
    {
    }
    return true;
}


#include "cat/utility/cat_time.h"
#include "cat/utility/cat_console.h"

cat_noinl void cat_image_test(void)
{
    cat_time_t const t0 = cat_platform_time();
    cat_time_t t1 = t0;

    cat_console_clear();
    printf("\nImage: \n Draw time: %"PRIi64, (t1 - t0));
    cat_platform_sleep(cat_platform_time_rate());
}


cat_implementation_end;