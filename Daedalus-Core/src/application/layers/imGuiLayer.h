#pragma once

#include "layer.h"

#include "events/keyEvent.h"
#include "events/mouseEvent.h"
#include "events/windowEvent.h"

namespace daedalusCore { namespace application {

	class DD_API ImGuiLayer : public Layer
	{
	private:
		float m_time = 0.0f;
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void Attach() override;
		void Detach() override;

		void ImGuiRender() override;

		void Begin();
		void End();
	};

} }