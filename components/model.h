#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "vector.h"

#include <cglm/cglm.h>   /* for inline */
#include <cglm/struct.h>


// Information to render each assimp node
typedef struct model_t{

  Vector shapes;
  vec3 pos;
  const char* name[32];
}model_t;



// This is for a shader uniform block
typedef struct material_t{
  float diffuse[4];
  float ambient[4];
  float specular[4];
  float emissive[4];
  float shininess;
  int texCount;
}material_t;

typedef struct mesh_t{

  GLuint vao;
  GLuint texIndex;
  GLuint uniformBlockIndex;
  unsigned num_faces;
  unsigned int *faceArray;
  vec3 pos;
  const char* name[32];
  material_t mat;

}mesh_t;


model_t *model_new();
mesh_t *mesh_new();