#pragma once

namespace daedalusCore { namespace graphics {

	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1
	};

	class Renderer
	{
	private:
		static RendererAPI m_rendererAPI;
	public:
		static inline RendererAPI GetCurrentAPI() { return m_rendererAPI; }
	};

} }