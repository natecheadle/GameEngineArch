#include "GLFWWindow.h"

#include <GLFW/glfw3.h>

#if __linux__
#define GLFW_EXPOSE_NATIVE_X11
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#elif WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3native.h>

#include <map>

namespace nate::Modules::GUI {

    std::atomic<size_t> GLFWWindow::WindowCount = 0;

    std::map<GLFWwindow*, std::function<void(int key, int scancode, int action, int mods)>>
                                                 GLFWWindow::KeyPressCallbacks;
    std::map<GLFWwindow*, std::function<void()>> GLFWWindow::OnCloseCallbacks;

    GLFWWindow::GLFWWindow(int width, int height, std::string name)
        : m_InitialWidth(width)
        , m_InitialHeight(height)
        , m_Name(std::move(name))
    {
        if (glfwInit() == GLFW_FALSE)
        {
            return;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_pWindow = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
        if (!m_pWindow)
            return;

        ++WindowCount;

        glfwSetKeyCallback(m_pWindow, &GLFWWindow::KeyPressCallBack);
        glfwSetWindowCloseCallback(m_pWindow, &GLFWWindow::OnCloseCallback);

        KeyPressCallbacks.insert({m_pWindow, [this](int key, int scancode, int action, int mods) {
                                      KeyPressCallBack(key, scancode, action, mods);
                                  }});
        OnCloseCallbacks.insert({m_pWindow, [this]() { OnCloseCallback(); }});
    }

    GLFWWindow::~GLFWWindow()
    {
        auto* pWindow = m_pWindow;
        m_pWindow     = nullptr;
        KeyPressCallbacks.erase(m_pWindow);
        OnCloseCallbacks.erase(m_pWindow);
        glfwDestroyWindow(pWindow);

        if (--WindowCount == 0)
        {
            glfwTerminate();
        }
    }

    void GLFWWindow::AttachKeyCallback(Key key, std::function<void(KeyState state, KeyModifiers modifiers)> callback)
    {
        auto it = m_OnKeyPressFuncs.find(key);
        if (it == m_OnKeyPressFuncs.end())
        {
            m_OnKeyPressFuncs.insert({key, std::move(callback)});
        }
        else
        {
            it->second = std::move(callback);
        }
    }
    void GLFWWindow::ClearKeyCallback(Key key) { m_OnKeyPressFuncs.erase(key); }

    void GLFWWindow::AttachOnCloseCallback(std::function<void()> callback) { m_OnCloseFunc = std::move(callback); }

    void GLFWWindow::ClearOnCloseCallback() { m_OnCloseFunc = std::function<void()>(); }

    void GLFWWindow::AttachOnResizeCallback(std::function<void(int width, int height)> callback)
    {
        m_OnResize = std::move(callback);
    }
    void GLFWWindow::ClearOnResizeCallback() { m_OnResize = std::function<void(int width, int height)>(); }

    void GLFWWindow::AttachMouseCallback(std::function<void(MouseButton button, KeyState state)> callback)
    {
        m_OnMouseClick = std::move(callback);
    }

    void GLFWWindow::ClearMouseCallback()
    {
        m_OnMouseClick = std::function<void(MouseButton button, KeyState state)>();
    }

    void GLFWWindow::Close() const { glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE); }
    bool GLFWWindow::ShouldClose() const { return glfwWindowShouldClose(m_pWindow) == GLFW_TRUE; }

    bool        GLFWWindow::IsValid() const { return m_pWindow == nullptr; }
    std::string GLFWWindow::LastError() const
    {
        static const char* lastError;
        glfwGetError(&lastError);
        return std::string(lastError);
    }
    const std::string& GLFWWindow::Name() const { return m_Name; }

    void* GLFWWindow::NativeHandle() const
    {
#if __linux__
        if (!IsValid())
            return nullptr;
        return glfwGetX11Window(m_pWindow);
#elif __APPLE__
        if (!IsValid())
            return nullptr;
        return glfwGetCocoaWindow(m_pWindow);
#elif WIN32
        if (!IsValid())
            return nullptr;
        return glfwGetWin32Window(m_pWindow);
#else
        static_assert(false, "Invalid native environment.");
#endif
    }

    int GLFWWindow::InitialWidth() const { return m_InitialWidth; }

    int GLFWWindow::InitialHeight() const { return m_InitialHeight; }

    CursorPosition GLFWWindow::QueryCursorPosition() const
    {
        double xpos{0.0};
        double ypos{0.0};
        glfwGetCursorPos(m_pWindow, &xpos, &ypos);

        return CursorPosition(xpos, ypos);
    }

    KeyState GLFWWindow::QueryKeyState(Key key) const
    {
        switch (glfwGetKey(m_pWindow, TranslateKey(key)))
        {
        case GLFW_RELEASE: return KeyState::Released;
        case GLFW_PRESS: return KeyState::Pressed;
        case GLFW_REPEAT: return KeyState::Repeat;
        }
        assert(false);
        return KeyState::Released;
    }

    WindowSize GLFWWindow::QueryWindowSize() const
    {
        int width{0};
        int height{0};
        glfwGetWindowSize(m_pWindow, &width, &height);
        return WindowSize(width, height);
    }

    void GLFWWindow::KeyPressCallBack(int key, int, int action, int mods)
    {
        if (m_OnKeyPressFuncs.empty())
            return;
        auto it = m_OnKeyPressFuncs.find(TranslateKey(key));
        if (it == m_OnKeyPressFuncs.end())
            return;

        it->second(TranslateAction(action), TranslateKeyMods(mods));
    }

    void GLFWWindow::OnCloseCallback() { m_OnCloseFunc(); }

    void GLFWWindow::KeyPressCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
    {
        auto it = KeyPressCallbacks.find(pWindow);
        if (it != KeyPressCallbacks.end())
        {
            it->second(key, scancode, action, mods);
        }
    }
    void GLFWWindow::OnCloseCallback(GLFWwindow* pWindow)
    {
        auto it = OnCloseCallbacks.find(pWindow);
        if (it != OnCloseCallbacks.end())
        {
            it->second();
        }
    }

    int GLFWWindow::TranslateKey(Key key)
    {
        switch (key)
        {
            // Numbers
        case Key::Zero: return GLFW_KEY_0;
        case Key::One: return GLFW_KEY_1;
        case Key::Two: return GLFW_KEY_2;
        case Key::Three: return GLFW_KEY_3;
        case Key::Four: return GLFW_KEY_4;
        case Key::Five: return GLFW_KEY_5;
        case Key::Six: return GLFW_KEY_6;
        case Key::Seven: return GLFW_KEY_7;
        case Key::Eight: return GLFW_KEY_8;
        case Key::Nine:
            return GLFW_KEY_9;

            // Letters
        case Key::A: return GLFW_KEY_A;
        case Key::B: return GLFW_KEY_B;
        case Key::C: return GLFW_KEY_C;
        case Key::D: return GLFW_KEY_D;
        case Key::E: return GLFW_KEY_E;
        case Key::F: return GLFW_KEY_F;
        case Key::G: return GLFW_KEY_G;
        case Key::H: return GLFW_KEY_H;
        case Key::I: return GLFW_KEY_I;
        case Key::J: return GLFW_KEY_J;
        case Key::K: return GLFW_KEY_K;
        case Key::L: return GLFW_KEY_L;
        case Key::M: return GLFW_KEY_M;
        case Key::N: return GLFW_KEY_N;
        case Key::O: return GLFW_KEY_O;
        case Key::P: return GLFW_KEY_P;
        case Key::Q: return GLFW_KEY_Q;
        case Key::R: return GLFW_KEY_R;
        case Key::S: return GLFW_KEY_S;
        case Key::T: return GLFW_KEY_T;
        case Key::U: return GLFW_KEY_U;
        case Key::V: return GLFW_KEY_V;
        case Key::W: return GLFW_KEY_W;
        case Key::X: return GLFW_KEY_X;
        case Key::Y: return GLFW_KEY_Y;
        case Key::Z: return GLFW_KEY_Z;

        // Arrow Keys
        case Key::Up: return GLFW_KEY_UP;
        case Key::Down: return GLFW_KEY_DOWN;
        case Key::Left: return GLFW_KEY_LEFT;
        case Key::Right: return GLFW_KEY_RIGHT;

        // Function Keys
        case Key::Escape: return GLFW_KEY_ESCAPE;
        case Key::Enter: return GLFW_KEY_ENTER;
        case Key::Tab: return GLFW_KEY_TAB;
        case Key::Backspace: return GLFW_KEY_BACKSPACE;
        case Key::Insert: return GLFW_KEY_INSERT;
        case Key::Delete: return GLFW_KEY_DELETE;

        case Key::LAST: assert(false);
        }

        assert(false);

        return 0;
    }

    Key GLFWWindow::TranslateKey(int key)
    {
        switch (key)
        {
            // Numbers
        case GLFW_KEY_0: return Key::Zero;
        case GLFW_KEY_1: return Key::One;
        case GLFW_KEY_2: return Key::Two;
        case GLFW_KEY_3: return Key::Three;
        case GLFW_KEY_4: return Key::Four;
        case GLFW_KEY_5: return Key::Five;
        case GLFW_KEY_6: return Key::Six;
        case GLFW_KEY_7: return Key::Seven;
        case GLFW_KEY_8: return Key::Eight;
        case GLFW_KEY_9:
            return Key::Nine;

            // Letters
        case GLFW_KEY_A: return Key::A;
        case GLFW_KEY_B: return Key::B;
        case GLFW_KEY_C: return Key::C;
        case GLFW_KEY_D: return Key::D;
        case GLFW_KEY_E: return Key::E;
        case GLFW_KEY_F: return Key::F;
        case GLFW_KEY_G: return Key::G;
        case GLFW_KEY_H: return Key::H;
        case GLFW_KEY_I: return Key::I;
        case GLFW_KEY_J: return Key::J;
        case GLFW_KEY_K: return Key::K;
        case GLFW_KEY_L: return Key::L;
        case GLFW_KEY_M: return Key::M;
        case GLFW_KEY_N: return Key::N;
        case GLFW_KEY_O: return Key::O;
        case GLFW_KEY_P: return Key::P;
        case GLFW_KEY_Q: return Key::Q;
        case GLFW_KEY_R: return Key::R;
        case GLFW_KEY_S: return Key::S;
        case GLFW_KEY_T: return Key::T;
        case GLFW_KEY_U: return Key::U;
        case GLFW_KEY_V: return Key::V;
        case GLFW_KEY_W: return Key::W;
        case GLFW_KEY_X: return Key::X;
        case GLFW_KEY_Y: return Key::Y;
        case GLFW_KEY_Z: return Key::Z;

        // Arrow Keys
        case GLFW_KEY_UP: return Key::Up;
        case GLFW_KEY_DOWN: return Key::Down;
        case GLFW_KEY_LEFT: return Key::Left;
        case GLFW_KEY_RIGHT: return Key::Right;

        // Function Keys
        case GLFW_KEY_ESCAPE: return Key::Escape;
        case GLFW_KEY_ENTER: return Key::Enter;
        case GLFW_KEY_TAB: return Key::Tab;
        case GLFW_KEY_BACKSPACE: return Key::Backspace;
        case GLFW_KEY_INSERT: return Key::Insert;
        case GLFW_KEY_DELETE: return Key::Delete;
        }

        assert(false);

        return Key::LAST;
    }

    static KeyState TranslateAction(int action)
    {
        switch (action)
        {
        case GLFW_PRESS: return KeyState::Pressed;
        case GLFW_RELEASE: return KeyState::Released;
        case GLFW_REPEAT: return KeyState::Repeat;
        }
        assert(false);
        return KeyState::Released;
    }
    static KeyModifiers TranslateKeyMods(int mods)
    {
        return KeyModifiers(
            (mods & GLFW_MOD_SHIFT) == 1,
            (mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL,
            (mods & GLFW_MOD_ALT) == GLFW_MOD_ALT,
            (mods & GLFW_MOD_SUPER) == GLFW_MOD_SUPER,
            (mods & GLFW_MOD_CAPS_LOCK) == GLFW_MOD_CAPS_LOCK,
            (mods & GLFW_MOD_NUM_LOCK) == GLFW_MOD_NUM_LOCK);
    }
} // namespace nate::Modules::GUI