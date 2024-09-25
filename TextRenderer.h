/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>

#include "constants.h"
#include "shader.h"
#include "Shape.h"
#include <glm/glm.hpp>
#include "Character.h"

extern std::vector<Font> FontLib;
// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer : public Shape
{
private:
    glm::fvec2 _pos0 = { 0, 0 };
    std::string _text;

    // Пытался сделать через ссылку. 
    // После часа танца с бубнами над исключением map'а, плюнул и сделал вот так.
    size_t fontIndex = 0;

public:
    // constructor
    void init(glm::fvec2 pos0);
    // pre-compiles a list of characters from the given font
    void loadFont(const std::string& font, unsigned int fontSize);
    // renders a string of text using the precompiled list of characters
    void render(const std::string& text, float x, float y, float scale, glm::fvec3 color);
    glm::fvec2 getBoxSize();
    ~TextRenderer()
    {
        glDeleteVertexArrays( 1, &_VAO );
        glDeleteBuffers( 1, &_VBO );
    }
};

///
/// 1. load Font
/// 2. use Font

#endif
