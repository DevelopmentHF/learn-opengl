#include <glad/glad.h>  // GLAD helper library for OpenGL function pointer help
#include <GLFW/glfw3.h> // GLFW helper library for window management
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

/* Default screen size values */
const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;

int main() {

    /* configure window and set glfw/opengl settings */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS specific apparently?

    /* create window object */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn-OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /* init GLAD before using any opengl functions */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {  // OS specific location of function pointers
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }

    /* init viewport prior to rendering -> makes it so screen coords work */
    glViewport(0, 0, 800, 600);     // separate to GLFW window size so make them equal!

    /* callback to allow for user window resizing */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* window loop */
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // greenish
        glClear(GL_COLOR_BUFFER_BIT);   // clear colour buffer

        // check and call events + swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}