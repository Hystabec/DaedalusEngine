#pragma once


namespace daedalusCore { namespace graphics {

	class Window
	{
	private:
		const char* m_title;
		int m_width, m_height;
	public:
		Window(const char* title, int width, int height, int xScreenPosition, int yScreenPosition);
		~Window();
		void update() const;

	};

} }