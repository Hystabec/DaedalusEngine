#include "2DTestLayer.h"

#include "imgui.h"

Layer2D::Layer2D()
	: m_camController(1280.0f / 720.0f)
{
}

void Layer2D::attach()
{
	DD_PROFILE_FUNCTION();
#if 0
	m_texture = daedalusCore::graphics::Texture2D::create("resources/DD_testImage.png");
	m_texture2 = daedalusCore::graphics::Texture2D::create("resources/testImage.png");
#else
	m_texture = daedalusCore::graphics::Texture2D::create("resources/alphaTesting_1.png");
	m_texture2 = daedalusCore::graphics::Texture2D::create("resources/alphaTesting_2.png");

	ro1 = { {-1.0f,0.0f, -0.1f},{1.0f}, m_texture };
	ro2 = { {1.0f,0.0f, -0.1f},{1.0f}, m_texture2 };
	ro3 = { {0.0f, -0.2f,0.0f}, {2.5f, 1.0f}, {1.0f,0.6f,0.6f,0.75f} };
#endif
}

void Layer2D::detach()
{
	DD_PROFILE_FUNCTION();
}

void Layer2D::update(const daedalusCore::application::DeltaTime& dt)
{
	DD_PROFILE_FUNCTION();

	m_camController.update(dt);

#ifndef DD_DISTRO
	daedalusCore::graphics::Renderer2D::resetStats();
#endif
	{
		DD_PROFILE_SCOPE("renderer prep");
		daedalusCore::graphics::RenderCommands::setClearColour({ 0.5f, 0.5f, 0.5f, 1.0f });
		daedalusCore::graphics::RenderCommands::clear();
	}

	{
		DD_PROFILE_SCOPE("renderer draw");
		
		static float rotation = 0.0f;
		rotation += dt * 50.0f;

		daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());

#if 0
		daedalusCore::graphics::Renderer2D::drawRotatedQuad(m_squareProps);
		daedalusCore::graphics::Renderer2D::drawQuad({ { -2, 0 }, { 0.75f, 0.75f }, { 0.3f, 0.8f, 0.2f, 1.0f } });
		daedalusCore::graphics::Renderer2D::drawQuad({ { 2, 0 }, { 0.5f, 0.5f }, m_texture });
		daedalusCore::graphics::Renderer2D::drawQuad({ { 2, 2 }, { 1.5f, 1.0f }, m_texture2 });
		daedalusCore::graphics::Renderer2D::drawRotatedQuad({{0, 1},{0.25f, 0.25f}, rotation, m_texture2, {1.0f, 0.6f, 0.6f, 1.0f}});

		for (float y = -5.0f; y < 5.0f; y += 0.05f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.05f)
			{
				daedalusCore::maths::vec4 colour = { (x + 5.0f) / 10.0f , 0.2f, (y + 5.0f) / 10.0f, 0.5f };
				daedalusCore::graphics::Renderer2D::drawQuad({ {x, y}, { 0.2f }, colour });
			}
		}
#else
		// in all variations the alpha wall is always closer to the camera (should be renderer on top)

		daedalusCore::graphics::Renderer2D::drawQuad(ro1);
		daedalusCore::graphics::Renderer2D::drawQuad(ro2);
		daedalusCore::graphics::Renderer2D::drawQuad(ro3);

#endif

		daedalusCore::graphics::Renderer2D::end();
	}
}

void Layer2D::imGuiRender()
{
	DD_PROFILE_FUNCTION();

#if 0
	ImGui::Begin("Settings");
#ifndef DD_DISTRO
	auto stats = daedalusCore::graphics::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::NewLine();
#endif

	ImGui::ColorEdit4("Colour", &(m_squareProps.colour.x));
	ImGui::InputFloat3("position", &(m_squareProps.position.x));
	ImGui::InputFloat2("scale", &(m_squareProps.size.x));

	float asRads = daedalusCore::maths::degrees_to_radians(m_squareProps.rotation);
	if (ImGui::SliderAngle("Z Rotation", &asRads))
		m_squareProps.rotation = daedalusCore::maths::radians_to_degrees(asRads);

	ImGui::End();
#else
	ImGui::Begin("Order settings");
	
	ImGui::SetNextItemOpen(ImGuiTreeNodeFlags_DefaultOpen);
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
#endif
}

void Layer2D::onEvent(daedalusCore::event::Event& e)
{
	m_camController.onEvent(e);
}