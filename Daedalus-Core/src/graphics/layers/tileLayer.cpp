#include "ddpch.h"
#include "tileLayer.h"

#include "graphics/batchRenderer2D.h"

namespace daedalusCore { namespace graphics {

	TileLayer::TileLayer(Shader* shader)
		: Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1, 1))
	{
	}

	TileLayer::~TileLayer()
	{

	}

} }