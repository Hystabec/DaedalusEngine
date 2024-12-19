#pragma once

#include "layer.h"

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

		void Update() override;
		void OnEvent(event::Event& event) override;
	};

} }