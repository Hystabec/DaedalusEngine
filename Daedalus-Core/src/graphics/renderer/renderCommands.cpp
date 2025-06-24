#include "ddpch.h"
#include "renderCommands.h"

#ifdef DD_RENDER_USING_OPENGL
#include "platformSpecific/openGL/graphics/renderer/openGLRendererAPI.h"
#endif

namespace daedalus { namespace graphics {

#ifdef DD_RENDER_USING_OPENGL
	RendererAPI* RenderCommands::s_rendererAPI = new OpenGLRendererAPI();
#else
#error Daedalus currently only supports OpenGL
#endif 


} }