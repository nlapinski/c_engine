
#ifndef SCENE_H
#define SCENE_H

#pragma once
#include "../components/model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "vector.h"



typedef struct scene_t
{
	Vector objects;
	int cam;

}scene_t;

scene_t *scene_new();
void scene_add_obj(scene_t *self,model_t *model);

#endif /* !SCENE_H */