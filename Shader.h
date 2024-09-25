#ifndef __Shader_h_
#define __Shader_h_

#include <string>
#include <glm/glm.hpp>

// General purpose shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader {
public:
    // state
    unsigned int ID = 0;
    // constructor
    Shader() : ID() { }
    Shader(const char* vertexPath, const char* fragmentPath);

    // sets the current shader as active
    Shader& use();
    Shader& reset();

    char *read(const char *FileName);
    //GLuint compile(const char *source, const GLenum type);
    // compiles the shader from given source code
    void Load(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); // note: geometry source code is optional 
    // utility functions
    void setBool(const std::string& name, bool value) const;
    void SetFloat(const char* name, float value, bool useShader = false);
    void SetInteger(const char* name, int value, bool useShader = false);
    void SetVector2f(const char* name, float x, float y, bool useShader = false);
    void SetVector2f(const char* name, const glm::fvec2 & value, bool useShader = false);
    void SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
    
    void SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
    void SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
    void SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
    
    void SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

private:
    // checks if compilation or linking failed and if so, print the error logs
    void checkCompileErrors(unsigned int object, std::string type);
};

#endif //__Shader_h_
