#pragma once

#include "../../maths/vec2.h"
#include "../../maths/vec3.h"
#include "../../maths/vec4.h"
#include "../../maths/mat4.h"

namespace daedalus { namespace graphics {

	class Shader
	{
	public:
		virtual ~Shader() {};

		/// @brief sets a Mat4 for a specified uniform location - shader->enable needs to be called first
		virtual void setUniformMat4(const maths::Mat4& matrix, const std::string& name) = 0;

		/// @brief sets 1 int for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform1i(int value, const std::string& name) = 0;

		/// @brief sets an array of ints of specified count at specified uniform location - shader->enable needs to be called first
		virtual void setUniformia(int* values, uint32_t count, const std::string& name) = 0;

		/// @brief sets 1 float for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform1f(float value, const std::string& name) = 0;

		/// @brief sets an array of float of specified count at specified uniform location - shader->enable needs to be called first
		virtual void setUniformfa(float* values, uint32_t count, const std::string& name) = 0;

		/// @brief sets a 2 float vector for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform2f(const maths::Vec2& vector, const std::string& name) = 0;

		/// @brief sets a 3 float vector for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform3f(const maths::Vec3& vector, const std::string& name) = 0;

		/// @brief sets a 4 float vector for a specified uniform location - shader->enable needs to be called first
		virtual void setUniform4f(const maths::Vec4& vector, const std::string& name) = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const std::string& getName() const = 0;

		static Shr_ptr<Shader> create(const std::string& filePath);
		static Shr_ptr<Shader> create(const std::string& name, const std::string& vertex, const std::string& frag);
	};

} }