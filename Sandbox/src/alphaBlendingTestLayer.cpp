#include "alphaBlendingTestLayer.h"

#include "imgui.h"

AlphaBlendingTestLayer::AlphaBlendingTestLayer()
	: m_camController(1280.0f / 720.0f)
{
}

void AlphaBlendingTestLayer::attach()
{
	DD_PROFILE_FUNCTION();

	m_texture = daedalus::graphics::Texture2D::create("resources/alphaTesting_1.png");
	m_texture2 = daedalus::graphics::Texture2D::create("resources/alphaTesting_2.png");

	ro1 = { {-1.0f,0.0f, -0.1f},{1.0f}, m_texture };
	ro2 = { {1.0f,0.0f, -0.1f},{1.0f}, m_texture2 };
	ro3 = { {0.0f, -0.2f,0.0f}, {2.5f, 1.0f}, {1.0f,0.6f,0.6f,0.75f} };
}

void AlphaBlendingTestLayer::detach()
{
	DD_PROFILE_FUNCTION();
}

void AlphaBlendingTestLayer::update(const daedalus::application::DeltaTime& dt)
{
	DD_PROFILE_FUNCTION();

	m_camController.update(dt);

#ifndef DD_DISTRO
	daedalus::graphics::Renderer2D::resetStats();
#endif
	{
		DD_PROFILE_SCOPE("renderer prep");
		daedalus::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		daedalus::graphics::RenderCommands::clear();
	}

	{
		DD_PROFILE_SCOPE("renderer draw");

		static float rotation = 0.0f;
		rotation += dt * 50.0f;

		daedalus::graphics::Renderer2D::begin(m_camController.getCamera());

		// in all variations the alpha wall is always closer to the camera (should be renderer on top)

		daedalus::graphics::Renderer2D::drawQuad(ro1);
		daedalus::graphics::Renderer2D::drawQuad(ro2);
		daedalus::graphics::Renderer2D::drawQuad(ro3);

		daedalus::graphics::Renderer2D::drawQuad({ {0.0f, -0.4f,0.1f}, {2.5f, 1.0f}, {0.0f,0.0f,0.0f,0.5f} });


		daedalus::graphics::Renderer2D::end();
	}
}

void AlphaBlendingTestLayer::imGuiRender()
{
	DD_PROFILE_FUNCTION();

	ImGui::Begin("Order settings");

	ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode("Render order selection"))
	{
		static int selected = 0;

		if (ImGui::Selectable("quad 1, quad 2, alpha", selected == 0))
		{
			ro1 = { {-1.0f,0.0f, -0.1f},{1.0f}, m_texture };
			ro2 = { {1.0f,0.0f, -0.1f},{1.0f}, m_texture2 };
			ro3 = { {0.0f, -0.2f,0.0f}, {2.5f, 1.0f}, {1.0f,0.6f,0.6f,0.75f} };

			selected = 0;
		}

		if (ImGui::Selectable("quad 1, alpha, quad 2", selected == 1))
		{
			ro1 = { {-1.0f,0.0f, -0.1f},{1.0f}, m_texture };
			ro2 = { {0.0f, -0.2f,0.0f}, {2.5f, 1.0f}, {1.0f,0.6f,0.6f,0.75f} };
			ro3 = { {1.0f,0.0f, -0.1f},{1.0f}, m_texture2 };

			selected = 1;
		}

		if (ImGui::Selectable("alpha, quad 1, quad 2", selected == 2))
		{
			ro1 = { {0.0f, -0.2f,0.0f}, {2.5f, 1.0f}, {1.0f,0.6f,0.6f,0.75f} };
			ro2 = { {-1.0f,0.0f, -0.1f},{1.0f}, m_texture };
			ro3 = { {1.0f,0.0f, -0.1f},{1.0f}, m_texture2 };

			selected = 2;
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

void AlphaBlendingTestLayer::onEvent(daedalus::event::Event& e)
{
	m_camController.onEvent(e);
}