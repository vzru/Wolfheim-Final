#include "window.h"

#include "spritelib.h"

namespace spritelib
{
	std::shared_ptr<Window> Window::m_mainWindow = std::shared_ptr<Window>(nullptr);

	Window::Window()
		: m_width(1280), m_height(720)
	{
		m_start = std::chrono::high_resolution_clock::now();

		m_proj = new math::Matrix4x4();
		*m_proj = math::Matrix4x4::ortho(0.0f, (float)m_width, 0.0f, (float)m_height, -100.0f, 100.0f);
		m_window = new sf::RenderWindow();
		m_window->display();
		m_window->clear();
	}

	Window::Window(const Window& obj)
	{

	}


	Window& Window::init(std::string a_game_name, unsigned int a_width, unsigned int a_height, bool a_full_screen)
	{
		m_name = a_game_name;
		m_width = a_width;
		m_height = a_height;
		m_proj = new math::Matrix4x4();
		*m_proj = math::Matrix4x4::ortho(0.0f, (float)m_width, 0.0f, (float)m_height, -100.0f, 100.0f);

		// Open the window
		m_window->create(sf::VideoMode(m_width, m_height), m_name, a_full_screen ? sf::Style::Fullscreen : sf::Style::Default);
		set_ortho_matrix(0, 0, m_width, m_height);
		m_window->setActive();
		glewInit();

		glEnable(GL_BLEND);
		glLineWidth(5.0f);
		glDisable(GL_DEPTH_TEST);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		return *this;
	}
	/*Window& Window::init(std::string a_game_name, math::Vector3 a_size = { 1280, 720 }, bool a_full_screen = false)
	{
		m_name = a_game_name;
		m_width = a_size.x;
		m_height = a_size.y;
		m_proj = new math::Matrix4x4();
		*m_proj = math::Matrix4x4::ortho(0.0f, a_size.x, 0.0f, a_size.y, -100.0f, 100.0f);

		// Open the window
		m_window->create(sf::VideoMode(a_size.x, a_size.y), m_name, a_full_screen ? sf::Style::Fullscreen : sf::Style::Default);
		set_screen_size(a_size.x, a_size.y);
		m_window->setActive();
		glewInit();

		glEnable(GL_BLEND);
		glLineWidth(5.0f);
		glDisable(GL_DEPTH_TEST);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		return *this;
	}*/

	Window & Window::set_ortho_matrix(int a_x, int a_y, unsigned int a_width, unsigned int a_height)
	{
		m_screenX = a_x;
		m_screenY = a_y;
		m_screenWidth = a_width;
		m_screenHeight = a_height;

		m_proj = new math::Matrix4x4();
		*m_proj = math::Matrix4x4::ortho((float)a_x, float(a_x + a_width), (float)a_y, float(a_y + a_height), -100.0f, 100.0f);

		return *this;
	}
	/*Window & Window::set_screen_size(math::Vector3 a_size)
	{
		m_screenSize = { a_size.x, a_size.y };

		m_proj = new math::Matrix4x4();
		*m_proj = math::Matrix4x4::ortho(0.0f, a_size.x, 0.0f, a_size.y, -100.0f, 100.0f);

		return *this;
	}*/

	Window& Window::set_size(int a_width, int a_height)
	{
		m_width = a_width;
		m_height = a_height;

		return *this;
	}
	/*Window& Window::set_size(math::Vector3 a_size)
	{
		m_size = { a_size.y, a_size.x };

		return *this;
	}*/

	Window & Window::set_title(std::string a_game_name)
	{
		m_window->setTitle(a_game_name);

		return *this;
	}

	Window & Window::set_window_style(bool a_resizable, bool a_titlebar)
	{
		auto style = a_resizable ? sf::Style::Resize : 0;
		style = style | (a_titlebar ? (sf::Style::Titlebar | sf::Style::Close) : 0);

		HWND handle = m_window->getSystemHandle();
		DWORD win32Style = WS_VISIBLE;

		if (style == sf::Style::None)
		{
			win32Style |= WS_POPUP;
		}
		else
		{
			if (style & sf::Style::Titlebar) win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
			if (style & sf::Style::Resize)   win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
			if (style & sf::Style::Close)    win32Style |= WS_SYSMENU;
		}

		SetWindowLongPtr(handle, GWL_STYLE, win32Style);

		// Force changes to take effect
		SetWindowPos(handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME);

		return *this;
	}

	Window & Window::set_clear_color(unsigned char a_red, unsigned char a_green, unsigned char a_blue)
	{
		m_clearColor = new sf::Color(a_red, a_green, a_blue);
		return *this;
	}

	sf::RenderWindow & Window::get_sfml_window()
	{
		return *m_window;
	}

	Window & Window::set_keyboard_callback(std::function<void(Key, EventType)> a_keyboardFunction)
	{
		m_keyboardFunction = a_keyboardFunction;
		return *this;
	}

	Window & Window::set_mouse_callback(std::function<void(Button, math::Vector3, EventType)> a_mouseFunction)
	{
		m_mouseFunction = a_mouseFunction;
		return *this;
	}

	float Window::get_delta_time()
	{
		return m_deltaTime;
	}

	int Window::get_width(bool a_getScreenWidth)
	{
		return a_getScreenWidth ? m_screenWidth : m_width;
	}
	int Window::get_height(bool a_getScreenHeight)
	{
		return a_getScreenHeight ? m_screenHeight : m_height;
	}
	math::Vector3 Window::get_position()
	{
		return math::Vector3(m_screenX, m_screenY);
	}
	math::Vector3 Window::get_size(bool a_getScreenSize)
	{
		return a_getScreenSize ?
			math::Vector3(m_screenWidth,	m_screenHeight	):
			math::Vector3(m_width,			m_height		);
	}

	Window & Window::show_mouse(bool a_showMouse)
	{
		m_window->setMouseCursorVisible(a_showMouse);
		return *this;
	}

	void Window::cleanup(Window* cleanup)
	{
		delete cleanup->m_proj;
		delete cleanup->m_window;
		delete cleanup->m_clearColor;

		delete cleanup;
	}

	// This function will return a reference to the main game window
	Window & Window::get_game_window()
	{
		// If the window is equal to nullptr, it means that no window exists
		if (m_mainWindow == nullptr)
		{
			// So, we set define m_mainWindow here, and create a new window instance
			m_mainWindow = std::shared_ptr<Window>(new Window());
		}

		// We return that window. This ensures that the window is only created once.
		return *m_mainWindow;
	}

	bool Window::update(int a_targetFPS)
	{
		do
		{	// FPS limiter. This can essentially be turned off by setting a_targetFPS to like, 9999
			m_stop = std::chrono::high_resolution_clock::now();
			using sec = std::chrono::duration<float>;
			m_deltaTime = std::chrono::duration_cast<sec>(m_stop - m_start).count();
		} while (1.0f / m_deltaTime >= a_targetFPS);
		m_start = m_stop;

		sf::Event event;
		while (m_window->pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				m_window->close();
			}

			if (event.type == sf::Event::EventType::Resized)
			{
				m_width = event.size.width; 
				m_height = event.size.height;

				// If you want to, you can handle aspect ratio here
			}

			if (m_keyboardFunction)
			{
				if (event.type == 5 || event.type == 6) // Keyboard events
				{
					m_keyboardFunction((Key)(event.key.code), (EventType)(event.type));
				}
			}
			if (m_mouseFunction)
			{
				if (event.type == 9 || event.type == 10) // Mouse button events
				{
					m_mouseFunction((Button)(event.mouseButton.button),
						{ float(event.mouseButton.x), float(m_window->getSize().y - event.mouseButton.y) }, (EventType)(event.type));
				}
				if (event.type == 11)
				{
					m_mouseFunction(Button::UnknownButton,
						{ float(event.mouseMove.x), float(m_window->getSize().y - event.mouseMove.y) }, (EventType)(event.type));
				}
			}
		}

		// These are SFML functions. display swaps the front and back buffer, and clear will clear the back buffer.
		m_window->display();
		m_window->clear(*m_clearColor);

		return m_window->isOpen();
	}
}