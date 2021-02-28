#include "FreeTypeFontScene.h"

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
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
    FreeTypeFontScene::FreeTypeFontScene()
      : outputString("test hallo"), m_ClearColor(glm::vec4(62.0f / 255.0f, 61.0f / 255.0f, 64.0f / 255.0f, 1.0f))
    {
        m_Camera = new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
        m_Projection = m_Camera->GetProjectionMatrix();
        m_Camera->SetPosition(glm::vec3(-0.33f, -0.66f, 0.0f));

        fontShader = new Mayra::Shader(SHADERS "Fonts.vert", SHADERS "Fonts.frag");
        prepare(fontShader);
    }

    void FreeTypeFontScene::OnUpdate(float /* deltaTime */)
    {}

    FreeTypeFontScene::~FreeTypeFontScene()
    {
        // delete all from vector
        delete fontShader;
    }

    void FreeTypeFontScene::OnRender()
    {
        GLCall(glClearColor(m_ClearColor.r,
                            m_ClearColor.g,
                            m_ClearColor.b,
                            m_ClearColor.a));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        RenderText(fontShader, outputString, 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));
        RenderText(fontShader, questString, 25.0f, 720.0f - 25.0f, 1.0f, glm::vec3(0.863f, 0.078f, 0.235f));
        RenderText(fontShader, "Hallo!", 540.0f, 570.0f, 0.5f, glm::vec3(0.3f, 0.7f, 0.9f));
    }

    void FreeTypeFontScene::OnImGuiRender()
    {
        ImGui::Begin("TextRendering");
        static char password[64] = "password123";

        if (ImGui::InputText("password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password))
        {
            outputString = std::string(password);
        }
        ImGui::SameLine(); HelpMarker("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging.\n");
        ImGui::InputTextWithHint("password (w/ hint)", "<password>", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
        ImGui::InputText("password (clear)", password, IM_ARRAYSIZE(password));


        // To wire InputText() with std::string or any other custom string type,
        // you can use the ImGuiInputTextFlags_CallbackResize flag + create a custom ImGui::InputText() wrapper
        // using your preferred type. See misc/cpp/imgui_stdlib.h for an implementation of this using std::string.
        HelpMarker(
                   "Using ImGuiInputTextFlags_CallbackResize to wire your custom string type to InputText().\n\n"
                   "See misc/cpp/imgui_stdlib.h for an implementation of this for std::string.");
        struct Funcs
        {
            static int MyResizeCallback(ImGuiInputTextCallbackData* data)
            {
                if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                {
                    ImVector<char>* my_str = (ImVector<char>*)data->UserData;
                    IM_ASSERT(my_str->begin() == data->Buf);
                    my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
                    data->Buf = my_str->begin();
                }
                return 0;
            }

            // Note: Because ImGui:: is a namespace you would typically add your own function into the namespace.
            // For example, you code may declare a function 'ImGui::InputText(const char* label, MyString* my_str)'
            static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
            {
                IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
                return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
            }
        };

        // For this demo we are using ImVector as a string container.
        // Note that because we need to store a terminating zero character, our size/capacity are 1 more
        // than usually reported by a typical string class.
//        static ImVector<char> my_str;
//        if (my_str.empty())
//            my_str.push_back(0);
//        Funcs::MyInputTextMultiline("##MyStr", &my_str, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
//        ImGui::Text("Data: %p\nSize: %d\nCapacity: %d", (void*)my_str.begin(), my_str.size(), my_str.capacity());

        if (outputString.empty())
            outputString.push_back(0);
        FreeTypeFontScene::InputText("##Lala", &outputString);
        ImGui::Text("Data: %hhd\nSize:%lu\nCapacity: %lu", outputString.c_str()[0], outputString.size(), outputString.capacity());

        if (questString.empty())
            questString.push_back(0);
        FreeTypeFontScene::InputTextMultiline("##Quest", &questString, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5));
        ImGui::Text("Data: %hhd\nSize:%lu\nCapacity: %lu", questString.c_str()[0], questString.size(), questString.capacity());

        ImGui::End();
    }

    bool FreeTypeFontScene::InputText(const char *label, std::string *str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;

        return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    bool FreeTypeFontScene::InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
    }

    bool FreeTypeFontScene::InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    void FreeTypeFontScene::prepare(Mayra::Shader* shader)
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
        glUniformMatrix4fv(glGetUniformLocation(shader->GetRendererID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

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
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
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
    void FreeTypeFontScene::RenderText(Mayra::Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color)
    {
        // activate corresponding render state
        shader->Bind();
        glUniform3f(glGetUniformLocation(shader->GetRendererID(), "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;
        if (text[0] == '\0')
            return;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

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
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
