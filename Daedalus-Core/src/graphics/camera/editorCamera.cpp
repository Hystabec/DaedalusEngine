#include "ddpch.h"
#include "editorCamera.h"

#include "maths/mathsHelpers.h"
#include "application/input/Input.h"

// This EditorCamera is copied from https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Renderer/EditorCamera.cpp
// There are "magic" numbers that I should tweak to make the camera feel how I want it too

namespace daedalus::graphics {

	EditorCamera::EditorCamera(float FOV, float aspectRatio, float nearClip, float farClip)
		: m_FOV(FOV), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip), Camera(maths::Mat4::perspective(maths::degrees_to_radians(FOV), aspectRatio, nearClip, farClip))
	{
		updateView();
	}

	void EditorCamera::update(const application::DeltaTime& dt)
	{
		using namespace application;
		if (Input::getKeyDown(InputCode::Key_Left_Control))
		{
			const maths::Vec2& mouse{ Input::getMousePosition() };
			maths::Vec2 delta = (mouse - m_initialMousePosition) * 0.003f;
			m_initialMousePosition = mouse;

			bool middlePress = Input::getMouseButton(InputCode::Mouse_Button_Middle);

			bool somethingHappened = false;
			if (middlePress && Input::getKeyDown(InputCode::Key_Left_Shift))
			{
				mousePanZ(delta);
				somethingHappened = true;
			}
			else if (middlePress)
			{
				mousePan(delta);
				somethingHappened = true;
			}
			else if (Input::getMouseButton(InputCode::Mouse_Button_Left))
			{
				mouseRotate(delta);
				somethingHappened = true;
			}
			else if (Input::getMouseButton(InputCode::Mouse_Button_Right))
			{
				mouseZoom(delta.y);
				somethingHappened = true;
			}

			// This is here so the the matrix maths is only done when
			// a value has changed
			if(somethingHappened)
				updateView();
		}
	}

	void EditorCamera::onEvent(event::Event& e)
	{
		event::EventDispatcher dispatcher(e);
		dispatcher.dispatch<event::MouseScrolledEvent>(DD_BIND_EVENT_FUN(EditorCamera::onMouseScroll));
	}

	maths::Vec3 EditorCamera::getUpDirection() const
	{
		return maths::experimental::rotate_vec3_by_quaternion(getOrientation(), maths::Vec3(0.0f, 1.0f, 0.0f));
	}

	maths::Vec3 EditorCamera::getRightDirection() const
	{
		return maths::experimental::rotate_vec3_by_quaternion(getOrientation(), maths::Vec3(1.0f, 0.0f, 0.0f));
	}

	maths::Vec3 EditorCamera::getFowardDirection() const
	{
		return maths::experimental::rotate_vec3_by_quaternion(getOrientation(), maths::Vec3(0.0f, 0.0f, -1.0f));
	}

	maths::experimental::Quaternion EditorCamera::getOrientation() const
	{
		return maths::experimental::Quaternion(maths::Vec3(-m_pitch, -m_yaw, 0.0f));
	}

	void EditorCamera::updateProjection()
	{
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projection = maths::Mat4::perspective(maths::degrees_to_radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
	}

	void EditorCamera::updateView()
	{
		//m_yaw = m_pitch = 0.0f; // Lock the camera's rotation
		m_position = calculatePosition();

		maths::experimental::Quaternion orientation = getOrientation();
		m_viewMatrix = maths::Mat4::translate(m_position) * maths::experimental::quaterion_to_mat4(orientation);
		m_viewMatrix = maths::Mat4::invert(m_viewMatrix);
	}

	bool EditorCamera::onMouseScroll(event::MouseScrolledEvent& e)
	{
		float delta = e.getY() * 0.1f;
		mouseZoom(delta);
		updateView();
		return false;
	}

	void EditorCamera::mousePan(const maths::Vec2& delta)
	{
		auto [xSpeed, ySpeed] = panSpeed();
		m_focalPoint += -getRightDirection() * delta.x * xSpeed * m_distance;
		m_focalPoint += getUpDirection() * delta.y * ySpeed * m_distance;
	}

	void EditorCamera::mousePanZ(const maths::Vec2& delta)
	{
		auto [xSpeed, zSpeed] = panSpeed();
		m_focalPoint += -getRightDirection() * delta.x * xSpeed * m_distance;
		m_focalPoint += -getFowardDirection() * delta.y * zSpeed * m_distance * 1.5f;
	}

	void EditorCamera::mouseRotate(const maths::Vec2& delta)
	{
		float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
		m_yaw += yawSign * delta.x * rotationSpeed();
		m_pitch += delta.y * rotationSpeed();
	}

	void EditorCamera::mouseZoom(float delta)
	{
		m_distance -= delta * zoomSpeed();
		if (m_distance < 1.0f)
		{
			m_focalPoint += getFowardDirection();
			m_distance = 1.0f;
		}
	}

	maths::Vec3 EditorCamera::calculatePosition() const
	{
		return m_focalPoint - getFowardDirection() * m_distance;
	}

	std::pair<float, float> EditorCamera::panSpeed() const
	{
#ifdef DD_PLATFORM_WINDOWS	// This is kind of anoying but cant use std::min as min is a macro
		float x = min(m_viewportWidth / 1000.0f, 2.4f);
#else
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f);
#endif
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

#ifdef DD_PLATFORM_WINDOWS	// This is kind of anoying but cant use std::min as min is a macro
		float y = min(m_viewportHeight / 1000.0f, 2.4f);
#else
		float y = std::min(m_viewportHeight / 1000.0f, 2.4f);
#endif
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::rotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::zoomSpeed() const
	{
		float distance = m_distance * 0.2f;
#ifdef DD_PLATFORM_WINDOWS	// This is kind of anoying but cant use std::max as max is a macro
		distance = max(distance, 0.0f);
#else
		distance = std::max(distance, 0.0f);
#endif
		float speed = distance * distance;
#ifdef DD_PLATFORM_WINDOWS	// This is kind of anoying but cant use std::min as min is a macro, thanks windows...
		speed = min(speed, 100.0f);
#else
		speed = std::min(speed, 100.0f);
#endif
		return speed;
	}

}