#include "ddpch.h"
#include "applicationCore.h"

#include "application/window.h"
#include "application/Input.h"
#include "utils/timer.h"

#define RenderExample 1
#if RenderExample
#include <GL/glew.h>
#endif 
#include <glfw3.h>	//temp

namespace daedalusCore {

	Application* Application::m_instance = nullptr;

	bool Application::OnWindowClose(event::WindowClosedEvent& e)
	{
		m_running = false;
		return true;
	}

	Application::Application(std::string title, unsigned int width, unsigned int height, bool vsync)
	{
		DD_CORE_ASSERT(!m_instance, "Duplicate application");
		m_instance = this;

		debug::Log::Init(debug::Log::LogFlags::Log_to_ImGui);
		m_window = std::unique_ptr<application::Window>(application::Window::Create(application::WindowProperties(title, width, height, vsync)));
		m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_ImGuiLayer = new application::ImGuiLayer;
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(event::Event& e)
	{
		event::EventDispatcher dispatch(e);
		dispatch.Dispatch<event::WindowClosedEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled())
				break;
		}
	}

	void Application::Init()
	{
		//Engine start Logic
#if RenderExample
		m_vertexArray.reset(graphics::buffers::VertexArray::Create());
		float vertcies[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.5f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.4f, 0.7f, 0.0f, 1.0f
		};

		std::shared_ptr<graphics::buffers::VertexBuffer> verBuff(graphics::buffers::VertexBuffer::Create(vertcies, sizeof(vertcies)));

		graphics::buffers::BufferLayout layout =
		{
			{ DD_BUFFERS_VEC3, "a_position" },
			{ DD_BUFFERS_VEC4, "a_colour" }
		};

		verBuff->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(verBuff);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<graphics::buffers::IndexBuffer> indexBuff(graphics::buffers::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->SetIndexBuffer(indexBuff);

		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_colour;
	
			out vec3 v_pos;
			out vec4 v_col;

			void main()
			{
				v_pos = a_position;
				v_col = a_colour;
				gl_Position = vec4(a_position, 1.0);
			}
		)";

		std::string fragSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 colour;
			in vec3 v_pos;
			in vec4 v_col;

			void main()
			{
				colour = vec4(v_pos * 0.5 + 0.5, 1.0);
				colour = v_col;
			}
		)";

		m_shader.reset(graphics::Shader::create(vertexSrc.c_str(), fragSrc.c_str(), false));
#endif
	}

	int Application::Run()
	{
		while (m_running)
		{
			//TO DO: Add clear function to rendering context
			glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //temp
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //temp

#if RenderExample
			m_shader->enable();
			m_vertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_vertexArray->GetIndexBuffer()->Count(), GL_UNSIGNED_INT, nullptr);
#endif

			for (application::Layer* layer : m_layerStack)
				layer->Update();

			m_ImGuiLayer->Begin();
			for (application::Layer* layer : m_layerStack)
				layer->ImGuiRender();
			m_ImGuiLayer->End();

			m_window->Update();
		}

		return 0;
	}

	void Application::PushLayer(application::Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->Attach();
	}

	void Application::PushOverlay(application::Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
		layer->Attach();
	}

}