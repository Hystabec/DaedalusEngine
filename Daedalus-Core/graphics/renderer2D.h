#pragma once

#include <GL/glew.h>
#include "../maths/maths.h"
#include "renderable2D.h"

namespace daedalusCore { namespace graphics {

	class Renderer2D
	{
	protected:
		virtual void submit(const Renderable2D* renderable) = 0;
		virtual void flush() = 0;
	};

} }