////////////////////////////////////////////////////////////////////////////////
/// Copyright 2025 Daniel S. Buckstein
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

/*! \file cat_image.h
*   \brief Image interface.
*/

#ifndef _CAT_IMAGE_H_
#define _CAT_IMAGE_H_


#include "cat/cat_platform.h"


cat_interface_begin;


//! \typedef cat_pixel_color_t
//! \brief Encoded pixel color.
typedef uint64_t cat_pixel_color_t;


//! \struct cat_image_s
//! \brief Image descriptor.
typedef struct cat_image_s
{
    uint32_t           image_width;   //< Width of image in pixels.
    uint32_t           image_height;  //< Height of image in pixels.
    cat_pixel_color_t* p_image_pixels;//< Pixel data.
} cat_image_t;


//! \fn cat_image_create
//! \brief Allocate image data, setup descriptor.
//! \param p_image Pointer to unused image descriptor (all fields are zero).
//! \param image_width Width of image in pixels in range [0,32768].
//! \param image_Height Height of image in pixels in range [0,32768].
//! \return True if image created successfully.
cat_decl bool cat_image_create(cat_image_t* const p_image, uint32_t const image_width, uint32_t const image_height);

//! \fn cat_image_destroy
//! \brief Deallocate image data, reset descriptor.
//! \param p_image Pointer to active image descriptor (all fields are non-zero).
//! \return True if image destroyed successfully.
cat_decl bool cat_image_destroy(cat_image_t* const p_image);

//! \fn cat_image_save_netpbm
//! \brief Save image(s) in convenient NetPBM format(s).
//! \param p_image Pointer to active image descriptor (all fields are non-zero).
//! \param directory Save directory c-string (not including file name).
//! \param file_name Save file name c-string (not including file path).
//! \param using_alpha Alpha flag, true if alpha channel should be saved.
//! \return True if image saved successfully.
cat_decl bool cat_image_save_netpbm(cat_image_t const* const p_image, cstr_t const directory, cstr_t const file_name, bool const using_alpha);

//! \fn cat_image_pixel_color
//! \brief Create encoded pixel color given individual channels.
//! \param r Red channel (16-bit).
//! \param g Green channel (16-bit).
//! \param b Blue channel (16-bit).
//! \param a Alpha channel (16-bit).
cat_decl cat_pixel_color_t cat_image_pixel_color(uint16_t const r, uint16_t const g, uint16_t const b, uint16_t const a);

//! \fn cat_image_set_pixel
//! \brief Set color for pixel(s) in image.
//! \param p_image Pointer to active image descriptor (all fields are non-zero).
//! \param pos_x Horizontal position of pixel in image; zero is left edge. Pass negative value for all columns.
//! \param pos_y Vertical position of pixel; zero is top edge. Pass negative value for all rows.
//! \param pixel_color Encoded color of pixel.
//! \return True if pixel set successfully.
cat_decl bool cat_image_set_pixel(cat_image_t const* const p_image, int32_t const pos_x, int32_t const pos_y, cat_pixel_color_t const pixel_color);


cat_interface_end;


#endif // #ifndef _CAT_IMAGE_H_