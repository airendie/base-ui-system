#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Vector2.h"


// constructor generates the shader on the fly
// ------------------------------------------------------------------------
Shader::Shader( const char* vertexPath, const char* fragmentPath )
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    try
    {
        // open files
        vShaderFile.open( vertexPath );
        fShaderFile.open( fragmentPath );
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, NULL );
    glCompileShader( vertex );
    checkCompileErrors( vertex, "VERTEX" );

    // fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, NULL );
    glCompileShader( fragment );
    checkCompileErrors( fragment, "FRAGMENT" );

    // shader Program
    ID = glCreateProgram();
    glAttachShader( ID, vertex );
    glAttachShader( ID, fragment );
    glLinkProgram( ID );
    checkCompileErrors( ID, "PROGRAM" );

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

char* Shader::read( const char* filename )
{
    FILE* file;
    char* buffer;
    size_t size;

#ifdef _WIN64
    errno_t err = fopen_s( &file, filename, "rb" );
#else
    file = fopen( filename, "rb" );
#endif
    if (!file)
    {
        fprintf( stderr, "Unable to open file \"%s\".\n", filename );
        return 0;
    }
    fseek( file, 0, SEEK_END );
    size = ftell( file );
    fseek( file, 0, SEEK_SET );
    buffer = new char[size + 1];
    fread( buffer, sizeof( char ), size, file );
    buffer[size] = 0;
    fclose( file );
    return buffer;
}

//GLuint Shader::compile(const char *source, const GLenum type)
//{
//    GLint compile_status;
//    GLuint handle = glCreateShader(type);
//    glShaderSource(handle, 1, &source, 0);
//    glCompileShader(handle);
//
//    glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_status);
//    if (compile_status == GL_FALSE)
//    {
//        GLchar messages[256];
//        glGetShaderInfoLog(handle, sizeof(messages), 0, &messages[0]);
//        fprintf(stderr, "%s\n", messages);
//        exit(EXIT_FAILURE);
//    }
//    return handle;
//}

Shader& Shader::use()
{
    glUseProgram( this->ID );
    return *this;
}

Shader& Shader::reset()
{
    glUseProgram( 0 );
    return *this;
}
// Предложение:
/// 1. Функционал классов TextRenderer и TextObject слишком схож. Don't Repeat Yourself. 
///    Класс TextRenderer -> метод класса TextObject Render (fvec2 pos0) (pos0 - локальная точка нуля координат в глобальной системе отсчета)
/// 2. методы Init и Draw объединяем в один виртуальный метод Render(...), выполняющий все сразу.
///    Нам нет смысла инициализировать переменную каждого класса отдельно, если мы можем сразу отрисовать ее.
///	3. Отказ от класса Vector2 в пользу GLM. Как уже обговаривалось ранее, наш кастомный класс... мягко говоря, ущербен. 
///	4. Для корректной работы выравнивания требуется знать, где тот или иной объект начинается и где оканчивается.
///    Волей случая, все объекты (прямоугольник, коробка и текст) спокойно описываются прямоугольником, в котором вписаны эти Shape.
///    Предлагаю и дальше исходить из этого соображения.
///    Для того, чтобы однозначно задать нужный нам прямоугольник, нужно знать координату его левого нижнего угла (или центра) и длины сторон.
///    От чего лучше отталкиваться: от угла и или его середины - я пока не решил. Пока что от л/н угла.
///	   Для каждого объекта заводим метод, принимающий на вход 2 адреса: на позицию центра + pos0 и на размеры прямоугольника.
/// 5. Выгодно ли создавать словарь сразу для всех символов? Предлагаю после добавить динамическое расширение, чтобы сэкономить место.

void Shader::Load( const char* vertexPath, const char* fragmentPath, const char* geometryPath )
{

    unsigned int vertex, fragment, geometry;
    // 1. retrieve the vertex/fragment source code from filePath
    if (vertexPath != nullptr)
    {
        std::string vertexCode;
        std::ifstream vShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        try
        {
            // open files
            vShaderFile.open( vertexPath );
            std::stringstream vShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();

        // shaders compiling

        // vertex shader
        vertex = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( vertex, 1, &vShaderCode, NULL );
        glCompileShader( vertex );
        checkCompileErrors( vertex, "VERTEX" );
    }
    if (fragmentPath != nullptr)
    {
        std::string fragmentCode;
        std::ifstream fShaderFile;

        fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        try
        {
            // open files
            fShaderFile.open( fragmentPath );
            std::stringstream fShaderStream;
            // read file's buffer contents into streams
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            fShaderFile.close();
            // convert stream into string
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* fShaderCode = fragmentCode.c_str();
    
        // fragment Shader
        fragment = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( fragment, 1, &fShaderCode, NULL );
        glCompileShader( fragment );
        checkCompileErrors( fragment, "FRAGMENT" );
    }

    if (geometryPath != nullptr)
    {
        std::string geometryCode;
        std::ifstream gShaderFile;

        gShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        try
        {
            // open files
            gShaderFile.open( geometryPath );
            std::stringstream gShaderStream;
            // read file's buffer contents into streams
            gShaderStream << gShaderFile.rdbuf();
            // close file handlers
            gShaderFile.close();
            // convert stream into string
            geometryCode = gShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* gShaderCode = geometryCode.c_str();

        // geometry Shader
        geometry = glCreateShader( GL_GEOMETRY_SHADER );
        glShaderSource( geometry, 1, &gShaderCode, NULL );
        glCompileShader( geometry );
        checkCompileErrors( geometry, "GEOMETRY" );
    }

   
    // shader Program
    this->ID = glCreateProgram();
    if (vertexPath != nullptr) {
        glAttachShader( this->ID, vertex );
        glDeleteShader( vertex );
    }
    if (fragmentPath != nullptr) {
        glAttachShader( this->ID, fragment );
        glDeleteShader( fragment );
    }
    if (geometryPath != nullptr) {
        glAttachShader( this->ID, geometry );
        glDeleteShader( geometry );
    }
    glLinkProgram( this->ID );
    checkCompileErrors( this->ID, "PROGRAM" );
 }

// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool( const std::string& name, bool value ) const
{
    glUniform1i( glGetUniformLocation( this->ID, name.c_str() ), (int)value );
}

void Shader::SetFloat( const char* name, float value, bool useShader )
{
    if (useShader)
        this->use();
    glUniform1f( glGetUniformLocation( this->ID, name ), value );
}

void Shader::SetInteger( const char* name, int value, bool useShader )
{
    if (useShader)
        this->use();
    glUniform1i( glGetUniformLocation( this->ID, name ), value );
}

void Shader::SetVector2f( const char* name, float x, float y, bool useShader )
{
    if (useShader)
        this->use();
    glUniform2f( glGetUniformLocation( this->ID, name ), x, y );
}

void Shader::SetVector2f( const char* name, const glm::fvec2& value, bool useShader )
{
    if (useShader)
        this->use();
    glUniform2f( glGetUniformLocation( this->ID, name ), value[0], value[1] );
}

void Shader::SetVector3f( const char* name, float x, float y, float z, bool useShader )
{
    if (useShader)
        this->use();
    glUniform3f( glGetUniformLocation( this->ID, name ), x, y, z );
}


void Shader::SetVector3f( const char* name, const glm::vec3& value, bool useShader )
{
    if (useShader)
        this->use();
    glUniform3f( glGetUniformLocation( this->ID, name ), value.x, value.y, value.z );
}

void Shader::SetVector4f( const char* name, float x, float y, float z, float w, bool useShader )
{
    if (useShader)
        this->use();
    glUniform4f( glGetUniformLocation( this->ID, name ), x, y, z, w );
}

void Shader::SetVector4f( const char* name, const glm::vec4& value, bool useShader )
{
    if (useShader)
        this->use();
    glUniform4f( glGetUniformLocation( this->ID, name ), value.x, value.y, value.z, value.w );
}

void Shader::SetMatrix4( const char* name, const glm::mat4& matrix, bool useShader )
{
    if (useShader)
        this->use();
    glUniformMatrix4fv( glGetUniformLocation( this->ID, name ), 1, false, glm::value_ptr( matrix ) );
}

void Shader::checkCompileErrors( unsigned int object, std::string type )
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv( object, GL_COMPILE_STATUS, &success );
        if (!success)
        {
            glGetShaderInfoLog( object, 1024, NULL, infoLog );
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv( object, GL_LINK_STATUS, &success );
        if (!success)
        {
            glGetProgramInfoLog( object, 1024, NULL, infoLog );
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}
