//
//  shader.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 05/12/2020.
//

#ifndef shader_h
#define shader_h

#include <glad/glad.h>
#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Mayra
{
    class Shader
    {
    public:
        unsigned int ID;
        // constructor generates the shader on the fly
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
            : m_VertexPath(vertexPath), m_FragmentPath(fragmentPath), m_GeometryPath(geometryPath)
        {
            m_LoadShaders();
        }

        // activate the shader
        // ---------------------------------------------------------------------
        void Use()
        {
            glUseProgram(ID);
        }

        // utility uniform functions
        // ---------------------------------------------------------------------
        void SetBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        // ---------------------------------------------------------------------
        void SetInt(const std::string &name, int16_t value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        // ---------------------------------------------------------------------
        void SetFloat(const std::string &name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        // ---------------------------------------------------------------------
        void SetVec2(const std::string &name, const glm::vec2 &value) const
        {
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void SetVec2(const std::string &name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
        }
        // ---------------------------------------------------------------------
        void SetVec3(const std::string &name, const glm::vec3 &value) const
        {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void SetVec3(const std::string &name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
        }
        // ---------------------------------------------------------------------
        void SetVec4(const std::string &name, const glm::vec4 &value) const
        {
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void SetVec4(const std::string &name, float x, float y, float z, float w) const
        {
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        }
        // ---------------------------------------------------------------------
        void SetMat2(const std::string &name, const glm::mat2 &mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ---------------------------------------------------------------------
        void SetMat3(const std::string &name, const glm::mat3 &mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ---------------------------------------------------------------------
        void SetMat4(const std::string &name, const glm::mat4 &mat) const
        {
            GLint location = glGetUniformLocation(ID, name.c_str());
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
        }

    private:
        const char* m_VertexPath;
        const char* m_FragmentPath;
        const char* m_GeometryPath;
    
        // utility function for checking shader compilation/linking errors.
        // ---------------------------------------------------------------------
        void checkCompileErrors(unsigned int shader, std::string type)
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
            
            int success;
            char infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type:" << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
        
        std::string m_ReadShaderSourceCode(const char* path)
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
        
        unsigned int m_CreateShader(std::string shaderSource, unsigned int glShaderType, std::string type)
        {
            unsigned int shaderID;
            const char* shaderCode = shaderSource.c_str();
            
            shaderID = glCreateShader(glShaderType);
            glShaderSource(shaderID, 1, &shaderCode, NULL);
            glCompileShader(shaderID);
            checkCompileErrors(shaderID, type);
            
            return shaderID;
        }
        
        unsigned int m_CreateShaderProgram(unsigned int vertex, unsigned int fragment, unsigned int geometry)
        {
            unsigned int programID;
            
            programID = glCreateProgram();
            glAttachShader(programID, vertex);
            glAttachShader(programID, fragment);
            if (m_GeometryPath != nullptr)
                glAttachShader(programID, geometry);
            
            glLinkProgram(programID);
            checkCompileErrors(programID, "PROGRAM");
            
            return programID;
        }
        
        void m_LoadShaders()
        {
            // compile shaders
            unsigned int vertex, fragment, geometry;

            vertex = m_CreateShader(m_ReadShaderSourceCode(m_VertexPath), GL_VERTEX_SHADER, "VERTEX");
            fragment = m_CreateShader(m_ReadShaderSourceCode(m_FragmentPath), GL_FRAGMENT_SHADER, "FRAGMENT");
            geometry = m_CreateShader(m_ReadShaderSourceCode(m_GeometryPath), GL_GEOMETRY_SHADER, "GEOMETRY");
            
            
            glDeleteProgram(ID); // ???
            // shader program
            ID = m_CreateShaderProgram(vertex, fragment, geometry);

            // delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            if (m_GeometryPath != nullptr)
            {
                glDeleteShader(geometry);
            }
        }
    };
}

#endif /* shader_h */
