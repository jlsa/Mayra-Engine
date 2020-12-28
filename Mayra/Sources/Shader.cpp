#include "Shader.hpp"

namespace Mayra
{
    Shader::Shader()
    : m_RendererID(0), m_VertexPath(""), m_FragmentPath("")
    {}
    Shader::Shader(const char* vertexPath, const char* fragmentPath)
        : m_RendererID(0), m_VertexPath(vertexPath), m_FragmentPath(fragmentPath)
    {
        m_LoadShaders();
    }

    Shader::~Shader()
    {
        GLCall(glDeleteProgram(m_RendererID));
    }

    // activate the shader
    // ---------------------------------------------------------------------
    void Shader::Bind() const
    {
        GLCall(glUseProgram(m_RendererID));
    }

    void Shader::Unbind() const
    {
        GLCall(glUseProgram(0));
    }

    void Shader::SetBool(const std::string& name, bool value)
    {
        GLCall(glUniform1i(GetUniformLocation(name), (int)value));
    }

    void Shader::SetInt(const std::string& name, int16_t value)
    {
        GLCall(glUniform1i(GetUniformLocation(name), value));
    }

    void Shader::SetFloat(const std::string& name, float value)
    {
        GLCall(glUniform1f(GetUniformLocation(name), value));
    }

    void Shader::SetVec2(const std::string& name, const glm::vec2& value)
    {
        GLCall(glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
    }

    void Shader::SetVec2(const std::string& name, float x, float y)
    {
        GLCall(glUniform2f(GetUniformLocation(name), x, y));
    }

    void Shader::SetVec3(const std::string& name, const glm::vec3& value)
    {
        GLCall(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
    }

    void Shader::SetVec3(const std::string& name, float x, float y, float z)
    {
        GLCall(glUniform3f(GetUniformLocation(name), x, y, z));
    }

    void Shader::SetVec4(const std::string& name, const glm::vec4& value)
    {
        GLCall(glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
    }

    void Shader::SetVec4(const std::string& name, float x, float y, float z, float w)
    {
        GLCall(glUniform4f(GetUniformLocation(name), x, y, z, w));
    }

    void Shader::SetMat2(const std::string& name, const glm::mat2& mat)
    {

        GLCall(glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void Shader::SetMat3(const std::string& name, const glm::mat3& mat)
    {
        GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
    {
        GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void Shader::CheckForCompileErrors(unsigned int shader, std::string type)
    {
        int result;
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
            if (result == GL_FALSE)
            {
                int length;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)alloca(length * sizeof(char));
                glGetShaderInfoLog(shader, length, &length, message);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << message << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetShaderiv(shader, GL_LINK_STATUS, &result);
            if (result == GL_FALSE)
            {
                int length;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)alloca(length * sizeof(char));
                glGetShaderInfoLog(shader, length, &length, message);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type:" << type << "\n"
                << message << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    std::string Shader::ParseFile(const char* path)
    {
        // 1. retrieve the shader source code from filePath
        std::string shaderCode;
        std::ifstream vShaderFile;

        // ensure ifstream objects can throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // open files
            vShaderFile.open(path);

            std::stringstream vShaderStream;

            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();

            // convert stream into string
            shaderCode = vShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        return shaderCode;
    }

    unsigned int Shader::Compile(Mayra::ShaderSource shader)
    {
        unsigned int shaderID;
        const char* shaderCode = shader.Source.c_str();

        shaderID = glCreateShader(shader.ShaderType);
        glShaderSource(shaderID, 1, &shaderCode, NULL);
        glCompileShader(shaderID);
        CheckForCompileErrors(shaderID, shader.GetStringOfType(shader.ShaderType));

        return shaderID;
    }

    unsigned int Shader::CreateProgram(Mayra::ShadersCompiled compiledShaders)
    {
        unsigned int programID;

        programID = glCreateProgram();
        glAttachShader(programID, compiledShaders.VertexShaderID);
        glAttachShader(programID, compiledShaders.FragmentShaderID);

        glLinkProgram(programID);
        CheckForCompileErrors(programID, "PROGRAM");

        return programID;
    }

    void Shader::m_LoadShaders()
    {
        Mayra::ShaderSource vertexShader;
        vertexShader.Source = ParseFile(m_VertexPath);
        vertexShader.ShaderType = GL_VERTEX_SHADER;

        Mayra::ShaderSource fragmentShader;
        fragmentShader.Source = ParseFile(m_FragmentPath);
        fragmentShader.ShaderType = GL_FRAGMENT_SHADER;

        Mayra::ShaderProgramSource source;
        source.VertexSource = vertexShader;
        source.FragmentSource = fragmentShader;

        Mayra::ShadersCompiled compiledShaders;
        compiledShaders.VertexShaderID = Compile(vertexShader);
        compiledShaders.FragmentShaderID = Compile(fragmentShader);
        m_RendererID = CreateProgram(compiledShaders);

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(compiledShaders.VertexShaderID);
        glDeleteShader(compiledShaders.FragmentShaderID);
    }

    int Shader::GetUniformLocation(const std::string &name)
    {
        if (m_UniformLocationCache.find(name.c_str()) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name.c_str()];
        GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
        if (location == -1)
            std::cout << "WARNING::SHADER::UNIFORM_DOES_NOT_EXIST::" << name << std::endl;

        m_UniformLocationCache[name] = location;
        return location;
    }
}
