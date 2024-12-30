#include "ddpch.h"
#include "rendererAPI.h"

namespace daedalusCore { namespace graphics {

#ifdef DD_RENDER_USING_OPENGL
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#else
	#error Daedalus currently only supports OpenGL
#endif 
	
} }