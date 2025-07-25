#include "ddpch.h"
#include "openGLShader.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "utils/fileUtils.h"
#include "utils/timer.h"
#include "maths/maths.h"

namespace daedalus { namespace graphics {

	namespace utils
	{
		static GLenum shaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			DD_CORE_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("Unknown shader type: {}", type));
			return 0;
		}

		static shaderc_shader_kind glShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			DD_CORE_ASSERT(false);
			return (shaderc_shader_kind)0;
		}

		static const char* glShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			DD_CORE_ASSERT(false);
			return nullptr;
		}


		static const char* getCacheDirectory()
		{
			return "resources/cache/shaders/opengl";
		}

		static void createCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = getCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* glShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
			}
			DD_CORE_ASSERT(false);
			return "";
		}

		static const char* glShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
			}
			DD_CORE_ASSERT(false);
			return "";
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
		: m_filePath(filePath)
	{
		DD_PROFILE_FUNCTION();

		utils::createCacheDirectoryIfNeeded();

		bool wasReadCorrectly = true;
		std::string shaderSrc = daedalus::utils::read_file(filePath, &wasReadCorrectly);
		
		if(!wasReadCorrectly)
		{
			DD_CORE_LOG_ERROR("Failed to read shader file : path - {}", filePath);
			m_name = "Invalid shader";
			m_shaderID = 0;
		}

		DD_CORE_ASSERT(wasReadCorrectly, "Invalid Shader");
		auto shaderSources = preProcess(shaderSrc);

		{
			daedalus::utils::Timer timer;
			compileOrGetVulkanBinaries(shaderSources);
			compileOrGetOpenGLBinaries();
			createProgram();
			DD_CORE_LOG_WARN("Shader creation took {} ms", timer.elapsedMilliseconds());
		}

		// Getting name from file path
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment)
		: m_name(name)
	{
		DD_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertex;
		sources[GL_FRAGMENT_SHADER] = fragment;

		compileOrGetVulkanBinaries(sources);
		compileOrGetOpenGLBinaries();
		createProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		DD_PROFILE_FUNCTION();

		glDeleteProgram(m_shaderID);
	}

	void OpenGLShader::setUniformMat4(const maths::Mat4& matrix, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.elements);
	}

	void OpenGLShader::setUniform1i(int value, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform1i(getUniformLocation(name), value);
	}

	void OpenGLShader::setUniformia(int* values, uint32_t count, const std::string& name)
	{
		glUniform1iv(getUniformLocation(name), count, values);
	}

	void OpenGLShader::setUniform1f(float value, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform1f(getUniformLocation(name), value);
	}

	void OpenGLShader::setUniformfa(float* values, uint32_t count, const std::string& name)
	{
		glUniform1fv(getUniformLocation(name), count, values);
	}

	void OpenGLShader::setUniform2f(const maths::Vec2& vector, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform2f(getUniformLocation(name), vector.x, vector.y);
	}

	void OpenGLShader::setUniform3f(const maths::Vec3& vector, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
	}

	void OpenGLShader::setUniform4f(const maths::Vec4& vector, const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::bind() const
	{
		DD_PROFILE_FUNCTION();

		glUseProgram(m_shaderID);
	}

	void OpenGLShader::unbind() const
	{
		DD_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source)
	{
		DD_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			DD_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			DD_CORE_ASSERT(utils::shaderTypeFromString(type), "Invalid shader type specifier");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			DD_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);
			shaderSources[utils::shaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		//GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_4);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = utils::getCacheDirectory();

		auto& shaderData = m_vulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_filePath;
			std::filesystem::path cachePath = cacheDirectory / (shaderFilePath.filename().string() + utils::glShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachePath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				DD_ASSERT(!m_filePath.empty(), "File path is invalid");
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, utils::glShaderStageToShaderC(stage), m_filePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					DD_CORE_LOG_ERROR(module.GetErrorMessage());
					DD_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachePath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			reflect(stage, data);
	}

	void OpenGLShader::compileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_openGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = utils::getCacheDirectory();

		shaderData.clear();
		m_openGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_vulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_filePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + utils::glShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_openGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_openGLSourceCode[stage];

				DD_ASSERT(!m_filePath.empty(), "File path is invalid");
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, utils::glShaderStageToShaderC(stage), m_filePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					DD_CORE_LOG_ERROR(module.GetErrorMessage());
					DD_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::createProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shadersIDs;
		for (auto&& [stage, spirv] : m_openGLSPIRV)
		{
			GLuint shaderID = shadersIDs.emplace_back(glCreateShader(stage));

			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), (GLsizei)(spirv.size() * sizeof(uint32_t)));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);

			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			if (length)
			{
				std::vector<GLchar> infoLog(length);
				glGetProgramInfoLog(program, length, &length, infoLog.data());
				DD_CORE_LOG_ERROR("Failed to link shader | {}", infoLog.data());
				DD_CORE_ASSERT(false, "Shader failed to link");
			}
			
			DD_CORE_LOG_ERROR("Failed to link shader");

			glDeleteProgram(program);

			for (auto id : shadersIDs)
				glDeleteShader(id);
		}

		for (auto id : shadersIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_shaderID = program;
	}

	void OpenGLShader::reflect(GLenum stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		DD_CORE_LOG_INFO("OpenGLShader::Reflect - {} {}", utils::glShaderStageToString(stage), m_filePath);
		DD_CORE_LOG_INFO("{} uniform buffers", resources.uniform_buffers.size());
		DD_CORE_LOG_INFO("{} resources", resources.sampled_images.size());

		if(!(resources.uniform_buffers.empty()))
			DD_CORE_LOG_INFO("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = (uint32_t)compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = (int)bufferType.member_types.size();

			DD_CORE_LOG_INFO("    {}", resource.name);
			DD_CORE_LOG_INFO("    Size = {}", bufferSize);
			DD_CORE_LOG_INFO("    Binding = {}", binding);
			DD_CORE_LOG_INFO("    Members = {}", memberCount);
		}
	}

	GLint OpenGLShader::getUniformLocation(const std::string& name)
	{
		DD_PROFILE_FUNCTION();

		return glGetUniformLocation(m_shaderID, name.c_str());
	}

	//GLuint OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	//{
	//	DD_PROFILE_FUNCTION();

	//	GLuint program = glCreateProgram();

	//	DD_CORE_ASSERT(shaderSources.size() <= 3, "Only supports up to 3 shaders");
	//	std::array<GLenum, 3> glShaderIDs;

	//	int glShaderIDIndex = 0;
	//	for (auto& kv : shaderSources)
	//	{
	//		GLenum type = kv.first;
	//		const std::string& src = kv.second;

	//		GLuint shader = glCreateShader(type);

	//		const GLchar* srcCstr = src.c_str();
	//		glShaderSource(shader, 1, &srcCstr, 0);

	//		glCompileShader(shader);

	//		GLint compileResult;
	//		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	//		if (compileResult == GL_FALSE)
	//		{
	//			GLint length;
	//			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	//			std::vector<GLchar> error(length);
	//			glGetShaderInfoLog(shader, length, &length, &error[0]);

	//			DD_CORE_LOG_ERROR("Failed to compile shader | {}", &error[0]);
	//			//DD_CORE_ASSERT(false, "Shader failed to compile");

	//			glDeleteShader(shader);
	//			break;
	//		}

	//		glAttachShader(program, shader);
	//		glShaderIDs[glShaderIDIndex++] = shader;
	//	}

	//	glLinkProgram(program);

	//	GLint isLinked = 0;
	//	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	//	if (isLinked == GL_FALSE)
	//	{
	//		GLint length = 0;
	//		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

	//		
	//		std::vector<GLchar> infoLog(length);
	//		if (length)
	//		{
	//			glGetProgramInfoLog(program, length, &length, &infoLog[0]);
	//			DD_CORE_LOG_ERROR("Failed to link shader | {}", &infoLog[0]);
	//			DD_CORE_ASSERT(false, "Shader failed to link");
	//		}
	//		

	//		DD_CORE_LOG_ERROR("Failed to link shader");
	//		DD_CORE_ASSERT(false, "Shader failed to link");

	//		glDeleteProgram(program);

	//		while (glShaderIDIndex > 0)
	//		{
	//			glDeleteShader(glShaderIDs[glShaderIDIndex-1]);
	//			glShaderIDIndex--;
	//		}

	//		return 0;
	//	}

	//	//glValidateProgram(program);

	//	while (glShaderIDIndex > 0)
	//	{
	//		glDetachShader(program, glShaderIDs[glShaderIDIndex-1]);
	//		glDeleteShader(glShaderIDs[glShaderIDIndex-1]);
	//		glShaderIDIndex--;
	//	}

	//	return program;
	//}

} }