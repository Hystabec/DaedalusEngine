#pragma once

#include <FreeImage.h>
#include <GL/glew.h>

namespace daedalusCore { namespace graphics {

	class Texture
	{
	private:
		const char* m_fileName;
		GLuint m_textureID;
		GLsizei m_width, m_height;
	public:
		Texture(const char* fileName);
		~Texture();

		void bind() const;
		void unbind() const;

		inline const unsigned int getWidth() const { return m_width; }
		inline const unsigned int getHeight() const { return m_height; }
		inline const GLuint getID() const { return m_textureID; }

	private:
		GLuint load();
	};

} }