#include <iostream>
#include <GLFW/glfw3.h>


class Window
{
    GLFWwindow* mWindow{};

    public:
        bool initWindow()
        {
                if (!glfwInit())
                {
                    LOG_ERROR("Can't initialize GLFW");
                    return false;
                }

                glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                int32_t w, h, x, y;
                glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);
                mWindow = glfwCreateWindow(w, h, "Example", nullptr, nullptr);
                if (!mWindow)
                {
                    LOG_ERROR("Can't create window");
                    glfwTerminate();
                    return false;
                }

                glfwSetWindowUserPointer(mWindow, this);
                glfwSetWindowSizeCallback(mWindow, windowResizeEvent);
                glfwSetCursorPosCallback(mWindow, mousePositionEvent);
                glfwSetMouseButtonCallback(mWindow, mouseActionEvent);
                glfwSetScrollCallback(mWindow, mouseScrollEvent);
                glfwSetKeyCallback(mWindow, keyEvent);
                glfwSetCharCallback(mWindow, textEvent);
                glfwMakeContextCurrent(mWindow);

                return true;
        }

    void swapBuffers()
    {
        glfwSwapBuffers(mWindow);
    };
    bool bind()
    {
        glfwMakeContextCurrent(mWindow);
        return true;
    };

    GLFWwindow* getWindow()
    {
        return mWindow;
    }

}


int main()
{
    std::unique_ptr<Window> wind = std::make_unique<Window>();
    if (!wind->initWindow())
    {
        LOG_ERROR("Window creation failure...shutting down.");
        return -1;
    }

    while (!glfwWindowShouldClose(wind->getWindow()))
    {
        glfwPollEvents();
    };
    return 0;
}