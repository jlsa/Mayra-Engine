#include "ModelLoadingScene.h"
#include "Input.hpp"
#include "Key.hpp"
#include "MouseButton.h"
#include <3Dshapes.hpp>

#include <Window.hpp>

namespace Mayra
{
    ModelLoadingScene::ModelLoadingScene()
    : m_ClearColor(glm::vec4(62.0f / 255.0f, 61.0f / 255.0f, 64.0f / 255.0f, 1.0f))
    {
        m_ScreenSize = glm::vec2(Mayra::Window::Instance()->GetProps()->Width, Mayra::Window::Instance()->GetProps()->Height);
        m_OrthoCamera = new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
        m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

        m_Playmode = false;
        m_Projection = glm::perspectiveFov(glm::radians(m_Camera->Zoom), m_ScreenSize.x, m_ScreenSize.y, 0.1f, 10000.0f);
        m_Camera->SetProjectionMatrix(m_Projection);

        m_CubeShader = new Mayra::Shader(SHADERS "cubemaps.vert", SHADERS "cubemaps.frag");

        m_VertexArray = new VertexArray();
        m_VertexBuffer = new VertexBuffer();

        m_VertexBuffer->CreateBuffer(Shapes::cube2.vertices, Shapes::cube2.verticesCount * Shapes::cube2.stride * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VertexArray->AddBuffer(m_VertexBuffer, layout);

        // load textures
        // -------------
        m_CubeTexture = Mayra::Texture2D::LoadFromFile(TEXTURES "wall.jpg", false);

        // shader configuration
        // --------------------
        m_CubeShader->Bind();
        m_CubeShader->SetInt("texture1", 0);

        m_Skybox = new Skybox();
    }

    void ModelLoadingScene::OnUpdate(float deltaTime)
    {
        if (m_Playmode)
        {
            if (Input::Instance()->IsKeyDown(KeyCode::Escape))
            {
                OnStop();
            }
            glm::vec2 offset = Input::Instance()->GetMouseDelta(); // if no movement delta should be zero
            m_Camera->ProcessMouseMovement(offset.x, offset.y);
            m_Camera->ProcessMouseScroll(Mayra::Input::Instance()->GetScrollOffset().y);

            if (Input::Instance()->IsMouseButtonDown(Mayra::MouseButton::Left)) {
                std::cout << "Click!" << std::endl;
            }

            if (Input::Instance()->IsAnyKey(KeyCode::W, KeyCode::Up))
                m_Camera->ProcessKeyboard(FORWARD, deltaTime);
            if (Input::Instance()->IsAnyKey(KeyCode::S, KeyCode::Down))
                m_Camera->ProcessKeyboard(BACKWARD, deltaTime);
            if (Input::Instance()->IsAnyKey(KeyCode::A, KeyCode::Left))
                m_Camera->ProcessKeyboard(LEFT, deltaTime);
            if (Input::Instance()->IsAnyKey(KeyCode::D, KeyCode::Right))
                m_Camera->ProcessKeyboard(RIGHT, deltaTime);

            if (Input::Instance()->IsKey(KeyCode::Space))
                m_Camera->ProcessKeyboard(UP, deltaTime);
            if (Input::Instance()->IsKey(KeyCode::X))
                m_Camera->ProcessKeyboard(DOWN, deltaTime);
        }
        else
        {
            if (Input::Instance()->IsKeyDown(KeyCode::P))
                OnPlay();
        }
    }

    ModelLoadingScene::~ModelLoadingScene()
    {
        // delete all from vector
        delete m_VertexArray;
        delete m_VertexBuffer;
        delete m_CubeShader;
        delete m_CubeTexture;
        delete m_Skybox;

        delete m_Camera;
        delete m_OrthoCamera;
    }

    void ModelLoadingScene::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // draw scene as normal
        m_CubeShader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_Camera->GetViewMatrix();
        glm::mat4 projection = glm::perspectiveFov(glm::radians(m_Camera->Zoom), m_ScreenSize.x, m_ScreenSize.y, 0.1f, 10000.0f);

        m_Camera->SetProjectionMatrix(projection);
        for (int x = 0; x < 3; x++)
        {
            float xPos = 1.0f * (float)x + 0.1f * (float)x;
            for (int y = 0; y < 3; y++)
            {
                float yPos = 1.0f * (float)y + 0.1f * (float)y;
                for (int z = 0; z < 3; z++)
                {
                    float zPos = 1.0f * (float)z + 0.1f * (float)z;
                    {
                        model = glm::mat4(1.0f);

                        model = glm::translate(model, glm::vec3(xPos, yPos, zPos));
                        m_CubeShader->SetMat4("model", model);
                        m_CubeShader->SetMat4("view", view);
                        m_CubeShader->SetMat4("projection", projection);

                        m_CubeTexture->Bind();
                        Renderer::Instance()->Draw(m_VertexArray, m_CubeShader, 36);
                    }
                }
            }
        }
        m_Skybox->Render(m_Camera);
    }

    void ModelLoadingScene::OnImGuiRender()
    {
        ImGui::Begin("Skybox");
        if (ImGui::Button("Play")) {
            OnPlay();
        }

        if (ImGui::Button("Show Fill")) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        ImGui::SameLine();
        if (ImGui::Button("Show Line")) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        ImGui::SameLine();
        if (ImGui::Button("Show Point")) {
            glPointSize(10.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
        ImGui::End();
    }

    void ModelLoadingScene::OnPlay()
    {
        m_Playmode = true;
        glfwSetInputMode(Mayra::Window::Instance()->Get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Input::Instance()->SetMousePosition(Window::Instance()->GetProps()->GetCenter());
    }

    void ModelLoadingScene::OnStop()
    {
        m_Playmode = false;
        glfwSetInputMode(Window::Instance()->Get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        Input::Instance()->SetMousePosition(Window::Instance()->GetProps()->GetCenter());
    }

    void ModelLoadingScene::OnPause()
    {

    }
}
