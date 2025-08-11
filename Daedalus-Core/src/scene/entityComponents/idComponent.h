#pragma once
#include "application/uuid.h"

namespace daedalus::scene {

	struct IDComponent
	{
		UUID ID;
		std::string name;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID uuid, const std::string& name)
			: ID(uuid), name(name)
		{
		};
	};

}