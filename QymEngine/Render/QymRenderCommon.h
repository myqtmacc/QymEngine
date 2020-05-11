#pragma once

#include "Common/QymCommonHeader.h"

namespace QymEngine {

	enum VertexAttributeLocation
	{
		VERTEX_ATTRIBUTE_LOCATION_POSITION = 0,
		VERTEX_ATTRIBUTE_LOCATION_COLOR = 1,
		VERTEX_ATTRIBUTE_LOCATION_UV0 = 2,
		VERTEX_ATTRIBUTE_LOCATION_UV1 = 3,
	};

	enum class E_INTERSECTION_DETECTION_MODE
	{
		AABB,
		OBB
	};

	const GLint DEFAULT_FRAMEBUFFER_SIZE = 512;

	const char RESOURCE_MODEL_PATH[] = "Models";
	const char RESOURCE_TEXTURE_PATH[] = "Textures";
	const char RESOURCE_SHADER_PATH[] = "Shaders";

	const char globe_shader_vs[] = "uv_map.vs";
	const char globe_shader_fs[] = "texure.ps";

	const char warp_shader_vs[] = "simplest.vs";
	const char warp_shader_fs[] = "warp.ps";

	const char simple_shader_vs[] = "simplest.vs";
	const char simple_shader_fs[] = "texure.ps";

	const char normal_shader_vs[] = "normal.vs";
	const char normal_shader_fs[] = "texure.ps";

	const char lookup_table_shader_vs[] = "simplest.vs";
	const char lookup_table_shader_fs[] = "look_up_table.ps";
}
