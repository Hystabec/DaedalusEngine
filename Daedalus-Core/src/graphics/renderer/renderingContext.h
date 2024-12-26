#pragma once

namespace daedalusCore { namespace graphics {

	class RenderingContext
	{
	public:
		virtual void swapBuffers() = 0;
	};

} }