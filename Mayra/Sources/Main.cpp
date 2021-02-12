//// Local Headers
//#include <Mayra.hpp>
//
//int main()
//{
//    Mayra::WindowProps* props = new Mayra::WindowProps("Mayra::Engine");
//
//    Mayra::Application* app = new Mayra::Application(props);
//    if (app->Initialize() == EXIT_SUCCESS) {
//        app->Run();
//        return EXIT_SUCCESS;
//    } else {
//        return EXIT_FAILURE;
//    }
//}

#include <Mayra.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.hpp>
#include <Camera.hpp>
#include <Texture2D.hpp>

#include <iostream>

#include "3Dshapes.hpp"

struct Material
{
    unsigned int diffuse;
    unsigned int specular;
    unsigned int emission;
    float shininess;
    float transparency;
};

struct Light
{
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

void framebuffer_size_callback2(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const * path);

void CalculateNormals(float *normal, float *p1, float *p2, float *p3);
void CalculateNormalsFromTriangles(float vertices[], int size, int stride);

// Settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float maxShine = 128.0f;

Light light = {
    glm::vec3(1.0f),
    glm::vec3(1.2f, 1.0f, 2.0f),
    glm::vec3(0.2f), // 0.2f
    glm::vec3(0.5f), // 0.5f
    glm::vec3(1.0f)
};

DirectionalLight sun = {
    glm::vec3(-0.2f, -1.0f, -0.3f),
    glm::vec3(0.2f), // 0.2f
    glm::vec3(0.5f), // 0.5f
    glm::vec3(1.0f)
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lighting!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR::GLFW::FAILED_TO_CREATE_WINDOW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback2);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR::GLAD::FAILED_TO_INITIALIZE" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Mayra::Shader lightingShader(SHADERS "LightingMaps.vert", SHADERS "LightingMaps.frag");
    Mayra::Shader lightCubeShader(SHADERS "LightCube.vert", SHADERS "LightCube.frag");
    Mayra::Texture2D* texture = Mayra::Texture2D::LoadFromFile(TEXTURES "awesomeface.png");
    unsigned int diffuseMap = loadTexture(TEXTURES "container2.png");
    unsigned int specularMap = loadTexture(TEXTURES "container2_specular.png");
    unsigned int emissionMap = loadTexture(TEXTURES "matrix.jpg");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,

        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,

        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,

        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVBO, lightCubeVAO;
    glGenBuffers(1, &lightVBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Mayra::Shapes::cube.vertices), Mayra::Shapes::cube.vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Material material = {
        0,
        1,
        2,
        64.0f,
        1.0f
    };

    lightingShader.Bind();
//    lightingShader.SetInt("material.diffuse", 0);

    // render loop
    // -----------
    float time = 0;

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
//        time += deltaTime;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 model = glm::mat4(1.0f);
        
        for (int i = 0; i < 10; i++)
        {
            // world transformation
            model = glm::mat4(1.0f);

            {
                // be sure to activate shader when setting uniforms/drawing objects
                lightingShader.Bind();
                //            lightingShader.SetVec3("light.color", light.color);
                lightingShader.SetVec3("light.ambient", light.ambient);
                lightingShader.SetVec3("light.diffuse", light.diffuse); // darken diffuse light a bit
                lightingShader.SetVec3("light.specular", light.specular);
                lightingShader.SetVec3("light.position", light.position);

                lightingShader.SetVec3("sun.ambient", sun.ambient);
                lightingShader.SetVec3("sun.diffuse", sun.diffuse); // darken diffuse light a bit
                lightingShader.SetVec3("sun.specular", sun.specular);
                lightingShader.SetVec3("sun.direction", sun.direction);

                lightingShader.SetVec3("viewPos", camera.Position);

                lightingShader.SetFloat("material.shininess", material.shininess);
                lightingShader.SetInt("material.diffuse", material.diffuse);
                lightingShader.SetInt("material.specular", material.specular);
                lightingShader.SetInt("material.emission", material.emission);

                lightingShader.SetFloat("time", time);// + i + j + k);

                lightingShader.SetMat4("projection", projection);
                lightingShader.SetMat4("view", view);

                // world transformation
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(cubePositions[i]));

                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                lightingShader.SetMat4("model", model);

                //            texture->Bind();
                // bind diffuse map
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuseMap);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, specularMap);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, emissionMap);

                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, Mayra::Shapes::cube.verticesCount);
            }
        }

        {
            // also draw the lamp object
            lightCubeShader.Bind();
            lightCubeShader.SetVec3("color", light.color);
            lightCubeShader.SetMat4("projection", projection);
            lightCubeShader.SetMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, light.position);
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller shape
            lightCubeShader.SetMat4("model", model);

            glBindVertexArray(lightCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, Mayra::Shapes::cube.verticesCount);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);

    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &lightVBO);

    delete texture;
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        std::cout << "x: " << camera.Position.x << ", ";
        std::cout << "y: " << camera.Position.y << ", ";
        std::cout << "z: " << camera.Position.z << std::endl;

        light.position = camera.Position;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        glPointSize(10.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        glEnable(GL_CULL_FACE);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        glDisable(GL_CULL_FACE);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback2(GLFWwindow*, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow*, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow*, double, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void CalculateNormals(float *normal, float *p1, float *p2, float *p3)
{
    // based on pseudo-code from https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
    float u[] = {
        p2[0] - p1[0],
        p2[1] - p1[1],
        p2[2] - p1[2]
    };
    float v[] = {
        p3[0] - p1[0],
        p3[1] - p1[1],
        p3[2] - p1[2]
    };

    normal[0] = u[1] * v[2] - u[2] * v[1];
    normal[1] = u[2] * v[0] - u[0] * v[2];
    normal[2] = u[0] * v[1] - u[1] * v[0];
}

void CalculateNormalsFromTriangles(float vertices[], int size, int stride)
{
    float p1[3] = { 0.0f, 0.0f, 0.0f };
    float p2[3] = { 0.0f, 0.0f, 0.0f };
    float p3[3] = { 0.0f, 0.0f, 0.0f };

    float normal[3] = { 0.0f, 0.0f, 0.0f};

    for (int i = 0; i < size; i += stride * 3)
    {
        if (i % 6 == 0)
            std::cout << std::endl;

        p1[0] = vertices[i + 0];
        p1[1] = vertices[i + 1];
        p1[2] = vertices[i + 2];

        p2[0] = vertices[i + 6];
        p2[1] = vertices[i + 7];
        p2[2] = vertices[i + 8];

        p3[0] = vertices[i + 12];
        p3[1] = vertices[i + 13];
        p3[2] = vertices[i + 14];

        CalculateNormals(&normal[0], p1, p2, p3);

        std::cout << "{ " << normal[0] << ", ";
        std::cout << normal[1] << ", ";
        std::cout << normal[2] << " }";
    }
}
