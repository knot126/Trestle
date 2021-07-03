/**
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Wrekt Configutation File
 */

#pragma once

// Include any required includes here...
#include "util/maths.h"

// Define what data types will be used for the engine
// Vectors must be able to be accessed by using .data[i] where x->i=0 ... w->i=3
// Matrices must be able to be accessed by using .data[i][j] = n, where j is a
// column and i is a row. (Ie. row major storage)
typedef DgVec2 wr_vec2_t;
typedef DgVec3 wr_vec3_t;
typedef DgVec4 wr_vec4_t;

typedef DgMat2 wr_mat2_t;
typedef DgMat3 wr_mat3_t;
typedef DgMat4 wr_mat4_t;

// Reference types
typedef wr_vec2_t * wr_vec2p_t;
typedef wr_vec3_t * wr_vec3p_t;
typedef wr_vec4_t * wr_vec4p_t;

typedef wr_mat2_t * wr_mat2p_t;
typedef wr_mat3_t * wr_mat3p_t;
typedef wr_mat4_t * wr_mat4p_t;
