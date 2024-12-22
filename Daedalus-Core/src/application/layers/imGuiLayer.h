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
	private:
		bool OnMouseButtonPressedEvent(event::MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(event::MouseButtonReleasedEvent& e);
		bool OnMouseMoveEvent(event::MouseMovedEvent& e);
		bool OnMouseMouseScrolledEvent(event::MouseScrolledEvent& e);
		bool OnKeyPressedEvent(event::KeyPressedEvent& e);
		bool OnKeyReleasedEvent(event::KeyReleasedEvent& e);
		bool OnKeyTypedEvent(event::KeyTypedEvent& e);
		bool OnWindowResizeEvent(event::WindowResizedEvent& e);
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void Attach() override;
		void Detach() override;

		void Update() override;
		void OnEvent(event::Event& event) override;
	};

} }