#pragma once

#include "../application/uuid.h"
#include "../maths/vec2.h"
#include "../maths/vec3.h"
#include "../maths/vec4.h"

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<daedalus::maths::Vec2>
	{
		static Node encode(const daedalus::maths::Vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, daedalus::maths::Vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<daedalus::maths::Vec3>
	{
		static Node encode(const daedalus::maths::Vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, daedalus::maths::Vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<daedalus::maths::Vec4>
	{
		static Node encode(const daedalus::maths::Vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, daedalus::maths::Vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<daedalus::UUID>
	{
		static Node encode(const daedalus::UUID& rhs)
		{
			Node node;
			node.push_back((uint64_t)rhs);
			return node;
		}

		static bool decode(const Node& node, daedalus::UUID& rhs)
		{
			if (!node.IsScalar())
				return false;

			rhs = node.as<uint64_t>();
			return true;
		}
	};

}