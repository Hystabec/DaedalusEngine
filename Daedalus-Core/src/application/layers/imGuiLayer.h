#pragma once

#include "layer.h"

namespace daedalusCore { namespace application {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void Attach() override;
		void Detach() override;

		void Update() override;
		void OnEvent(event::Event& event) override;
	};

} }