#pragma once

#include "layer.h"

//example implementation of the Layer

namespace daedalusCore { namespace graphics {

	class TileLayer : public Layer
	{
	public:
		TileLayer(Shader* shader);
		~TileLayer();
	};

} }