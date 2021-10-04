#include "render_device.h"

#define true  1
#define false 0


void c_render_device_update_ubo(c_render_device_t *self)
{
	if(!self->ubo)
	{
		glGenBuffers(1, &self->ubo);// glerr();
		glBindBuffer(GL_UNIFORM_BUFFER, self->ubo);// glerr();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(self->scene), &self->scene,
				GL_STATIC_DRAW); //glerr();
		glBindBufferBase(GL_UNIFORM_BUFFER, 20, self->ubo);// glerr();
		c_render_device_bind_ubo(self, 20, self->ubo);
	}
	/* else if(self->scene_changes) */
	else
	{
		glBindBuffer(GL_UNIFORM_BUFFER, self->ubo);// glerr();
		/* GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY); glerr(); */
		/* memcpy(p, &self->scene, sizeof(self->scene)); */

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(self->scene), &self->scene);//glerr();

		/* glUnmapBuffer(GL_UNIFORM_BUFFER); glerr(); */

		self->updates_ubo = 0;
	}
}


void c_render_device_bind_ubo(c_render_device_t *self, uint32_t base,uint32_t ubo)
{
	self->bound_ubos[base] = ubo;
}

int c_render_device_update(c_render_device_t *self)
{
	if(self->updates_ram)
	{
		//c_render_device_update_lights(self);
		//self->scene.test_color = vec3(0, 1, 0);
		self->scene.time = ((float)self->frame) * 0.01f;
		self->updates_ubo = true;
		self->updates_ram = false;
	}
	return 1;
}


c_render_device_t *c_render_device_new()
{
	c_render_device_t *self;
  	self = malloc(sizeof(c_render_device_t));
  	c_render_device_update(self);
  	self->updates_ubo = 1;
	
	//svp_init();

	return self;
}



int c_render_device_draw(c_render_device_t *self)
{
	if(self->updates_ubo)
	{
		c_render_device_update_ubo(self);
		self->update_frame++;
	}
	self->frame++;
	//entity_signal(entity_null, sig("world_pre_draw"), NULL, NULL);

	return 1;
}

