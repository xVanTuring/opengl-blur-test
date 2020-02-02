#include "Common.hh"

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
float left = 150;
float top = 200;
float radius = 200;
void processInput(GLFWwindow *window);
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float *quad1Vertices = rectWithSize(SCR_WIDTH, 200);
    float *quad2Vertices = rectWithSize(200, SCR_WIDTH);
    float *blurQuadVertices = rectWithSize(200.0, 200.0);
    float backgroundVertices[] = {
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f};

    unsigned int quad1VAO, quad1VBO;
    glGenVertexArrays(1, &quad1VAO);
    glGenBuffers(1, &quad1VBO);
    glBindVertexArray(quad1VAO);
    glBindBuffer(GL_ARRAY_BUFFER, quad1VBO);
    glBufferData(GL_ARRAY_BUFFER, RECT_SIZE, quad1Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

    unsigned int quad2VAO, quad2VBO;
    glGenVertexArrays(1, &quad2VAO);
    glGenBuffers(1, &quad2VBO);
    glBindVertexArray(quad2VAO);
    glBindBuffer(GL_ARRAY_BUFFER, quad2VBO);
    glBufferData(GL_ARRAY_BUFFER, RECT_SIZE, quad2Vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

    unsigned int quad3VAO, quad3VBO;
    glGenVertexArrays(1, &quad3VAO);
    glGenBuffers(1, &quad3VBO);
    glBindVertexArray(quad3VAO);
    glBindBuffer(GL_ARRAY_BUFFER, quad3VBO);
    glBufferData(GL_ARRAY_BUFFER, RECT_SIZE, blurQuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

    unsigned int backgroundVAO, backgroundVBO;
    glGenVertexArrays(1, &backgroundVAO);
    glGenBuffers(1, &backgroundVBO);
    glBindVertexArray(backgroundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), &backgroundVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    Shader shader("simple_rect.vs", "simple_rect.fs");
    Shader screenShader("screen.vs", "screen.fs");
    Shader blurShader("blur_rect.vs", "blur_rect.fs");
    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1.0);
    shader.use();
    shader.setMat4("projection", projection);

    blurShader.use();
    blurShader.setMat4("projection", projection);
    blurShader.setInt("screenTexture", 0);

    screenShader.use();
    screenShader.setMat4("projection", glm::mat4(1.0));
    screenShader.setMat4("model", glm::mat4(1.0));
    screenShader.setInt("screenTexture", 0);

    GLuint colorFBO;
    GLuint colorBuffer;

    glGenFramebuffers(1, &colorFBO);
    glGenTextures(1, &colorBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, colorFBO);

    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

    GLuint pingPongFBO[2];
    GLuint pingPongColorBuffer[2];
    glGenFramebuffers(2, pingPongFBO);
    glGenTextures(2, pingPongColorBuffer);
    for (GLuint i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[i]);

        glBindTexture(GL_TEXTURE_2D, pingPongColorBuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingPongColorBuffer[i], 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glBindFramebuffer(GL_FRAMEBUFFER, colorFBO);
        glClearColor(229.0 / 255.0, 229.0 / 255.0, 229.0 / 255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        shader.setMat4("model", glm::translate(model, glm::vec3(0.0f, 100.0f, 0.0f)));
        shader.setVec4("uColor", glm::vec4(0.3451, 0.7333, 0.2, 1.0));
        glBindVertexArray(quad1VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader.setVec4("uColor", glm::vec4(0, 178.0 / 255.0, 1, 1.0));
        shader.setMat4("model", glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f)));
        glBindVertexArray(quad2VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[0]);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glClearColor(0.0f, 1.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        screenShader.use();
        glBindVertexArray(backgroundVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[1]);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glClearColor(0.0f, 1.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        screenShader.use();
        glBindVertexArray(backgroundVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[1]);
        glBindTexture(GL_TEXTURE_2D, pingPongColorBuffer[0]);
        blurShader.use();
        blurShader.setMat4("model", glm::translate(model, glm::vec3(left, top, 0)));
        blurShader.setInt("screenTexture", 0);
        blurShader.setBool("horizontal", true);
        blurShader.setFloat("radius", radius);
        glBindVertexArray(quad3VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[0]);
        glBindTexture(GL_TEXTURE_2D, pingPongColorBuffer[1]);
        blurShader.setBool("horizontal", false);
        glBindVertexArray(quad3VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 1.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        screenShader.use();
        glBindVertexArray(backgroundVAO);
        glBindTexture(GL_TEXTURE_2D, pingPongColorBuffer[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &quad1VAO);
    glDeleteBuffers(1, &quad1VBO);

    glDeleteVertexArrays(1, &quad2VAO);
    glDeleteBuffers(1, &quad2VBO);

    glDeleteVertexArrays(1, &quad3VAO);
    glDeleteBuffers(1, &quad3VBO);

    glDeleteVertexArrays(1, &backgroundVAO);
    glDeleteBuffers(1, &backgroundVBO);
    glfwTerminate();
    return 0;
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        top += 3.0f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        top -= 3.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        left -= 3.0f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        left += 3.0f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        radius += 3.0f;
        std::cout << "radius: " << radius << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        radius -= 3.0f;
        std::cout << "radius: " << radius << std::endl;
    }

    if (radius < 5)
    {
        radius = 5;
    }
    if (radius > 200)
    {
        radius = 200;
    }
}
