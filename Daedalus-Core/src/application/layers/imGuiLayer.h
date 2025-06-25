#pragma once

#include "layer.h"

namespace daedalus { namespace application {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void attach() override;
		void detach() override;

		void imGuiRender() override;

		void onEvent(event::Event& event) override;

		void begin();
		void end();

		inline void setAllowEvents(bool allow) { m_allowEvents = allow; }
	private:
		bool m_allowEvents = true;
		float m_time = 0.0f;
	};

} }