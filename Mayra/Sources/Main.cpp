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

#include <iostream>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float transparency;
};

struct Light {
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

void framebuffer_size_callback2(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

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
    glm::vec3(1.0f), // 0.2f
    glm::vec3(1.0f), // 0.5f
    glm::vec3(1.0f)
};

Material materialDefault = {
    glm::vec3(1.0f, 0.5f, 0.31f),
    glm::vec3(1.0f, 0.5f, 0.31f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    maxShine,
    1.0f
};

// https://people.sc.fsu.edu/~jburkardt/data/mtl/example.mtl
Material flatwhite = {
    glm::vec3(0.5f),
    glm::vec3(1.0f),
    glm::vec3(0.0f),
    0.0f,
    1.0f
};

Material shinyred = {
    glm::vec3(0.1986f, 0.0000f, 0.0000f),
    glm::vec3(0.5922f, 0.0166f, 0.0000f),
    glm::vec3(0.5974f, 0.2084f, 0.2084f),
    100.2237f,
    1.0f
};

Material clearblue = {
    glm::vec3(0.0394f, 0.0394f, 0.3300f),
    glm::vec3(0.1420f, 0.1420f, 0.9500f),
    glm::vec3(0.0f),
    0.0f,
    0.4300f
};
// end example.mtl

Material materialStone = {
    glm::vec3(1.0000f, 1.0000f, 1.0000f),
    glm::vec3(0.3372f, 0.2997f, 0.2954f),
    glm::vec3(0.2591f, 0.2591f, 0.2591f),
    54.2231f,
    1.0f
};

Material materialJade = {
    glm::vec3(0.135f, 0.2225f, 0.1575f),
    glm::vec3(0.54f, 0.89f, 0.63f),
    glm::vec3(0.316228f),
    12.8f * maxShine,
    1.0f
};

Material materialEmerald = {
    glm::vec3(0.0215f, 0.1745f, 0.0215f),
    glm::vec3(0.07568f, 0.61424f, 0.07568f),
    glm::vec3(0.633f, 0.727811f, 0.633f),
    76.8f * maxShine,
    1.0f
};

Material materialRuby = {
    glm::vec3(0.1745f, 0.01175f, 0.01175f),
    glm::vec3(0.61424f, 0.04136f, 0.04136f),
    glm::vec3(0.727811f, 0.626959f, 0.626959f),
    76.8f * maxShine,
    1.0f
};

Material materialYellowRubber = {
    glm::vec3(0.05f, 0.05f, 0.0f),
    glm::vec3(0.5f, 0.5f, 0.4f),
    glm::vec3(0.7f, 0.7f, 0.04f),
    10.0f * maxShine,
    1.0f
};

Material materialYellowPlastic = {
    glm::vec3(0.0f),
    glm::vec3(0.5f, 0.5f, 0.0f),
    glm::vec3(0.6f, 0.6f, 0.5f),
    32.0f * maxShine,
    1.0f
};

Material materialBlackRubber = {
    glm::vec3(0.02f),
    glm::vec3(0.01f),
    glm::vec3(0.4f),
    0.078125f * maxShine,
    1.0f
};

Material materialBlackPlastic = {
    glm::vec3(0.0f),
    glm::vec3(0.01f),
    glm::vec3(0.5f),
    0.25f * maxShine,
    1.0f
};

Material materialPearl = {
    glm::vec3(0.25f, 0.20725f, 0.20725f),
    glm::vec3(1.0f, 0.829f, 0.829f),
    glm::vec3(0.296648f, 0.296648f, 0.296648f),
    11.264f * maxShine,
    1.0f
};

Material materials[] = {
    shinyred,
    flatwhite,
    clearblue,
    materialStone,
    materialYellowRubber,
    materialYellowPlastic,
    materialBlackRubber,
    materialPearl,
    materialBlackPlastic,
    materialRuby,
    materialEmerald,
    materialJade,
    materialDefault
};

glm::vec3 colors[] = {
    glm::vec3(0.275f, 0.510f, 0.706f),
    glm::vec3(0.294f, 0.000f, 0.510f),
    glm::vec3(1.000f, 1.000f, 0.941f),
    glm::vec3(1.000f, 0.271f, 0.000f),
    glm::vec3(0.859f, 0.439f, 0.576f),

    glm::vec3(1.000f, 0.941f, 0.961f),
    glm::vec3(0.486f, 0.988f, 0.000f),
    glm::vec3(0.627f, 0.322f, 0.176f),
    glm::vec3(0.678f, 0.847f, 0.902f),
    glm::vec3(0.941f, 0.502f, 0.502f)
};

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

void CalculateNormalsFromTriangles(float *vertices, int stride)
{
    float p1[3] = { 0.0f, 0.0f, 0.0f };
    float p2[3] = { 0.0f, 0.0f, 0.0f };
    float p3[3] = { 0.0f, 0.0f, 0.0f };

    float normal[3] = { 0.0f, 0.0f, 0.0f};

    int verticesCount = (int)sizeof(vertices) / sizeof(float);
    for (int i = 0; i < verticesCount; i += stride * 3)
    {
        if (i % 6 == 0)
            std::cout << "\n" << std::endl;

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
    Mayra::Shader lightingShader(SHADERS "Colors.vert", SHADERS "Colors.frag");
    Mayra::Shader lightCubeShader(SHADERS "LightCube.vert", SHADERS "LightCube.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices2[] = {
        // X    Y      Z      Normal
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    float cubeVertices[] = {
        -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,// triangle 1 : begin
        -0.5f,-0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f, // triangle 1 : end

         0.5f, 0.5f,-0.5f,   0.0f, 0.0f, -1.0f, // triangle 2 : begin
        -0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f,-0.5f,  0.0f, 0.0f, -1.0f, // triangle 2 : end

         0.5f,-0.5f, 0.5f,  0.0f, -1.0f, 0.0f,
        -0.5f,-0.5f,-0.5f,  0.0f, -1.0f, 0.0f,
         0.5f,-0.5f,-0.5f,  0.0f, -1.0f, 0.0f,

         0.5f, 0.5f,-0.5f,  0.0f, 0.0f, -1.0f,
         0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f,
        -0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f,

        -0.5f,-0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,  -1.0f, 0.0f, 0.0f,

         0.5f,-0.5f, 0.5f,  0.0f, -1.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,  0.0f, -1.0f, 0.0f,
        -0.5f,-0.5f,-0.5f,  0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,

         0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, 0.5f,-0.5f,  1.0f, 0.0f, 0.0f,

         0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,-0.5f, 0.5f,  1.0f, 0.0f, 0.0f,

         0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,

         0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,

         0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
    };

    int cubeVerticesCount = 36;

    float pyramidVertices[] = {
         0.0f,  0.5f,  0.0f,  0.0f,  0.5f, 1.0f, // A - 0
        -0.5f, -0.5f,  0.5f,  0.0f,  0.5f, 1.0f, // E - 4
         0.5f, -0.5f,  0.5f,  0.0f,  0.5f, 1.0f, // D - 3

         0.0f,  0.5f,  0.0f,  1.0f,  0.5f, 0.0f, // A - 0
         0.5f, -0.5f,  0.5f,  1.0f,  0.5f, 0.0f, // D - 3
         0.5f, -0.5f, -0.5f,  1.0f,  0.5f, 0.0f, // C - 2

         0.0f,  0.5f,  0.0f,  0.0f,  0.5f, -1.0f, // A - 0
         0.5f, -0.5f, -0.5f,  0.0f,  0.5f, -1.0f, // C - 2
        -0.5f, -0.5f, -0.5f,  0.0f,  0.5f, -1.0f, // B - 1

         0.0f,  0.5f,  0.0f, -1.0f,  0.5f, 0.0f, // A - 0
        -0.5f, -0.5f, -0.5f, -1.0f,  0.5f, 0.0f, // B - 1
        -0.5f, -0.5f,  0.5f, -1.0f,  0.5f, 0.0f, // E - 4

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f
    };
    int pyramidVerticesCount = 18;

    float quadVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f
    };
    int quadVerticesCount = 6;

    float tetrahedronVertices[] = {
        -0.471405f, -0.333333f, -0.816497f,   1.08866f,  0.769801f, -1.88562f, // C - 2
         0.000000f,  1.000000f,  0.000000f,   1.08866f,  0.769801f, -1.88562f, // A - 0
         0.942809f, -0.333333f,  0.000000f,   1.08866f,  0.769801f, -1.88562f, // B - 1

        -0.471405f, -0.333333f,  0.816497f,  -2.17732f,  0.769802f,  0.00000f, // D - 3
         0.000000f,  1.000000f,  0.000000f,  -2.17732f,  0.769802f,  0.00000f, // A - 0
        -0.471405f, -0.333333f, -0.816497f,  -2.17732f,  0.769802f,  0.00000f, // C - 2

        -0.471405f, -0.333333f, -0.816497f,   0.00000f, -2.309400f,  0.000000f, // C - 2
         0.942809f, -0.333333f,  0.000000f,   0.00000f, -2.309400f,  0.000000f, // B - 1
        -0.471405f, -0.333333f,  0.816497f,   0.00000f, -2.309400f,  0.000000f, // D - 3

         0.942809f, -0.333333f,  0.000000f,   1.08866f,  0.769801f,  1.88562f, // B - 1
         0.000000f,  1.000000f,  0.000000f,   1.08866f,  0.769801f,  1.88562f, // A - 0
        -0.471405f, -0.333333f,  0.816497f,   1.08866f,  0.769801f,  1.88562f, // D - 3
    };
    int tetrahedronVerticesCount = 12;

    // first, configure the cube's VAO (and VBO)
    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // pyramid
    unsigned int pyramidVBO, pyramidVAO;
    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &pyramidVBO);

    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    glBindVertexArray(pyramidVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVBO, lightCubeVAO;
    glGenBuffers(1, &lightVBO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int quadVBO, quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int tetrahedronVBO, tetrahedronVAO;
    glGenVertexArrays(1, &tetrahedronVAO);
    glGenBuffers(1, &tetrahedronVBO);

    glBindBuffer(GL_ARRAY_BUFFER, tetrahedronVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedronVertices), tetrahedronVertices, GL_STATIC_DRAW);

    glBindVertexArray(tetrahedronVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);

        for (unsigned int i = 0; i < 1; i++)//sizeof(materials) / sizeof(Material); i++)
        {
            if (materials[i].transparency == 1.0f)
            {
                {
                    // be sure to activate shader when setting uniforms/drawing objects
                    lightingShader.Bind();
                    lightingShader.SetVec3("light.color", light.color);
                    lightingShader.SetVec3("light.ambient", light.ambient);
                    lightingShader.SetVec3("light.diffuse", light.diffuse); // darken diffuse light a bit
                    lightingShader.SetVec3("light.specular", light.specular);
                    lightingShader.SetVec3("light.position", light.position);

                    lightingShader.SetVec3("viewPos", camera.Position);

                    lightingShader.SetFloat("material.shininess", materials[i].shininess);
                    lightingShader.SetVec3("material.ambient", materials[i].ambient);
                    lightingShader.SetVec3("material.diffuse", materials[i].diffuse);
                    lightingShader.SetVec3("material.specular", materials[i].specular);
                    lightingShader.SetFloat("material.transparency", materials[i].transparency);

                    lightingShader.SetMat4("projection", projection);
                    lightingShader.SetMat4("view", view);

                    // world transformation
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(1.0f * i + (0.1f * i), 3.0f, 0.0f));
    //                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(2.0f, 2.0f, 2.0f));
                    lightingShader.SetMat4("model", model);

                    // render the shape
    //                glBindVertexArray(quadVBO);
    //                glDrawArrays(GL_TRIANGLES, 0, quadVerticesCount);

//                    glBindVertexArray(tetrahedronVAO);
//                    glDrawArrays(GL_TRIANGLES, 0, tetrahedronVerticesCount);

                    glBindVertexArray(cubeVAO);
                    glDrawArrays(GL_TRIANGLES, 0, cubeVerticesCount);

    //                glBindVertexArray(pyramidVAO);
    //                glDrawArrays(GL_TRIANGLES, 0, pyramidVerticesCount);
                }
            }
        }

        for (unsigned int i = 0; i < sizeof(materials) / sizeof(Material); i++)
        {
            if (materials[i].transparency < 1.0f)
            {
                {
                    // be sure to activate shader when setting uniforms/drawing objects
                    lightingShader.Bind();
                    lightingShader.SetVec3("light.color", light.color);
                    lightingShader.SetVec3("light.ambient", light.ambient);
                    lightingShader.SetVec3("light.diffuse", light.diffuse); // darken diffuse light a bit
                    lightingShader.SetVec3("light.specular", light.specular);
                    lightingShader.SetVec3("light.position", light.position);

                    lightingShader.SetVec3("viewPos", camera.Position);

                    lightingShader.SetFloat("material.shininess", materials[i].shininess);
                    lightingShader.SetVec3("material.ambient", materials[i].ambient);
                    lightingShader.SetVec3("material.diffuse", materials[i].diffuse);
                    lightingShader.SetVec3("material.specular", materials[i].specular);
                    lightingShader.SetFloat("material.transparency", materials[i].transparency);

                    lightingShader.SetMat4("projection", projection);
                    lightingShader.SetMat4("view", view);

                    // world transformation
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(1.0f * i + (0.1f * i), 3.0f, 0.0f));
                    //                model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(2.0f, 2.0f, 2.0f));
                    lightingShader.SetMat4("model", model);

                    // render the shape
//                glBindVertexArray(quadVBO);
//                glDrawArrays(GL_TRIANGLES, 0, quadVerticesCount);

//                    glBindVertexArray(tetrahedronVAO);
//                    glDrawArrays(GL_TRIANGLES, 0, tetrahedronVerticesCount);

                    glBindVertexArray(cubeVAO);
                    glDrawArrays(GL_TRIANGLES, 0, cubeVerticesCount);

//                glBindVertexArray(pyramidVAO);
//                glDrawArrays(GL_TRIANGLES, 0, pyramidVerticesCount);
                }
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
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
            lightCubeShader.SetMat4("model", model);

            glBindVertexArray(lightCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 18); // 36
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
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteBuffers(1, &quadVBO);

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

