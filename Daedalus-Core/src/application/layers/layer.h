#pragma once

#include "events/event.h"

namespace daedalusCore { namespace application {

	class  Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void Attach() {}
		virtual void Detach() {}
		virtual void Update() {}
		virtual void ImGuiRender() {}
		virtual void OnEvent(event::Event& event) {}
	};

} }