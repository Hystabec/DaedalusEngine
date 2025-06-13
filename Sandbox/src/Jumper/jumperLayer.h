#pragma once
#include <Daedalus.h>

#include "jumperMan.h"
#include "platform.h"

namespace jumper {

	class JumperLayer : public daedalusCore::application::Layer
	{
	public:
		JumperLayer();

		void attach() override;
		void detach() override;
		void update(const daedalusCore::application::DeltaTime& dt) override;
		void imGuiRender() override;
		void onEvent(daedalusCore::event::Event& e) override;

	private:
		daedalusCore::graphics::OrthographicCameraController m_camController;

		JumperMan m_jumperCharacter;
		Platform m_testPlatform;
	};

}