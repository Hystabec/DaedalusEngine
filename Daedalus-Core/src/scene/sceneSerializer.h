#pragma once
#include "scene.h"

namespace daedalus::scene {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Shr_ptr<Scene>& scene);

		/// @brief Serialize into yaml (Plain Text)
		void serialize(const std::string& filepath);
		/// @brief Serialize into binary
		void serializeRuntime(const std::string& filepath);

		/// @brief Serialize from yaml (Plain Text)
		bool deserialize(const std::string& filepath);
		/// @brief Serialize from binary
		bool deserializeRuntime(const std::string& filepath);
	private:
		Shr_ptr<Scene> m_scene;
	};

}