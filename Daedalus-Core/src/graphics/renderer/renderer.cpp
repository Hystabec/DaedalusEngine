#include "ddpch.h"
#include "renderer.h"

namespace daedalusCore { namespace graphics {

#ifdef DD_RENDER_USING_OPENGL
	RendererAPI Renderer::m_rendererAPI = RendererAPI::OpenGL; 
#else
	#error Daedalus currently only supports OpenGL
#endif 
	
} }