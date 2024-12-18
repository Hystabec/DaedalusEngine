#pragma once

namespace daedalusCore { namespace graphics {

	class RenderingContext
	{
	public:
		virtual void SwapBuffers() = 0;
	};

} }