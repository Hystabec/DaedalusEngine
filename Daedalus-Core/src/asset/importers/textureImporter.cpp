#include "ddpch.h"
#include "textureImporter.h"

#include "../../project/project.h"

#include <stb_image.h>

namespace daedalus {

	IntrusivePtr<graphics::Texture2D> daedalus::TextureImporter::importTexture2D(AssetHandle handle, const AssetMetadata& metadata)
	{
		return loadTexture2D(Project::getActiveAssetDirectory() / metadata.filepath);
	}

	IntrusivePtr<graphics::Texture2D> TextureImporter::loadTexture2D(const std::filesystem::path& path)
	{
		DD_PROFILE_FUNCTION();
		Buffer data;

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		{
			DD_PROFILE_SCOPE("stbi_load - TextureImporter::importTexture2D");
			void* loadedImgPtr = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
			data.replace(Buffer(loadedImgPtr, width * height * channels));
		}

		if (data.data() == nullptr)
		{
			DD_CORE_LOG_ERROR("TextureImporter::importTexture2D - Couldn't load texture from file: {}", path);
			return nullptr;
		}

		graphics::TextureSpecification spec;
		spec.width = width;
		spec.height = height;
		switch (channels)
		{
		case 3:
			spec.format = graphics::ImageFormat::RGB8;
			break;
		case 4:
			spec.format = graphics::ImageFormat::RGBA8;
			break;
		}

		IntrusivePtr<graphics::Texture2D> texture = graphics::Texture2D::create(spec, data);
		data.release();

		return texture;
	}

}