#pragma once

#include "layer.h"

#include "events/keyEvent.h"
#include "events/mouseEvent.h"
#include "events/windowEvent.h"

namespace daedalus { namespace application {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void attach() override;
		void detach() override;

		void imGuiRender() override;

		void begin();
		void end();

	private:
		float m_time = 0.0f;
	};

} }