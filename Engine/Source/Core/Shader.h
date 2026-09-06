#pragma once
#include <glad/glad.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

namespace AtomToolKit {

    class Shader
    {
    public:
        Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

        void CompileShaders();
        void CheckForErrors(const GLenum& shaderType) const;
        void CheckProgramErrors(const GLuint& shaderType) const;
        void AttachShaders() const;
        void UseProgram() const { if (programId != 0) glUseProgram(programId); }

        ~Shader();
    public:
        constexpr void SetUniformInt(const char* location, int value) {
            glUniform1i(glGetUniformLocation(programId, location), value);
        }
        constexpr void SetUniformMatrix4(const char* location, glm::mat4 val) {
            glUniformMatrix4fv(glGetUniformLocation(programId, location), 1, false, glm::value_ptr(val));
        }
        constexpr void SetUniformVec3(const char* location, glm::vec3 val) {
            glUniform3f(glGetUniformLocation(programId, location), val.x, val.y, val.z);
        }

        constexpr void SetUniformFloat(const char* location, float val) {
            glUniform1f(glGetUniformLocation(programId, location), val);
        }

        constexpr void SetUniformBool(const char* location, bool val) {
            glUniform1i(glGetUniformLocation(programId, location), val);
        }

        constexpr int GetProgramID() const noexcept {
            return programId;
        }

        std::string GetCode(GLenum shaderType) {
            if (shaderType == GL_VERTEX_SHADER) return vertexCode;
            else if (shaderType == GL_FRAGMENT_SHADER) return fragmentCode;
        }
    private:
        std::string vertexCode, fragmentCode;

        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int programId;
    };


}