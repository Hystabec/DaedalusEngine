#pragma once

#include "shader.h"

namespace daedalus { namespace graphics {

	class ShaderLibrary
	{
	public:
		void add(const std::string& name, const Shr_ptr<Shader>& shader);
		void add(const Shr_ptr<Shader>& shader);

		Shr_ptr<Shader> load(const std::filesystem::path& filePath);
		Shr_ptr<Shader> load(const std::string& name, const std::filesystem::path& filePath);

		Shr_ptr<Shader> get(const std::string& name);

		bool exists(const std::string& name);
	private:
		std::unordered_map<std::string, Shr_ptr<Shader>> m_shaderMap;
	};

} }