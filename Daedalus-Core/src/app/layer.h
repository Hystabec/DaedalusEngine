#pragma once

#include "events/event.h"

namespace daedalusCore { namespace app {

	class DD_API Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void Attach() {}
		virtual void Detach() {}
		virtual void Update() {}
		virtual void Event(event::Event& event) {}
	};

} }