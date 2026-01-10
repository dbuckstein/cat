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

/*! \file cat_console.h
*   \brief Console interface.
*/

#ifndef _CAT_CONSOLE_H_
#define _CAT_CONSOLE_H_


#include "cat/cat_platform.h"


cat_interface_begin;


//! \enum cat_console_color_e
//! \brief Enumeration of console colors.
typedef enum cat_console_color_e
{
    cat_console_0,                    // Color is disabled.
    cat_console_b,                    // Color contains blue channel.
    cat_console_g,                    // Color contains green channel.
    cat_console_gb,                   // Color contains green and blue channels.
    cat_console_r,                    // Color contains red channel.
    cat_console_br,                   // Color contains red and blue channels.
    cat_console_rg,                   // Color contains red and green channels
    cat_console_rgb,                  // Color contains red and green and blue channels
    cat_console_a,                    // Color contains opacity channel.
    cat_console_black = cat_console_0,// Color dark black:   0,0,0,0.
    cat_console_blueDark,             // Color dark blue:    0,0,0,1.
    cat_console_greenDark,            // Color dark green:   0,0,1,0.
    cat_console_cyanDark,             // Color dark cyan:    0,0,1,1.
    cat_console_redDark,              // Color dark red:     0,1,0,0.
    cat_console_magentaDark,          // Color dark magenta: 0,1,0,1.
    cat_console_yellowDark,           // Color dark yellow:  0,1,1,0.
    cat_console_gray,                 // Color gray:         0,1,1,1.
    cat_console_grayDark,             // Color dark gray:    1,0,0,0.
    cat_console_blue,                 // Color blue:         1,0,0,1.
    cat_console_green,                // Color green:        1,0,1,0.
    cat_console_cyan,                 // Color cyan:         1,0,1,1.
    cat_console_red,                  // Color red:          1,1,0,0.
    cat_console_magenta,              // Color magenta:      1,1,0,1.
    cat_console_yellow,               // Color yellow:       1,1,1,0.
    cat_console_white,                // Color white:        1,1,1,1.
} cat_console_color_t;


//! \fn cat_console_create
//! \brief Create main console window.
//! \return True if successful.
cat_decl bool cat_console_create(void);

//! \fn cat_console_destroy
//! \brief Destroy main console window.
//! \return True if successful.
cat_decl bool cat_console_destroy(void);

//! \fn cat_console_config
//! \brief Configure main console window.
//! \param in Flag to redirect input stream.
//! \param out Flag to redirect output stream.
//! \param err Flag to redirect error stream.
//! \return True if successful.
cat_decl bool cat_console_config(bool const in, bool const out, bool const err);

//! \fn cat_console_toggle_cursor
//! \brief Toggle cursor in main console window.
//! \param visible Flag to enable cursor visibility.
//! \return True if successful.
cat_decl bool cat_console_toggle_cursor(bool const visible);

//! \fn cat_console_get_pos
//! \brief Get cursor position in main console window.
//! \param p_x_out Pointer to result horizontal coordinate (left to right).
//! \param p_y_out Pointer to result vertical coordinate (top to bottom).
//! \return True if successful.
cat_decl bool cat_console_get_pos(int16_t* const p_x_out, int16_t* const p_y_out);

//! \fn cat_console_set_pos
//! \brief Set cursor position in main console window.
//! \param x Horizontal coordinate (left to right).
//! \param y Vertical coordinate (top to bottom).
//! \return True if successful.
cat_decl bool cat_console_set_pos(int16_t const x, int16_t const y);

//! \fn cat_console_get_color
//! \brief Get foreground and background colors of main console window at cursor.
//! \param p_fg_out Pointer to result foreground color (text).
//! \param p_bg_out Pointer to result background color (window).
//! \return True if successful.
cat_decl bool cat_console_get_color(cat_console_color_t* const p_fg_out, cat_console_color_t* const p_bg_out);

//! \fn cat_console_set_color
//! \brief Set foreground and background colors of main console window at cursor.
//! \param fg Foreground color (text).
//! \param bg Background color (window).
//! \return True if successful.
cat_decl bool cat_console_set_color(cat_console_color_t const fg, cat_console_color_t const bg);

//! \fn cat_console_reset_color
//! \brief Reset foreground and background colors of main console window at cursor to default.
//! \return True if successful.
cat_decl bool cat_console_reset_color(void);

//! \fn cat_console_get_pos_color
//! \brief Get cursor position and foreground and background colors of main console window at cursor.
//! \param p_x_out Pointer to result horizontal coordinate (left to right).
//! \param p_y_out Pointer to result vertical coordinate (top to bottom).
//! \param p_fg_out Pointer to result foreground color (text).
//! \param p_bg_out Pointer to result background color (window).
//! \return True if successful.
cat_decl bool cat_console_get_pos_color(int16_t* const p_x_out, int16_t* const p_y_out, cat_console_color_t* const p_fg_out, cat_console_color_t* const p_bg_out);

//! \fn cat_console_set_pos_color
//! \brief Set cursor position and foreground and background colors of main console window at cursor.
//! \param x Horizontal coordinate (left to right).
//! \param y Vertical coordinate (top to bottom).
//! \param fg Foreground color (text).
//! \param bg Background color (window).
//! \return True if successful.
cat_decl bool cat_console_set_pos_color(int16_t const x, int16_t const y, cat_console_color_t const fg, cat_console_color_t const bg);

//! \fn cat_console_get_size
//! \brief Get size of drawable area in main console window.
//! \param p_w_out Pointer to result horizontal dimension (width).
//! \param p_h_out Pointer to result vertical dimension (height).
//! \return True if successful.
cat_decl bool cat_console_get_size(int16_t* const p_w_out, int16_t* const p_h_out);

//! \fn cat_console_set_size
//! \brief Set size of drawable area in main console window.
//! \param w Horizontal dimension (width).
//! \param h Vertical dimension (height).
//! \return True if successful.
cat_decl bool cat_console_set_size(int16_t const w, int16_t const h);

//! \fn cat_console_clear
//! \brief Clear main console window.
//! \return True if successful.
cat_decl bool cat_console_clear(void);

//! \fn cat_console_draw_test_patch
//! \brief Draw preset diagnostic test patch in main console window.
//! \return True if successful.
cat_decl bool cat_console_draw_test_patch(void);

//! \fn cat_console_debug_print
//! \brief Print to IDE debug output if available.
//! \param format Standard IO format c-string.
//! \param ... Variadic arguments aligning with parameters declared in \a format.
//! \return Number of characters printed.
cat_decl int cat_console_debug_print(cstr_t const format, ...);


cat_interface_end;


#endif // #ifndef _CAT_CONSOLE_H_