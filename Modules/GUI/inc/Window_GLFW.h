#pragma once

#include "IWindow.h"
#include "KeyModifiers.hpp"
#include "Keys.h"

#include <atomic>
#include <map>

typedef struct GLFWwindow GLFWwindow;
namespace nate::Modules::GUI {
    class Window_GLFW : public IWindow {

        static std::atomic<size_t> WindowCount;
        static std::map<GLFWwindow*, std::function<void(int key, int scancode, int action, int mods)>>
                                                                                            KeyPressCallbacks;
        static std::map<GLFWwindow*, std::function<void()>>                                 OnCloseCallbacks;
        static std::map<GLFWwindow*, std::function<void(int width, int height)>>            OnResizeCallbacks;
        static std::map<GLFWwindow*, std::function<void(int button, int action, int mods)>> OnMouseClickCallbacks;

        const WindowSize  m_InitialSize;
        const std::string m_Name;

        std::map<Key, KeyPressFunc> m_OnKeyPressFuncs;
        CloseFunc                   m_OnCloseFunc;
        ResizeFunc                  m_OnResize;
        MouseClickFunc              m_OnMouseClick;

        GLFWwindow* m_pWindow{nullptr};

      public:
        Window_GLFW(const WindowSize& size, std::string name);
        ~Window_GLFW();

        Window_GLFW(Window_GLFW&& other) = delete;
        Window_GLFW& operator=(Window_GLFW&& other) = delete;

        Window_GLFW(const Window_GLFW& other) = delete;
        Window_GLFW& operator=(const Window_GLFW& other) = delete;

        void AttachKeyCallback(Key key, KeyPressFunc callback) override;
        void ClearKeyCallback(Key key) override;

        void AttachOnCloseCallback(CloseFunc callback) override;
        void ClearOnCloseCallback() override;

        void AttachOnResizeCallback(ResizeFunc callback) override;
        void ClearOnResizeCallback() override;

        void AttachMouseCallback(MouseClickFunc callback) override;
        void ClearMouseCallback() override;

        void PollEvents() const override;
        void Close() const override;
        bool ShouldClose() const override;

        bool               IsValid() const override;
        std::string        LastError() const override;
        const std::string& Name() const override;
        void*              NativeHandle() const override;
        WindowSize         InitialSize() const override;

        CursorPosition QueryCursorPosition() const override;
        KeyState       QueryKeyState(Key key) const override;
        WindowSize     QueryWindowSize() const override;

      private:
        static void KeyPressCallBack(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
        static void OnCloseCallback(GLFWwindow* pWindow);
        static void OnMouseClickCallback(GLFWwindow* pWindow, int button, int action, int mods);
        static void OnWindowResizeCallback(GLFWwindow* window, int width, int height);

        static int          TranslateKey(Key key);
        static Key          TranslateKey(int key);
        static KeyState     TranslateAction(int action);
        static KeyModifiers TranslateKeyMods(int mods);
        static MouseButton  TranslateMouseButton(int button);
    };
} // namespace nate::Modules::GUI