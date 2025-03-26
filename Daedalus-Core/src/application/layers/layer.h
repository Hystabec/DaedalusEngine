#pragma once

#include "events/event.h"
#include "application/deltaTime.h"

namespace daedalusCore { namespace application {

	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void attach() {}
		virtual void detach() {}
		virtual void update(application::DeltaTime dt) {}
		virtual void imGuiRender() {}
		virtual void onEvent(event::Event& event) {}
	};

} }