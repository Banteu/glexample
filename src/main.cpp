#include <iostream>
#include <GLFW/glfw3.h>
#include <renderer.h>

class Window
{
    Renderer *mRenderer;
    GLFWwindow *mWindow{};

    static void resizeEvent(GLFWwindow* wnd, int32_t w, int32_t h)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(wnd))->mRenderer->resize(w, h);
    }

public:
    bool initWindow(Renderer *r)
    {
        mRenderer = r;
        if (!glfwInit())
        {
            LOG_ERROR("Can't initialize GLFW");
            return false;
        }

        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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
        glfwSetWindowSizeCallback(mWindow, resizeEvent);
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

    GLFWwindow *getWindow()
    {
        return mWindow;
    }
};

int main()
{
    printf("Starting");
    Renderer r{};
    Window wind{};

    if (!wind.initWindow(&r))
    {
        LOG_ERROR("Window creation failure...shutting down.");
        return -1;
    }
    if (!r.init())
    {
        LOG_ERROR("Renderer creation failure ... shutting down");
        return -1;
    }
    int32_t w, h;
    glfwGetWindowSize(wind.getWindow(), &w, &h);
    r.resize(w, h);

    while (!glfwWindowShouldClose(wind.getWindow()))
    {
        r.draw();
        wind.swapBuffers();
        glfwPollEvents();
    };
    return 0;
}