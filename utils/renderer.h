
#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include <GL/glew.h>

#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flag
#include "../vector/vector.h"

#include "../components/world.h"
#include "../components/camera.h"


#include <cglm/cglm.h>   /* for inline */
#include <cglm/struct.h>

#include "../systems/scene.h"


typedef struct renderer
{
  int frame;
  uint32_t render_device_frame;
  float resolution;
  int width, height;
  float proj_near, proj_far, proj_fov;
  int scene_changes;
  int outputs_num;
  int passes_size;
  uint32_t stored_camera_frame[6];
  gl_camera glvars[6];
  uint32_t ubos[6];
  uint32_t ubo_changed[6];
  mat4 relative_transform[6];
  vec2 pos[6];
  vec2 size[6];
  GLuint program;
  GLuint vertex_shader, fragment_shader;
  uint8_t *mips;
  uint32_t mips_buffer_size;
  float aspect;
} renderer_t;



renderer_t *renderer_new(float resolution);
int renderer_draw(renderer_t *self);
void renderer_set_resolution(renderer_t *self, float resolution);
void c_render(renderer_t *self,scene_t *scene,world_t *world);
#endif