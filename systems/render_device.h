#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <GL/glew.h>

struct gl_light
{
	//vec3_t color;
	float volumetric;
	//ivec2_t pos;
	uint32_t lod;
	float radius;
};

struct gl_scene
{
	struct gl_light lights[62];
	//vec3_t test_color;
	//
	//
	float time;
};

struct gl_bones
{
	//mat4_t bones[30];
};

typedef struct c_render_device
{


	//fs_t *frag_bound;
	//shader_t *shader;

	uint32_t ubo;
	struct gl_scene scene;
	int32_t updates_ram;
	int32_t updates_ubo;
	int32_t frame;
	uint32_t update_frame;
	//bool_t cull_invert;
	uint32_t bound_ubos[32];

} c_render_device_t;


c_render_device_t *c_render_device_new(void);

void world_changed(void);
void c_render_device_bind_ubo(c_render_device_t *self, uint32_t base, uint32_t ubo);

#endif /* !RENDER_DEVICE_H */