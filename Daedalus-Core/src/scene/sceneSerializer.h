#pragma once
#include "scene.h"

namespace daedalus::scene {

	class SceneSerializer
	{
	public:
		SceneSerializer(const IntrusivePtr<Scene>& scene);

		/// @brief Serialize into yaml (Plain Text)
		void serialize(const std::filesystem::path& filepath);
		/// @brief Serialize into binary
		void serializeRuntime(const std::filesystem::path& filepath);

		/// @brief Serialize from yaml (Plain Text)
		bool deserialize(const std::filesystem::path& filepath);
		/// @brief Serialize from binary
		bool deserializeRuntime(const std::filesystem::path& filepath);
	private:
		IntrusivePtr<Scene> m_scene;

		static constexpr uint32_t m_currentFileFormatVersion = 1;
	};

}