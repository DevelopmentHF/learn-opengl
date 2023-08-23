#include <glad/glad.h>  // GLAD helper library for OpenGL function pointer help
#include <GLFW/glfw3.h> // GLFW helper library for window management
#include <iostream>
#include "shaders/Shader.h"

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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);     // separate to GLFW window size so make them equal!

    /* callback to allow for user window resizing */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Set up our shader */
    Shader shader("shaders/vertexShading.glsl", "shaders/fragmentShading.glsl");
//    Shader shader("/Users/henryfielding/Documents/repos/learn-opengl/src/shaders/vertexShading.glsl",
//           "/Users/henryfielding/Documents/repos/learn-opengl/src/shaders/fragmentShading.glsl");

    /* Define x,y,z positions of each vertex on our triangle.
       Normalised device coords -> always between -1 and 1 */
    float triangle[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };


    /*  VBO holds the actual vertex data.
        VAO keeps track of how to use the vertex data from a VBO.
        EBO specifies the order in which the vertices should be connected to form shapes. */
    unsigned int VBOs[2], VAOs[2];
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);

    /* triangle data */
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBindVertexArray(VAOs[0]);

    /* copy vertex data into the buffer's memory */
    /* static draw since data is set once and used many times. Use dynamic if the data changes a lot */
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    /* how we should interpret vertex data per vertex attribute */
    // position attr
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   // we set location as 0 in our shader code
    // color attr
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));   // offset of size 3 floats
    glEnableVertexAttribArray(1);   // we set location as 1 in our shader code



    /* window loop */
    while(!glfwWindowShouldClose(window)) {
        // input -------------------------------------------------------------------------------------------------------
        processInput(window);


        // rendering ---------------------------------------------------------------------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // greenish
        glClear(GL_COLOR_BUFFER_BIT);   // clear colour buffer

        shader.use();

        /* draw triangle */
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0 ,3);


        // unbind buffers ----------------------------------------------------------------------------------------------
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
    /* ability to close window */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    /* ability to swap between fill and wireframe modes -> hold w key */
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}