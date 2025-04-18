#pragma once

#include "../../maths/vec2.h"
#include "../../maths/vec3.h"
#include "../../maths/vec4.h"
#include "../../maths/mat4.h"

namespace daedalusCore { namespace graphics {

	class Shader
	{
	public:
		virtual ~Shader() {};

		/// @brief sets a mat4 for a specified uniform location - shader->enable needs to be called first
		virtual void setUniformMat4(const maths::mat4& matrix, const std::string& name) = 0;

		/// @brief sets 1 int for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform1i(int value, const std::string& name) = 0;

		/// @brief sets an array of ints of specified count at specified uniform location - shader->enable needs to be called first
		virtual void setUniformia(int* values, uint32_t count, const std::string& name) = 0;

		/// @brief sets 1 float for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform1f(float value, const std::string& name) = 0;

		/// @brief sets an array of float of specified count at specified uniform location - shader->enable needs to be called first
		virtual void setUniformfa(float* values, uint32_t count, const std::string& name) = 0;

		/// @brief sets a 2 float vector for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform2f(const maths::vec2& vector, const std::string& name) = 0;

		/// @brief sets a 3 float vector for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform3f(const maths::vec3& vector, const std::string& name) = 0;

		/// @brief sets a 4 float vector for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform4f(const maths::vec4& vector, const std::string& name) = 0;

		virtual void enable() const = 0;
		virtual void disable() const = 0;

		virtual const std::string& getName() const = 0;

		static shr_ptr<Shader> create(const std::string& filePath);
		static shr_ptr<Shader> create(const std::string& name, const std::string& vertex, const std::string& frag);
	};

} }