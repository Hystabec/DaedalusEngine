#pragma once

#include "events/event.h"

namespace daedalusCore { namespace application {

	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void attach() {}
		virtual void detach() {}
		virtual void update() {}
		virtual void imGuiRender() {}
		virtual void onEvent(event::Event& event) {}
	};

} }