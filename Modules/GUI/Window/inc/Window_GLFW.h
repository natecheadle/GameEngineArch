#pragma once

#include "IWindow.h"
#include "KeyModifiers.hpp"
#include "KeyPressedInfo.hpp"
#include "Keys.h"
#include "MouseClickedInfo.hpp"
#include "WindowMessagePump.h"
#include "WindowSize.hpp"

#include <DebugMutex.hpp>
#include <MutexProtected.hpp>
#include <StaticMap.hpp>

#include <atomic>
#include <map>
#include <mutex>

typedef struct GLFWwindow GLFWwindow;
namespace nate::Modules::GUI
{
    class Window_GLFW : public IWindow
    {

        static std::atomic<size_t> WindowCount;
        static std::map<GLFWwindow*, std::function<void(int key, int scancode, int action, int mods)>>
                                                                                            KeyPressCallbacks;
        static std::map<GLFWwindow*, std::function<void()>>                                 OnCloseCallbacks;
        static std::map<GLFWwindow*, std::function<void(int width, int height)>>            OnResizeCallbacks;
        static std::map<GLFWwindow*, std::function<void(int button, int action, int mods)>> OnMouseClickCallbacks;

        const WindowSize  m_InitialSize;
        const std::string m_Name;

        GLFWwindow*       m_pWindow{nullptr};
        WindowMessagePump m_MessagePump;
        union {
            WindowSize       WindowSizeInfo;
            KeyPressedInfo   KeyPressInfo;
            MouseClickedInfo MouseClickInfo;
        } m_MessageData{WindowSize(0, 0)};
        DebugMutex m_MessageDataMutex;

        MutexProtected<std::mutex, KeyStateMap> m_KeyStates;

      public:
        Window_GLFW(const WindowSize& size, std::string name);
        ~Window_GLFW();

        Window_GLFW(Window_GLFW&& other)            = delete;
        Window_GLFW& operator=(Window_GLFW&& other) = delete;

        Window_GLFW(const Window_GLFW& other)            = delete;
        Window_GLFW& operator=(const Window_GLFW& other) = delete;

        void SubscribeToMessage(void* subscriber, WindowMessages id, std::function<void(const WindowMessage*)> callback)
            override;
        void Unsubscribe(void* subscriber) override;

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

        std::pair<KeyState, KeyModifiers> GetLastKeyState(Key key) const override
        {
            return m_KeyStates.execute<std::pair<KeyState, KeyModifiers>>(
                [key](const StaticMap<Key, Key::None, Key::LAST, std::pair<KeyState, KeyModifiers>>& val)
                    -> std::pair<KeyState, KeyModifiers> { return val[key]; });
        }

        void ExecuteWithKeyStates(std::function<void(const KeyStateMap&)> exe) const override
        {
            m_KeyStates.execute(exe);
        }

      private:
        void InitializeKeyStates(KeyStateMap& val) const;
        void OnKeyPressed(const GUI::WindowMessage* pMessage);

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