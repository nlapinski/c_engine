#pragma once
#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../components/model.h"


#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flag


typedef struct loader_t
{
	//const C_STRUCT aiScene* scene;
	model_t* load;

}loader_t;

loader_t *loader_new(const char* file, const char* obj_name);


#endif /* !MESH_LOADER_H */