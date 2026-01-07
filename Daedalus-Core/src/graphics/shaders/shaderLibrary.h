#pragma once

#include "shader.h"

namespace daedalus { namespace graphics {

	class ShaderLibrary
	{
	public:
		void add(const std::string& name, const IntrusivePtr<Shader>& shader);
		void add(const IntrusivePtr<Shader>& shader);

		IntrusivePtr<Shader> load(const std::filesystem::path& filePath);
		IntrusivePtr<Shader> load(const std::string& name, const std::filesystem::path& filePath);

		IntrusivePtr<Shader> get(const std::string& name);

		bool exists(const std::string& name);
	private:
		std::unordered_map<std::string, IntrusivePtr<Shader>> m_shaderMap;
	};

} }