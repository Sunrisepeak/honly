#include <GLFW/glfw3.h>

#include "honly_opengl_recorder.hpp"

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(400, 400, "XRecorder - OpenGLRecorder", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    honly::OpenGLRecorder<1920, 1080> xr;

    unsigned char i = 0;
    
    while (!glfwWindowShouldClose(window)) {
        glClearColor(i / 255.f, i / 255.f, i / 255.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        i++;
        
        xr.captureFrameData();
        xr.saveToVideo();
        //xr.saveToImg();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}