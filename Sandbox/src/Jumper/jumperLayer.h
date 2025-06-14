#pragma once
#include <Daedalus.h>

#include "gameCamera.h"
#include "jumperMan.h"
#include "levelManager.h"

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
		GameCamera m_gameCamera;
		LevelManager m_levelManager;
		JumperMan m_jumperCharacter;
	};

}