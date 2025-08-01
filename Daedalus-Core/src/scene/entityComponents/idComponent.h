#pragma once
#include "application/uuid.h"

namespace daedalus::scene {

	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID uuid)
			: ID(uuid)
		{
		};
	};

}