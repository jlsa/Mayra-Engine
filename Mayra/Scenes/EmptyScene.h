#ifndef EmptyScene_h
#define EmptyScene_h

#include <Mayra.hpp>
#include <Scene.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

namespace Mayra
{
  class EmptyScene : public Scene
  {
  public:
    EmptyScene();
    ~EmptyScene();
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    glm::vec4 m_ClearColor;
    glm::mat4 m_Projection;
    OrthographicCamera *m_Camera;
  };
}

#endif /* EmptyScene_h */
