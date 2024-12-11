#include "texture.h"

#include <FreeImage.h>
#include "utils/imageLoad.h"

namespace daedalusCore { namespace graphics {

		Texture::Texture(const char* fileName) : m_fileName(fileName)
		{
			m_textureID = load();
		}

		Texture::~Texture()
		{
		}

		void Texture::bind() const
		{
			glBindTexture(GL_TEXTURE_2D, m_textureID);
		}

		void Texture::unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0); 
		}

		GLuint Texture::load()
		{
			BYTE* pixels = utils::load_image(m_fileName, &m_width, &m_height);

			GLuint resID;
			glGenTextures(1, &resID);
			glBindTexture(GL_TEXTURE_2D, resID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
			glBindTexture(GL_TEXTURE_2D, 0);

			delete[] pixels;
			return resID;
		}

} }