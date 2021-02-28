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


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.hpp>
#include <Camera.hpp>
#include <Model.hpp>

#include <iostream>

void framebuffer_size_callback2(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback2);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Mayra::Shader shader(SHADERS "cubemaps.vert", SHADERS "cubemaps.frag");
//    Mayra::Shader skyboxShader(SHADERS "skybox.vert", SHADERS "skybox.frag");
    Mayra::Shader skyboxShader(SHADERS "skybox.vert", SHADERS "EditorBg.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture(TEXTURES "wall.jpg");

    std::vector<std::string> faces
    {
        TEXTURES "skybox/right.jpg",
        TEXTURES "skybox/left.jpg",
        TEXTURES "skybox/top.jpg",
        TEXTURES "skybox/bottom.jpg",
        TEXTURES "skybox/front.jpg",
        TEXTURES "skybox/back.jpg"
    };
//    stbi_set_flip_vertically_on_load(true);
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    shader.Bind();
    shader.SetInt("texture1", 0);

    skyboxShader.Bind();
    skyboxShader.SetInt("skybox", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw scene as normal
        shader.Bind();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.SetMat4("model", model);
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.Bind();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.SetMat4("view", view);
        skyboxShader.SetVec3("cam_pos", camera.Position);
        skyboxShader.SetVec2("screen_size", glm::vec2((float)SCR_WIDTH, (float)SCR_HEIGHT));
        skyboxShader.SetMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVAO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback2(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
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

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}



//
//#include <Mayra.hpp>
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <Shader.hpp>
//#include <Camera.hpp>
//#include <Texture2D.hpp>
//
//#include <iostream>
//
//#include "3Dshapes.hpp"
//#include "Light.hpp"
//#include "Material.hpp"
//
//#include "Mesh.hpp"
//#include "Model.hpp"
//
//#include <ft2build.h>
//#include FT_FREETYPE_H
//
//#include "Input.hpp"
//
//void character_callback(GLFWwindow* window, unsigned int codepoint);
//void RenderText(Mayra::Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);
//
//struct Character {
//    unsigned int TextureID; // ID handle of the glyph texture
//    glm::ivec2   Size;      // Size of glyph
//    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
//    unsigned int Advance;   // Horizontal offset to advance to next glyph
//};
//
//std::map<GLchar, Character> Characters;
//unsigned int VAO, VBO;
//
//std::string outputString = "a";
//
//void framebuffer_size_callback2(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//unsigned int loadTexture(char const * path);
//
//void CalculateNormals(float *normal, float *p1, float *p2, float *p3);
//void CalculateNormalsFromTriangles(float vertices[], int size, int stride);
//
//// Settings
//const unsigned int SCR_WIDTH = 1920;//1280;
//const unsigned int SCR_HEIGHT = 1080;//720;
//
//// Camera
//Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//// Timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//float maxShine = 128.0f;
//
//bool moveSpotLight { false };
//bool onRenderBbox { true };
//
//float scale = 1.0f;// / 10.0f;
//glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
//
//int prepareFontRendering(Mayra::Shader &shader);
//
//bool guiHasFocus { false };
//
//int main()
//{
//    glm::vec4 clearColor{14.0f / 255.0f, 10.0f / 255.0f, 20.0f / 255.0f, 1.0f};
////    glm::vec4 clearColor{150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f, 1.0f};
//
//    glm::vec3 pointLightPositions[] = {
//        glm::vec3(9.39123f, 1.81413f, -0.460245f),
//        glm::vec3(-7.06756f, 11.7134f, -0.502092f),
//        glm::vec3(-11.8016f, 1.78952f, 4.14569f),
//        glm::vec3(-0.61018f, 1.3829f, -4.9079f)
//    };
//
//    glm::vec3 pointLightColors[] = {
//        glm::vec3(0.863f, 0.078f, 0.235f), // coral
//        glm::vec3(0.863f, 0.078f, 0.235f), // navy
//        glm::vec3(0.863f, 0.078f, 0.235f), // navajowhite
//        glm::vec3(0.863f, 0.078f, 0.235f) // pink
//    };
//
//    Mayra::DirectionalLight directionalLight = {
//        glm::vec3(-0.2f, -1.0f, -0.3f),
//        glm::vec3(0.545f, 0.000f, 0.000f), // 0.2f
//        glm::vec3(0.863f, 0.078f, 0.235f), // 0.5f
//        glm::vec3(1.0f)
//    };
//
//    Mayra::PointLight pointLight = {
//        glm::vec3(0.0f),
//        glm::vec3(29.0f / 255.0f, 29.0f / 255.0f, 67.0f / 255.0f), // 0.2f
//        glm::vec3(0.8f), // 0.5f
//        glm::vec3(1.0f),
//        1.0f,
//        0.09f,
//        0.032f
//    };
//
//    Mayra::SpotLight spotLight = {
//        camera.Position,
//        camera.Front,
//        glm::cos(glm::radians(12.5f)),
//        glm::cos(glm::radians(15.5f)),
//
//        glm::vec3(0.0f), // 0.2f
//        glm::vec3(1.0f), // 0.5f
//        glm::vec3(1.0f),
//
//        0,
//
//        1.0f,
//        0.09f,
//        0.032f
//    };
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lighting!", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "ERROR::GLFW::FAILED_TO_CREATE_WINDOW" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback2);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//    glfwSetCharCallback(window, character_callback);
//
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "ERROR::GLAD::FAILED_TO_INITIALIZE" << std::endl;
//        return -1;
//    }
//
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//
//    // build and compile our shader zprogram
//    // ------------------------------------
//    Mayra::Shader lightingShader(SHADERS "LightingMaps.vert", SHADERS "LightingMaps.frag");
//    Mayra::Shader lightCubeShader(SHADERS "LightCube.vert", SHADERS "LightCube.frag");
//    Mayra::Shader fontShader("Shaders/Fonts.vert", "Shaders/Fonts.frag");
//
//    // setup light meshes
//    unsigned int lightVBO, lightCubeVAO;
//    glGenBuffers(1, &lightVBO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Mayra::Shapes::pyramid.vertices), Mayra::Shapes::pyramid.vertices, GL_STATIC_DRAW);
//
//    glGenVertexArrays(1, &lightCubeVAO);
//    glBindVertexArray(lightCubeVAO);
//
//    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
//    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    if (prepareFontRendering(fontShader) == -1)
//    {
//        return -1;
//    }
//
////    lightingShader.Bind();
////    Mayra::Shader phongShader(SHADERS "PhongShading.vert", SHADERS "PhongShading.frag");
//    Mayra::Shader ourShader(SHADERS "ModelLoading.vert", SHADERS "ModelLoading.frag");
////    ourShader.Bind();
//    stbi_set_flip_vertically_on_load(false);
////    Model ourModel(MODELS "Sponza/sponza.obj");
//    Model ourModel(MODELS "axe/axe.obj");
////    Model ourModel(MODELS "Forest_pack_v1/Forest_pack_v1.obj");
////    Model ourModel(MODELS "mossy_cube/mossy_cube.obj");
////    Model ourModel(MODELS "Wolf/Wolf.obj");
////    stbi_set_flip_vertically_on_load(true);
////    Model ourModel(MODELS "backpack/backpack.obj");
//
//    // render loop
//    // -----------
//    float time = 0;
//
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        // --------------------
//        float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
////        time += deltaTime;
//
//        // input
//        // -----
//        processInput(window);
//
//        // render
//        // ------
//        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
////        ourShader.Bind();
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        glm::mat4 view = camera.GetViewMatrix();
////        ourShader.SetMat4("projection", projection);
////        ourShader.SetMat4("view", view);
//
////        glm::mat4 model = glm::mat4(1.0f);
////        model = glm::translate(model, glm::vec3(5.0f));
////        model = glm::scale(model, glm::vec3(0.1f));
////        ourShader.SetMat4("model", model);
////        mossy_cube.Render(ourShader);
////
//        if (moveSpotLight)
//        {
//            spotLight.position = camera.Position;
//            spotLight.direction = camera.Front;
//        }
//        lightingShader.Bind();
//        lightingShader.SetFloat("time", time);
//
//        lightingShader.SetVec3("viewPos", camera.Position);
//
//        lightingShader.SetVec3("directionalLight.direction", directionalLight.direction);
//        lightingShader.SetVec3("directionalLight.ambient", spotLight.ambient);
//        lightingShader.SetVec3("directionalLight.diffuse", spotLight.diffuse);
//        lightingShader.SetVec3("directionalLight.specular", spotLight.specular);
//
//        for (int i = 0; i < 4; i++)
//        {
//            std::string str;
//            str = "pointLights[" + std::to_string(i) + "]";
//
//            lightingShader.SetVec3(str + ".position", pointLightPositions[i]);
//            lightingShader.SetVec3(str + ".ambient", pointLight.ambient);
//            lightingShader.SetVec3(str + ".diffuse", pointLightColors[i]);
//            lightingShader.SetVec3(str + ".specular", pointLight.specular);
//
//            lightingShader.SetFloat(str + ".constant", pointLight.constant);
//            lightingShader.SetFloat(str + ".linear", pointLight.linear);
//            lightingShader.SetFloat(str + ".quadratic", pointLight.quadratic);
//        }
//
//        lightingShader.SetVec3("spotLight.ambient", spotLight.ambient);
//        lightingShader.SetVec3("spotLight.diffuse", spotLight.diffuse);
//        lightingShader.SetVec3("spotLight.specular", spotLight.specular);
//        lightingShader.SetVec3("spotLight.position", spotLight.position);
//        lightingShader.SetVec3("spotLight.direction", spotLight.direction);
//        lightingShader.SetFloat("spotLight.cutOff", spotLight.cutOff);
//        lightingShader.SetFloat("spotLight.outerCutOff", spotLight.outerCutOff);
//        lightingShader.SetFloat("spotLight.constant", spotLight.constant);
//        lightingShader.SetFloat("spotLight.linear", spotLight.linear);
//        lightingShader.SetFloat("spotLight.quadratic", spotLight.quadratic);
//
//        // view/projection transformations
//        /*glm::mat4*/ projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
//        /*glm::mat4*/ view = camera.GetViewMatrix();
//        lightingShader.SetMat4("projection", projection);
//        lightingShader.SetMat4("view", view);
//
//        glm::mat4 model = glm::mat4(1.0f);
//        {
//            model = glm::mat4(1.0f);
//            lightingShader.Bind();
//            lightingShader.SetFloat("material.shininess", 2.0f);
////            lightingShader.SetMat4("model", model);
//            model = glm::rotate(model, glm::radians((float)glfwGetTime()), glm::vec3(0.0, 0.0, 1.0));
//            model = glm::translate(model, position);
//            model = glm::scale(model, glm::vec3(scale));//glm::vec3(0.99f - scale, 0.99f - scale, 0.99f - scale)); // 0.01f
//            lightingShader.SetMat4("model", model);
//
//            ourModel.Render(lightingShader);
//
//            ourModel.object2world = glm::translate(model, position);
//
//            lightCubeShader.Bind();
//            lightCubeShader.SetVec3("color", glm::vec3(1.0f));
//            lightCubeShader.SetMat4("projection", projection);
//            lightCubeShader.SetMat4("view", view);
//
////            model = glm::translate(model, glm::vec3(0.0f));
////            model = glm::scale(model, glm::vec3(scale)); // 0.01f
//            lightCubeShader.SetMat4("model", model);
////            lightCubeShader.ListUniformsAndAttributes();
//            if (onRenderBbox) {
//                ourModel.RenderBBox(lightCubeShader);
//            }
//        }
//
//        for (int i = 0; i < 4; i++)
//        {
//            {
//                // also draw the lamp object
//                lightCubeShader.Bind();
//
//                lightCubeShader.SetVec3("color", pointLightColors[i]);
//                lightCubeShader.SetMat4("projection", projection);
//                lightCubeShader.SetMat4("view", view);
//
//                model = glm::mat4(1.0f);
//                model = glm::translate(model, pointLightPositions[i]);
//                model = glm::scale(model, glm::vec3(0.1f)); // a smaller shape
//                lightCubeShader.SetMat4("model", model);
//
//                glBindVertexArray(lightCubeVAO);
//                glDrawArrays(GL_TRIANGLES, 0, Mayra::Shapes::pyramid.verticesCount);
////                glm::vec3 c = pointLightColors[i] * glm::vec3(255.0f);
////
////                if (((c.r * 255.0f) * 0.299 + (c.g * 255.0f) * 0.587 + (c.b * 255.0f) * 0.114) > 186.0f)
////                {
////                    lightCubeShader.SetVec3("color", glm::vec3(0.0f));
////                } else {
////                    lightCubeShader.SetVec3("color", glm::vec3(1.0f));
////                }
////                model = glm::scale(model, glm::vec3(1.01f));
////                lightCubeShader.SetMat4("model", model);
////                glDrawArrays(GL_LINES, 0, Mayra::Shapes::pyramid.verticesCount);
//            }
//        }
//
//        // render font stuff
//        RenderText(fontShader, outputString, 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
//        RenderText(fontShader, "Hallo!", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//
//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
////    glDeleteVertexArrays(1, &cubeVAO);
//    glDeleteVertexArrays(1, &lightCubeVAO);
//
////    glDeleteBuffers(1, &cubeVBO);
//    glDeleteBuffers(1, &lightVBO);
//
//    //    delete texture;
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//    return 0;
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow *window)
//{
//    for (unsigned int i = 32; i < GLFW_KEY_LAST; i++)
//    {
//        if (glfwGetKey(window, i) == GLFW_PRESS)
//            Mayra::Input::Instance()->HandleKeyDown(i);
//
//        if (glfwGetKey(window, i) == GLFW_RELEASE)
//            Mayra::Input::Instance()->HandleKeyRelease(i);
//
//        if (glfwGetKey(window, i) == GLFW_REPEAT)
//            Mayra::Input::Instance()->HandleKeyRepeat(i);
//    }
//
//    if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::Escape))
//    {
//        if (!guiHasFocus)
//            glfwSetWindowShouldClose(window, true);
//        if (guiHasFocus)
//            guiHasFocus = false;
//    }
//
//    if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::Enter))
//    {
//        guiHasFocus = true;
//    }
//
//    if (!guiHasFocus)
//    {
//        if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::C))
//        {
//            std::cout << "[";
//            std::cout << camera.Position.x << "f, ";
//            std::cout << camera.Position.y << "f, ";
//            std::cout << camera.Position.z << "f";
//            std::cout << "]" << std::endl;
//        }
//
//        if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::T))
//            moveSpotLight = true;
//        if (Mayra::Input::Instance()->IsKeyUp(Mayra::KeyCode::T))
//            moveSpotLight = false;
//
//        if (Mayra::Input::Instance()->IsAnyKey(Mayra::KeyCode::W, Mayra::KeyCode::Up))
//            camera.ProcessKeyboard(FORWARD, deltaTime);
//        if (Mayra::Input::Instance()->IsAnyKey(Mayra::KeyCode::S, Mayra::KeyCode::Down))
//            camera.ProcessKeyboard(BACKWARD, deltaTime);
//        if (Mayra::Input::Instance()->IsAnyKey(Mayra::KeyCode::A, Mayra::KeyCode::Left))
//            camera.ProcessKeyboard(LEFT, deltaTime);
//        if (Mayra::Input::Instance()->IsAnyKey(Mayra::KeyCode::D, Mayra::KeyCode::Right))
//            camera.ProcessKeyboard(RIGHT, deltaTime);
//
//        if (Mayra::Input::Instance()->IsKey(Mayra::KeyCode::Space))
//            camera.ProcessKeyboard(UP, deltaTime);
//        if (Mayra::Input::Instance()->IsKey(Mayra::KeyCode::X))
//            camera.ProcessKeyboard(DOWN, deltaTime);
//
//        if (Mayra::Input::Instance()->AreBothKeys(Mayra::KeyCode::LeftShift, Mayra::KeyCode::Equal))
//            scale += 0.5f * deltaTime;
//        if (Mayra::Input::Instance()->AreBothKeys(Mayra::KeyCode::LeftShift, Mayra::KeyCode::Minus))
//            scale -= 0.5f * deltaTime;
//
//
//        if (Mayra::Input::Instance()->IsKey(Mayra::KeyCode::LeftShift))
//        {
//            if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::Alpha1))
//                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//            if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::Alpha2))
//                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//            if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::Alpha3))
//            {
//                glPointSize(10.0f);
//                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
//            }
//
//            if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::Alpha4))
//                glEnable(GL_CULL_FACE);
//
//            if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::Alpha5))
//                glDisable(GL_CULL_FACE);
//
//            if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::Alpha6))
//                onRenderBbox = true;
//            if (Mayra::Input::Instance()->IsKeyDown(Mayra::KeyCode::Alpha7))
//                onRenderBbox = false;
//        }
//    }
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback2(GLFWwindow*, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
//
//
//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow*, double xpos, double ypos)
//{
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow*, double, double yoffset)
//{
//    camera.ProcessMouseScroll(yoffset);
//}
//
//unsigned int loadTexture(char const * path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//        else return textureID;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}
//
//
//void CalculateNormals(float *normal, float *p1, float *p2, float *p3)
//{
//    // based on pseudo-code from https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
//    float u[] = {
//        p2[0] - p1[0],
//        p2[1] - p1[1],
//        p2[2] - p1[2]
//    };
//    float v[] = {
//        p3[0] - p1[0],
//        p3[1] - p1[1],
//        p3[2] - p1[2]
//    };
//
//    normal[0] = u[1] * v[2] - u[2] * v[1];
//    normal[1] = u[2] * v[0] - u[0] * v[2];
//    normal[2] = u[0] * v[1] - u[1] * v[0];
//}
//
//void CalculateNormalsFromTriangles(float vertices[], int size, int stride)
//{
//    float p1[3] = { 0.0f, 0.0f, 0.0f };
//    float p2[3] = { 0.0f, 0.0f, 0.0f };
//    float p3[3] = { 0.0f, 0.0f, 0.0f };
//
//    float normal[3] = { 0.0f, 0.0f, 0.0f};
//
//    for (int i = 0; i < size; i += stride * 3)
//    {
//        if (i % 6 == 0)
//            std::cout << std::endl;
//
//        p1[0] = vertices[i + 0];
//        p1[1] = vertices[i + 1];
//        p1[2] = vertices[i + 2];
//
//        p2[0] = vertices[i + 6];
//        p2[1] = vertices[i + 7];
//        p2[2] = vertices[i + 8];
//
//        p3[0] = vertices[i + 12];
//        p3[1] = vertices[i + 13];
//        p3[2] = vertices[i + 14];
//
//        CalculateNormals(&normal[0], p1, p2, p3);
//
//        std::cout << "{ " << normal[0] << ", ";
//        std::cout << normal[1] << ", ";
//        std::cout << normal[2] << " }";
//    }
//}
//
//int prepareFontRendering(Mayra::Shader &shader)
//{
//    // OpenGL state
//    // ------------
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    // compile and setup the shader
//    // ----------------------------
//    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
//    shader.Bind();
//    glUniformMatrix4fv(glGetUniformLocation(shader.GetRendererID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//
//    // FreeType
//    // --------
//    FT_Library ft;
//    // All functions return a value different than 0 whenever an error occurred
//    if (FT_Init_FreeType(&ft))
//    {
//        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//        return -1;
//    }
//
//    //    "Antonio-Bold.ttf"
//    // find path to font
//    std::string font_name = FONTS "Antonio-Bold.ttf";
//    if (font_name.empty())
//    {
//        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
//        return -1;
//    }
//
//    // load font as face
//    FT_Face face;
//    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
//        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//        return -1;
//    }
//    else {
//        // set size to load glyphs as
//        FT_Set_Pixel_Sizes(face, 0, 48);
//
//        // disable byte-alignment restriction
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//        // load first 128 characters of ASCII set
//        for (unsigned char c = 0; c < 128; c++)
//        {
//            // Load character glyph
//            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//            {
//                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//                continue;
//            }
//            // generate texture
//            unsigned int texture;
//            glGenTextures(1, &texture);
//            glBindTexture(GL_TEXTURE_2D, texture);
//            glTexImage2D(
//                         GL_TEXTURE_2D,
//                         0,
//                         GL_RED,
//                         face->glyph->bitmap.width,
//                         face->glyph->bitmap.rows,
//                         0,
//                         GL_RED,
//                         GL_UNSIGNED_BYTE,
//                         face->glyph->bitmap.buffer
//                         );
//            // set texture options
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//            // now store character for later use
//            Character character = {
//                texture,
//                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//                static_cast<unsigned int>(face->glyph->advance.x)
//            };
//            Characters.insert(std::pair<char, Character>(c, character));
//        }
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }
//    // destroy FreeType once we're finished
//    FT_Done_Face(face);
//    FT_Done_FreeType(ft);
//
//    // configure VAO/VBO for texture quads
//    // -----------------------------------
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    return 0; // everything went a-ok;
//}
//
//// render line of text
//// -------------------
//void RenderText(Mayra::Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
//{
//    // activate corresponding render state
//    shader.Bind();
//    glUniform3f(glGetUniformLocation(shader.GetRendererID(), "textColor"), color.x, color.y, color.z);
//    glActiveTexture(GL_TEXTURE0);
//    glBindVertexArray(VAO);
//
//    // iterate through all characters
//    std::string::const_iterator c;
//    for (c = text.begin(); c != text.end(); c++)
//    {
//        Character ch = Characters[*c];
//
//        float xpos = x + ch.Bearing.x * scale;
//        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
//
//        float w = ch.Size.x * scale;
//        float h = ch.Size.y * scale;
//        // update VBO for each character
//        float vertices[6][4] = {
//            { xpos,     ypos + h,   0.0f, 0.0f },
//            { xpos,     ypos,       0.0f, 1.0f },
//            { xpos + w, ypos,       1.0f, 1.0f },
//
//            { xpos,     ypos + h,   0.0f, 0.0f },
//            { xpos + w, ypos,       1.0f, 1.0f },
//            { xpos + w, ypos + h,   1.0f, 0.0f }
//        };
//        // render glyph texture over quad
//        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//        // update content of VBO memory
//        glBindBuffer(GL_ARRAY_BUFFER, VBO);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
//
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        // render quad
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
//    }
//    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//void character_callback(GLFWwindow*, unsigned int codepoint)
//{
//    if (!guiHasFocus)
//        return;
//
//    if (outputString.length() > 10) {
//        outputString.erase(0, 1);
//    }
//    outputString += (char)codepoint;
////    std::cout << codepoint << ": " << (char)codepoint << std::endl;
//}
