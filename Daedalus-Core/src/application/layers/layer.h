#pragma once

#include "events/event.h"
#include "application/time/deltaTime.h"

namespace daedalus { namespace application {

	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void attach() {}
		virtual void detach() {}
		virtual void update(const application::DeltaTime& dt) {}
		virtual void imGuiRender() {}
		virtual void onEvent(event::Event& event) {}
	};

} }