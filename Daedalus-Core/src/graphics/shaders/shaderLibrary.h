#pragma once

#include "shader.h"

namespace daedalusCore { namespace graphics {

	class ShaderLibrary
	{
	public:
		void add(const std::string& name, const shr_ptr<Shader>& shader);
		void add(const shr_ptr<Shader>& shader);

		shr_ptr<Shader> load(const std::string& filePath);
		shr_ptr<Shader> load(const std::string& name, const std::string& filePath);

		shr_ptr<Shader> get(const std::string& name);

		bool exists(const std::string& name);
	private:
		std::unordered_map<std::string, shr_ptr<Shader>> m_shaderMap;
	};

} }