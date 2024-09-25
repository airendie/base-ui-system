#include "TextBox.h"

#include "constants.h"
#include <glm/glm.hpp>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Box::Box()
{
    this->pos0 = { 0.f, 0.f };
    this->v0 = { 0.f, 0.f };
    this->v1 = { 0.f, 0.f };
    this->size = { 0.f, 0.f };
    this->bg_color = { 1.f, 1.f, 1.f };

    this->_VBO = 0;
    this->_EBO = 0;
}

Box::Box( const glm::fvec2& v0,
          const glm::fvec2& v1,
          float r, float g, float b)
{
    this->pos0 = { 0, 0 };
    this->v0 = v0;
    this->v1 = v1;
    this->size = glm::fvec2( fabsf( v1.x - v0.x ), fabsf( v1.y - v0.y ) );
    this->bg_color = glm::fvec4(r, g, b, 1.f);

    this->_VBO = 0;
    this->_EBO = 0;
}

Box::Box( float x0, float y0,
          float x1, float y1,
          float r, float g, float b )
{
    this->pos0 = { 0, 0 };

    this->v0 = glm::fvec2(x0, y0);
    this->v1 = glm::fvec2(x1, y1);
    
    size = glm::fvec2( fabsf(x1 - x0), fabsf( y1 - y0 ) );

    this->bg_color = glm::fvec4( r, g, b, 1.f );

    this->_VBO = 0;
    this->_EBO = 0;
}

Box::Box( glm::fvec2 v0,
          float width, 
          float height,
          float r, float g, float b )
{
    this->pos0 = { 0, 0 };

    this->v0 = v0;
    this->v1 = glm::fvec2(v0.x + width, v0.y + height);
    size = { width, height };

    this->bg_color = glm::fvec4( r, g, b, 1.f );
    
    this->_VBO = 0;
    this->_EBO = 0;
}

void Box::init(const glm::fvec2 &pos0 )
{
    this->pos0 = pos0;

    this->_shader.Load( "Shaders/box_vs.glsl", "Shaders/box_fs.glsl", nullptr );

}

void Box::render()
{
    float width = this->size.x / static_cast<float>(WINDOW_WIDTH);
    float height = this->size.y / static_cast<float>(WINDOW_HEIGHT);

    glm::fvec2 pos = { (this->v0.x + this->v1.x) / 2.f, (this->v0.y + this->v1.y) / 2.f };

    pos.x /= static_cast<float>(WINDOW_WIDTH);
    pos.y /= static_cast<float>(WINDOW_HEIGHT);

    float vertices[] = {
         width / 2 + pos[0],  height / 2 + pos[1], bg_color[0], bg_color[1], bg_color[2], bg_color[3],
         width / 2 + pos[0], -height / 2 + pos[1], bg_color[0], bg_color[1], bg_color[2], bg_color[3],
        -width / 2 + pos[0], -height / 2 + pos[1], bg_color[0], bg_color[1], bg_color[2], bg_color[3],
        -width / 2 + pos[0],  height / 2 + pos[1], bg_color[0], bg_color[1], bg_color[2], bg_color[3]
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &this->_VAO);
    glGenBuffers(1, &this->_VBO);
    glGenBuffers(1, &this->_EBO);
    glBindVertexArray(this->_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    this->_shader.SetVector4f( "ourColor", bg_color.r, bg_color.g, bg_color.b, 1.0f, false );
    
    this->_shader.use();
    
    glBindVertexArray( this->_VAO );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

    this->_shader.reset();
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

glm::fvec2 Box::getBoxSize()
{
    return size;
}