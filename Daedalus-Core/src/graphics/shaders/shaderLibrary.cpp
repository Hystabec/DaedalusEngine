#include "ddpch.h"
#include "shaderLibrary.h"

namespace daedalus { namespace graphics {

	void ShaderLibrary::add(const std::string& name, const Shr_ptr<Shader>& shader)
	{
		DD_CORE_ASSERT(!exists(name), DD_ASSERT_FORMAT_MESSAGE("Shader '{}' already exists", name));
		m_shaderMap[name] = shader;
	}
	void ShaderLibrary::add(const Shr_ptr<Shader>& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}

	Shr_ptr<Shader> ShaderLibrary::load(const std::filesystem::path& filePath)
	{
		auto shader = Shader::create(filePath);
		add(shader);
		return shader;
	}

	Shr_ptr<Shader> ShaderLibrary::load(const std::string& name, const std::filesystem::path& filePath)
	{
		auto shader = Shader::create(filePath);
		add(name, shader);
		return shader;
	}

	Shr_ptr<Shader> ShaderLibrary::get(const std::string& name)
	{
		DD_CORE_ASSERT(exists(name), DD_ASSERT_FORMAT_MESSAGE("Shader '{}' not found", name));
		return m_shaderMap[name];
	}

	bool ShaderLibrary::exists(const std::string& name)
	{
		return m_shaderMap.find(name) != m_shaderMap.end();
	}

} }