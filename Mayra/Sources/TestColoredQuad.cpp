#include "TestColoredQuad.hpp"
#include "TestFancyQuad.hpp"
#include "TestClearColor.hpp"

namespace Test
{
    TestColoredQuad TestColoredQuad::m_Instance;
    
    const char *vertexShaderSource2 = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 u_Color;\n"
    "void main()\n"
    "{\n"
    "   FragColor = u_Color;\n"
    "}\n\0";


    void TestColoredQuad::OnUpdate(TestsManager*, float)
    {

    }

    void TestColoredQuad::Suspend()
    {

    }

    void TestColoredQuad::Resume()
    {

    }

    void TestColoredQuad::OnAttach()
    {
        m_ClearColor = glm::vec4(0.941f, 1.0f, 1.0f, 1.0f);
        m_QuadColor = glm::vec4(0.412f, 0.255f, 0.222f, 1.0f);

        //        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource2, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    void TestColoredQuad::OnDetach()
    {
        GLCall(glDeleteVertexArrays(1, &VAO));
        GLCall(glDeleteBuffers(1, &VBO));
        GLCall(glDeleteBuffers(1, &EBO));
        GLCall(glDeleteProgram(shaderProgram));
    }

    void TestColoredQuad::OnRender(TestsManager*)
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUseProgram(shaderProgram));
        GLCall(int location = glGetUniformLocation(shaderProgram, "u_Color"));
        GLCall(glUniform4f(location, m_QuadColor.r, m_QuadColor.g, m_QuadColor.b, m_QuadColor.a));

        GLCall(glBindVertexArray(VAO));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    }

    void TestColoredQuad::OnImGuiRender(TestsManager* testsManager)
    {
        ImGui::ColorEdit4("Clear Color", glm::value_ptr(m_ClearColor));
        ImGui::Separator();
        ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));
        ImGui::Separator();
        if (ImGui::Button("Clear Color Test"))
        {
            testsManager->ChangeTest(TestClearColor::Instance());
        }
        ImGui::SameLine();
        if (ImGui::Button("Fancy Quad Test"))
        {
            testsManager->ChangeTest(TestFancyQuad::Instance());
        }
    }
}
