#ifndef BetterFontRenderingScene_h
#define BetterFontRenderingScene_h

#include <Mayra.hpp>
#include <Scene.hpp>

#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Input.hpp"

#include <imgui.h>

#include <FontCharacter.h>

namespace Mayra
{
    class BetterFontRenderingScene : public Scene
    {
    public:
        BetterFontRenderingScene();
        ~BetterFontRenderingScene();
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void RenderText(Mayra::Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color);

        bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
        bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
        bool InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);

        std::map<GLchar, Mayra::FontCharacter> Characters;
        unsigned int VAO, VBO;

        std::string outputString;
        std::string questString;
        void prepare(Mayra::Shader* shader);
        Mayra::Shader* fontShader;

        int lineSpacing;
        int scaledLineSpacing;
    private:
        glm::vec4 m_ClearColor;
        glm::mat4 m_Projection;
        OrthographicCamera *m_Camera;
    };
}

#endif /* BetterFontRenderingScene_h */
