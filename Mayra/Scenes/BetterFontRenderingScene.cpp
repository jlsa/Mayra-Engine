#include "BetterFontRenderingScene.h"

struct InputTextCallback_UserData
{
    std::string*            Str;
    ImGuiInputTextCallback  ChainCallback;
    void*                   ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        std::string* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }

    return 0;
}

namespace Mayra
{
    BetterFontRenderingScene::BetterFontRenderingScene()
      : outputString("> example.txt"), m_ClearColor(glm::vec4(62.0f / 255.0f, 61.0f / 255.0f, 64.0f / 255.0f, 1.0f))
    {
        m_Camera = new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
        m_Projection = m_Camera->GetProjectionMatrix();
        m_Camera->SetPosition(glm::vec3(-0.33f, -0.66f, 0.0f));

        fontShader = new Mayra::Shader(SHADERS "Fonts.vert", SHADERS "Fonts.frag");
        prepare(fontShader);
    }

    void BetterFontRenderingScene::OnUpdate(float /* deltaTime */)
    {}

    BetterFontRenderingScene::~BetterFontRenderingScene()
    {
        // delete all from vector
        delete fontShader;
    }

    void BetterFontRenderingScene::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        RenderText(fontShader, outputString, 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
        RenderText(fontShader, questString, 25.0f, 600.0f, 1.0f, glm::vec3(0.863f, 0.078f, 0.235f));
    }

    void BetterFontRenderingScene::OnImGuiRender()
    {
        ImGui::Begin("Better Font Rendering");

        if (outputString.empty())
            outputString.push_back(0);
        BetterFontRenderingScene::InputText("##Lala", &outputString);
        ImGui::Text("Data: %hhd\nSize:%lu\nCapacity: %lu", outputString.c_str()[0], outputString.size(), outputString.capacity());

        if (questString.empty())
            questString.push_back(0);
        BetterFontRenderingScene::InputTextMultiline("##Quest", &questString, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 15));
        ImGui::Text("Data: %hhd\nSize:%lu\nCapacity: %lu", questString.c_str()[0], questString.size(), questString.capacity());

        ImGui::End();
    }

    bool BetterFontRenderingScene::InputText(const char *label, std::string *str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;

        return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    bool BetterFontRenderingScene::InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
    }

    bool BetterFontRenderingScene::InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    void BetterFontRenderingScene::prepare(Mayra::Shader* shader)
    {
        // OpenGL state
        // ------------
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // compile and setup the shader
        // ----------------------------
//        glm::mat4 projection = m_Camera->GetProjectionMatrix();
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1280), 0.0f, static_cast<float>(720));
        shader->Bind();
        shader->SetMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.5f));
        shader->SetMat4("model", model);
//        glUniformMatrix4fv(glGetUniformLocation(shader->GetRendererID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // FreeType
        // --------
        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//            return -1;
        }

        //    "Antonio-Bold.ttf"
        // find path to font
        std::string font_name = FONTS "Antonio-Bold.ttf";
        if (font_name.empty())
        {
            std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
//            return -1;
        }

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//            return -1;
        }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            lineSpacing = face->height;
            scaledLineSpacing = face->size->metrics.height;

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                             GL_TEXTURE_2D,
                             0,
                             GL_RED,
                             face->glyph->bitmap.width,
                             face->glyph->bitmap.rows,
                             0,
                             GL_RED,
                             GL_UNSIGNED_BYTE,
                             face->glyph->bitmap.buffer
                             );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Mayra::FontCharacter character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Mayra::FontCharacter>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        // configure VAO/VBO for texture quads
        // -----------------------------------
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // render line of text
    // -------------------
    void BetterFontRenderingScene::RenderText(Mayra::Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color)
    {
        float originalx = x;
        // activate corresponding render state
        shader->Bind();
        shader->SetVec3("textColor", color);
//        glUniform3f(glGetUniformLocation(shader->GetRendererID(), "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;
        if (text[0] == '\0')
            return;
        int liney = 0;
        for (c = text.begin(); c != text.end(); c++)
        {
            if (*c == '\n')
            {
                liney -= scaledLineSpacing / 64.0f;
                x = originalx;
                continue;
            }
            else
            {
                // get line of characters here
                Mayra::FontCharacter ch = Characters[*c];

                float xpos = x + ch.Bearing.x * scale;
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale + liney;

                float w = ch.Size.x * scale;
                float h = ch.Size.y * scale;
                // update VBO for each character
                float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }
                };
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.TextureID);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);

                // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
                x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            }
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
