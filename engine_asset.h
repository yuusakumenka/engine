/* Copyright (c) 2024 Yuusaku Menka
 * Licensed under the Apache license, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0) */

/* This part of the engine isn't supported yet. All here is but hopes and dreams. :') */

/* 	Asset module:
 * 		Asset Metadata is static/compile time, generated .c file asset_generated.c
 *      If ASSET_GENERATED is not defined, use dynamic asset.
 *      asset_load_<asset_kind>_job
 *  What is an asset?
 *  	A large binary + slim metadata.
 * 	What is metatada?
 * 		Data about how should the file be loaded / interpreted.
 * 		The minimum for a raw binary would be { size, offset }.
 * 	How is metadata accessed?
 * 		Trough it's type and unique string id.
 * 			* constant variable name when using ASSET_GENERATED
 * 			* hashed to CRC64 when not using ASSET_GENERATED
 * 	Why do I need to precise the metata type since it is known at compile time?
 * 		It is the price to pay so that your code also work whith dynamic assets.
 * 		
 */



//#define ENGINE_USE_COMPILE_TIME_METADATA

typedef void* Asset;
typedef void* Asset_Id;
typedef void* Asset_Ref;

typedef struct Asset_Info {
	Asset_Id id;
	void*    data;
} Asset_Info;

typedef struct Metadata_Base {
	Uint num_bytes;
	Uint offset;
} Metadata_Base;

void *asset_map(Metadata_Base *metadata_base);

// example: void *data = asset_map(METADATA(my_cool_asset_sid));

// void asset_load_from_memory(Metadata* metadata, void *data) {
	
// }

void *asset_read(
	Arena              *dst_arena,
	const char         *path,
	unsigned long long *size
);

typedef struct Generated_Asset_Metadata_Database Generated_Asset_Metadata_Database;


typedef U8 Asset_Mesh_Metadata_Vertex_Attribute_Semantic;
typedef enum Asset_Mesh_Metadata_Vertex_Attribute_Semantic_ {
	ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_SEMANTIC_POSITION,
	ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_SEMANTIC_NORMAL,
	ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_SEMANTIC_COUNT,
} Asset_Mesh_Metadata_Vertex_Attribute_Semantic_;

typedef U8 Asset_Mesh_Metadata_Vertex_Attribute_Format;
typedef enum Asset_Mesh_Metadata_Vertex_Attribute_Format_ {
	ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_FORMAT_V3F16,
	ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_FORMAT_V2F16,
	ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_FORMAT_COUNT,
} Asset_Mesh_Metadata_Vertex_Attribute_Format_;

typedef struct Asset_Mesh_Metadata_Vertex_Attribute {
	Asset_Mesh_Metadata_Vertex_Attribute_Semantic semantic;
	Asset_Mesh_Metadata_Vertex_Attribute_Format   format;
} Asset_Mesh_Metadata_Vertex_Attribute;

typedef struct  Asset_Mesh_Metadata {
	Uint                                  num_vertex_attributes;
	Asset_Mesh_Metadata_Vertex_Attribute *vertex_attributes;
} Asset_Mesh_Metadata;

void asset_mesh_metadata_serialize(void *data_dst, Asset_Mesh_Metadata *metadata_src) {

}

void asset_mesh_metadata_deserialize(Asset_Mesh_Metadata *metadata_dst, void *data_src) {
	
}

/* generated*/
static const Asset_Mesh_Metadata_Vertex_Attribute METADATA_models_basic_rig_vertex_attributes[] = {
	{ 
		.format  = ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_SEMANTIC_POSITION,
		.semantic = ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_FORMAT_V3F16
	},
	{ 
		.format  = ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_SEMANTIC_POSITION,
		.semantic = ASSET_MESH_METADATA_VERTEX_ATTRIBUTE_FORMAT_V3F16
	},
};
static const Asset_Mesh_Metadata METADATA_models_basic_rig = {
	.num_vertex_attributes = 5,
	.vertex_attributes = &METADATA_models_basic_rig_vertex_attributes
};

void* metadata_dynamic_get(Asset_Ref asset_ref) {
	// Uint metadata_index = table_indexof(table, asset_ref);
	// if(metadata_index != ((Uint)-1))
	// 	return metadata_pointers[metadata_index];
	// table_get(table, asset_ref);
	// void *metadata;
	// if(indexof(table, asset_ref);)
}

#ifndef ENGINE_USE_COMPILE_TIME_METADATA
#define METADATA(type, asset_ref) ((type*)metadata_dynamic_get(#asset_ref))
#else /* ENGINE_USE_DYNAMIC_METADATA */
#define METADATA(type, asset_ref) (&METADATA##_##asset_ref)
#endif /* !ENGINE_USE_COMPILE_TIME_METADATA */

void metadata_demo() {
	METADATA(Asset_Mesh_Metadata, models_basic_rig)->vertex_attributes[0].format;
	if(METADATA_models_basic_rig.num_vertex_attributes) {

	}
}
