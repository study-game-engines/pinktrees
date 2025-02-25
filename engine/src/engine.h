#pragma once

#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

#include "scenegraph.h"
#include "shader.h"
#include "light.h"
#include "model.h"
#include "geometrybuffer.h"
#include "postprocess.h"
#include "meshfactory.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "constants.h"

namespace engine {

    class Engine;
    class App;
    
    class Engine
    {
    public:
        static Engine& getInstance();

        Engine(Engine const&) = delete;
        void operator=(Engine const&) = delete;

        const char* windowTitle = "Cherry Blossom";
        int windowWidth = 1200, windowHeight = 700;
        bool fullscreen = false;

        int glMajor = 3, glMinor = 3;
        bool vysnc = true;

        void setup();
        void setApp(App* app);
        void start();
        void run();
        void stop();

        int getKey(int key);
        int getMouseButton(int button);
        Vector2 getCursorPos();
    private:
        GLFWwindow* window = nullptr;
        App* app = nullptr;

        Engine() = default;
        void setupGLFW();
        void setupWindow();
        void setupCallbacks();
        void setupGLEW();
        void setupOpenGL();
        void setupImGui();
    };

    class App
    {
    public:
        GLFWwindow* window = nullptr;
        Engine& engine = Engine::getInstance();

        virtual void start() = 0;
        virtual void update(double elapsedSecs) = 0;

        virtual void windowCloseCallback() { };
        virtual void windowSizeCallback(int newWidth, int newHeight) { };
        virtual void keyCallback(int key, int scanCode, int action, int mods) { };
        virtual void mouseCallback(Vector2 mousePosition) { };
        virtual void mouseButtonCallback(int button, int action, int mods) { };
        virtual void mouseScrollCallback(double xoffset, double yoffset) { };
    };

    void glfwErrorCallback(int error, const char* description);
}
