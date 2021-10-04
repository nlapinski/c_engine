#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "../vector/vector.h"


#include <cglm/cglm.h>   /* for inline */
#include <cglm/struct.h>

typedef struct gl_camera
{
  mat4 previous_view;
  mat4 model;
  mat4 inv_model;
  mat4 projection;
  mat4 inv_projection;
  vec3 pos;
  vec3 pivot;
  vec3 look_at;
  vec3 up;
  vec3 dir;
  float x_angle;
  float y_angle;
  float exposure;
  vec3 eye;
}gl_camera;

void update_view(gl_camera *self);
void get_view_dir(gl_camera *self,vec3 view);
void right_dir(gl_camera *self, vec3 right);
void arc_ball(gl_camera *self);

#endif