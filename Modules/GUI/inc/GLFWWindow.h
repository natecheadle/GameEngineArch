#pragma once

#include "IWindow.h"

#include <atomic>
#include <map>

typedef struct GLFWwindow GLFWwindow;
namespace nate::Modules::GUI {
    class GLFWWindow : public IWindow {

        static std::atomic<size_t> WindowCount;
        static std::map<GLFWwindow*, std::function<void(int key, int scancode, int action, int mods)>>
                                                            KeyPressCallbacks;
        static std::map<GLFWwindow*, std::function<void()>> OnCloseCallbacks;

        const int         m_InitialWidth;
        const int         m_InitialHeight;
        const std::string m_Name;

        std::map<Key, std::function<void(KeyState state, KeyModifiers modifiers)>> m_OnKeyPressFuncs;
        std::function<void()>                                                      m_OnCloseFunc;
        std::function<void(int width, int height)>                                 m_OnResize;
        std::function<void(MouseButton button, KeyState state)>                    m_OnMouseClick;

        GLFWwindow* m_pWindow{nullptr};

      public:
        GLFWWindow(int width, int height, std::string name);
        ~GLFWWindow();

        GLFWWindow(GLFWWindow&& other) = delete;
        GLFWWindow& operator=(GLFWWindow&& other) = delete;

        GLFWWindow(const GLFWWindow& other) = delete;
        GLFWWindow& operator=(const GLFWWindow& other) = delete;

        void AttachKeyCallback(Key key, std::function<void(KeyState state, KeyModifiers modifiers)> callback) override;
        void ClearKeyCallback(Key key) override;

        void AttachOnCloseCallback(std::function<void()> callback) override;
        void ClearOnCloseCallback() override;

        void AttachOnResizeCallback(std::function<void(int width, int height)> callback) override;
        void ClearOnResizeCallback() override;

        void AttachMouseCallback(std::function<void(MouseButton button, KeyState state)> callback) override;
        void ClearMouseCallback() override;

        void Close() const override;
        bool ShouldClose() const override;

        bool               IsValid() const override;
        std::string        LastError() const override;
        const std::string& Name() const override;
        void*              NativeHandle() const override;
        int                InitialWidth() const override;
        int                InitialHeight() const override;

        CursorPosition QueryCursorPosition() const override;
        KeyState       QueryKeyState(Key key) const override;
        WindowSize     QueryWindowSize() const override;

      private:
        void KeyPressCallBack(int key, int scancode, int action, int mods);
        void OnCloseCallback();

        static void KeyPressCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
        static void OnCloseCallback(GLFWwindow* pWindow);

        static int          TranslateKey(Key key);
        static Key          TranslateKey(int key);
        static KeyState     TranslateAction(int actino);
        static KeyModifiers TranslateKeyMods(int mods);
    };
} // namespace nate::Modules::GUI