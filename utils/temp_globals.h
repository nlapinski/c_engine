
#pragma once
#ifndef TEMP_GLOBALS_H
#define TEMP_GLOBALS_H


// Vertex Attribute Locations
GLuint vertexLoc=0, normalLoc=1, texCoordLoc=2;

// Uniform Bindings Points
GLuint matricesUniLoc = 1, materialUniLoc = 2;

// The sampler uniform for textured models
// we are assuming a single texture so this will
//always be texture unit 0
GLuint texUnit = 0;
#endif