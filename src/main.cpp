#include <glad/glad.h>  // GLAD helper library for OpenGL function pointer help
#include <GLFW/glfw3.h> // GLFW helper library for window management
#include <iostream>
#include "shaders/Shader.h"
#include "Textures/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"
#include "ext/glm/glm.hpp"
#include "ext/glm/gtc/matrix_transform.hpp"
#include "ext/glm/gtc/type_ptr.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, Shader shader);

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
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE); // fixes macOS issue where window had to be interacted with for objects to center

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

    /* Set up our texture */
    Texture texture("../res/container.jpg", GL_RGB);
    Texture texture2("../res/awesomeface.png", GL_RGBA);

    /* Define x,y,z positions of each vertex on our vertices.
       Normalised device coords -> always between -1 and 1 */
    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    unsigned int indices[] = {
            0, 1, 3,  // First triangle
            1, 2, 3   // Second triangle
    };

    /*  VBO holds the actual vertex data.
        VAO keeps track of how to use the vertex data from a VBO.
        EBO specifies the order in which the vertices should be connected to form shapes. */
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    /* vertices data */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);

    // Bind the EBO and copy index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* copy vertex data into the buffer's memory */
    /* static draw since data is set once and used many times. Use dynamic if the data changes a lot */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* how we should interpret vertex data per vertex attribute */
    // position attr
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   // we set location as 0 in our shader code
    // color attr
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));   // offset of size 3 floats
    glEnableVertexAttribArray(1);   // we set location as 1 in our shader code
    // texture attr
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    /* model matrix -> consists of changing objects vertices to the global world space */
    glm::mat4 model = glm::mat4(1.0f);
    /* rotate around the x-axis so our object is lying on the "floor" */
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    /* view matrix -> we want to move backwards, so translate the scene in the negative z direction (into the screen) */
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    /* proj matrix -> creates a frustrum */
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

    /* Set which samplers are correct */
    shader.use();
    shader.setUniformInt("ourTexture", 0);
    shader.setUniformInt("ourTexture2", 1);
    shader.setUniformMat4fv("model", model);
    shader.setUniformMat4fv("view", view);
    shader.setUniformMat4fv("projection", projection);

    /* window loop */
    while(!glfwWindowShouldClose(window)) {
        // input -------------------------------------------------------------------------------------------------------
        processInput(window, shader);

        // rendering ---------------------------------------------------------------------------------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // greenish
        glClear(GL_COLOR_BUFFER_BIT);   // clear colour buffer

        /* bind texture data */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2.getID());

        shader.use();

        /* draw vertices */
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void processInput(GLFWwindow *window, Shader shader) {
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

    /* ability to change texture mix level via arrow keys */
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        shader.use();
//        std::cout << shader.getUniformFloat("mixValue") << std::endl;
        shader.setUniformFloat("mixValue", shader.getUniformFloat("mixValue")+0.01f);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        shader.setUniformFloat("mixValue", shader.getUniformFloat("mixValue")-0.01f);
    }
}