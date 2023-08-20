#include <glad/glad.h>  // GLAD helper library for OpenGL function pointer help
#include <GLFW/glfw3.h> // GLFW helper library for window management
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

/* Default screen size values */
const unsigned int SCR_HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;

/* Basic shader code ported */
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

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

    /* create shader object */
    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    /* attach shader source code to shader object and compile it */
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    /* check for shader compilation errors */
    int vertexSuccess, fragmentSuccess;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);
    if (!vertexSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    if (!fragmentSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /* link our shaders into a shader program */
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    /* check for shader linking failure */
    int linkSuccess;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
    }

    /* no longer need shader objects, delete them */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /* Define x,y,z positions of each vertex on our triangle.
       Normalised device coords -> always between -1 and 1 */
//    float vertices[] = {
//            // first triangle
//            -0.5f, -0.5f, 0.0f, // top right
//            0.5f, -0.5f, 0.0f,  // bot right
//            0.0f, 0.5f, 0.0f,   // top left
//            // second triangle
//            0.5f, -0.5f, 0.0f,  // bot right
//            -0.5f, -0.5f, 0.0f, // bot left
//            -0.5f, 0.5f, 0.0f   // top left
//    };

    /* none overlapping vertices like above */
//    float vertices[] = {
//            0.5f,  0.5f, 0.0f,  // top right
//            0.5f, -0.5f, 0.0f,  // bottom right
//            -0.5f, -0.5f, 0.0f,  // bottom left
//            -0.5f,  0.5f, 0.0f   // top left
//    };

    float vertices[] = {
            // left triangle
            -0.2f, 0.0f, 0.0f,
            -0.8f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            // second triangle
            0.2f, 0.0f, 0.0f,
            0.8f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,
    };

    unsigned int indices[] = {
            0, 1, 3,    // first triangle
            1, 2, 3     // second triangle
    };

    /* generate a vertex objects and bind them */

    /*  VBO holds the actual vertex data.
        VAO keeps track of how to use the vertex data from a VBO.
        EBO specifies the order in which the vertices should be connected to form shapes. */
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);  // only want to generate `1` buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // any calls to GL_ARRAY_BUFFER target configure VBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    /* copy vertex data into the buffer's memory */
    /* static draw since data is set once and used many times. Use dynamic if the data changes a lot */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* how we should interpret vertex data per vertex attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    /* window loop */
    while(!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // greenish
        glClear(GL_COLOR_BUFFER_BIT);   // clear colour buffer
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawArrays(GL_TRIANGLES, 0 ,6);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // unbind buffers
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