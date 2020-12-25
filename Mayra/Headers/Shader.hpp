//
//  shader.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 05/12/2020.
//

#ifndef shader_h
#define shader_h

#include <GL.hpp>
#include <glad/glad.h>
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Mayra
{
    struct ShaderSource
    {
        std::string Source;
        unsigned int ShaderType;

        static std::string GetStringOfType(unsigned int type)
        {
            switch (type)
            {
                case GL_VERTEX_SHADER:      return "VERTEX";
                case GL_FRAGMENT_SHADER:    return "FRAGMENT";
            }
            ASSERT(false);
            return "";
        }
    };

    struct ShaderProgramSource
    {
        Mayra::ShaderSource VertexSource;
        Mayra::ShaderSource FragmentSource;
    };

    struct ShadersCompiled
    {
        unsigned int VertexShaderID;
        unsigned int FragmentShaderID;
    };

    class Shader
    {
    public:
        // constructor generates the shader on the fly
        Shader(const char* vertexPath, const char* fragmentPath);
        ~Shader();
        void Bind() const;
        void Unbind() const;
        void SetBool(const std::string& name, bool value);
        void SetInt(const std::string& name, int16_t value);
        void SetFloat(const std::string& name, float value);
        void SetVec2(const std::string& name, const glm::vec2& value);
        void SetVec2(const std::string& name, float x, float y);
        void SetVec3(const std::string& name, const glm::vec3& value);
        void SetVec3(const std::string& name, float x, float y, float z);
        void SetVec4(const std::string& name, const glm::vec4& value);
        void SetVec4(const std::string& name, float x, float y, float z, float w);
        void SetMat2(const std::string& name, const glm::mat2& mat);
        void SetMat3(const std::string& name, const glm::mat3& mat);
        void SetMat4(const std::string& name, const glm::mat4& mat);

    private:
        void m_LoadShaders();
        void CheckForCompileErrors(unsigned int shader, std::string type);
        std::string ParseFile(const char* path);
        unsigned int Compile(Mayra::ShaderSource shader);
        unsigned int CreateProgram(Mayra::ShadersCompiled shaders);
        unsigned int CreateProgram(unsigned int vertex, unsigned int fragment);
        int GetUniformLocation(const std::string& name);

        unsigned int m_RendererID;
        const char* m_VertexPath;
        const char* m_FragmentPath;

        std::unordered_map<std::string, int> m_UniformLocationCache;
    };
}

#endif /* shader_h */
