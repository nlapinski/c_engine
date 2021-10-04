#pragma once
#ifndef WORLD_H
#define WORLD_H
#include "../vector/vector.h"
#include "../components/camera.h"

#include <cglm/cglm.h>   /* for inline */
#include <cglm/struct.h>

typedef struct world_t{

  vec3 cam_pos;
  vec3 cam_target;
  vec3 cam_dir;

  vec3 player_pos;
  vec3 player_dir;

  vec3 player_heading;

  vec3 player_view_dir;

  float player_accel;
  vec3 sun_pos;
  vec3 bg_clr;
  vec3 fog_clr;
  int width, height;

  float cx_angle;
  float cy_angle;

  float r_x_axis;
  float r_y_axis;

  float l_x_axis;
  float l_y_axis;


}world_t;




world_t *world_new();
void update_world(world_t *world,gl_camera *camera);
#endif