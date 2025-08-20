#pragma once

#include <string>

namespace daedalus::scene {

	struct ScriptComponent
	{
		std::string className;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

}