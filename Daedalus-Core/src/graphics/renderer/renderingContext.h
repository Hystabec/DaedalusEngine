#pragma once

namespace daedalus { namespace graphics {

	class RenderingContext
	{
	public:
		virtual ~RenderingContext() = default;

		virtual void swapBuffers() = 0;
	};

} }