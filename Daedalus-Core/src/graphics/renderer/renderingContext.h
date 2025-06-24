#pragma once

namespace daedalus { namespace graphics {

	class RenderingContext
	{
	public:
		virtual void swapBuffers() = 0;
	};

} }