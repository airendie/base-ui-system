/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "TextRenderer.h"
std::vector<Font> FontLib;

glm::fvec2 TextRenderer::getBoxSize()
{
    float textW = 0, textH = 0;

    for (auto c = this->_text.begin(); c != this->_text.end(); c++) {
        Character ch = FontLib[this->fontIndex].Characters[*c];
        int ch_advance = ch.advance;

        textW += ch.size.x + ch_advance;
        textH = std::max(textH, (float)ch.size.y);
    }

    return glm::fvec2(textW, textH);
}

void TextRenderer::init(glm::fvec2 pos0)
{
    this->_pos0 = pos0;

    this->_shader.Load("Shaders/text.vs", "Shaders/text.fs", nullptr);
    this->_shader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT), 0.0f), true);
    this->_shader.SetInteger("text", 0, true);
}

void createFont(const std::string& fontPath, unsigned int fontSize)
{
    Font newFont;
    // Free Type 
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }
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
            static_cast<unsigned>(face->glyph->advance.x)
        };
        newFont.Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    newFont.path = fontPath;
    newFont.size = fontSize;
    FontLib.push_back(newFont);
}

void TextRenderer::loadFont(const std::string& fontPath, unsigned int fontSize)
{
    for (size_t i = 0; i < FontLib.size(); i++)
    {
        if ((FontLib[i].path == fontPath) && (FontLib[i].size == fontSize))
        {
            this->fontIndex = i;
            return;
        }
    }

    createFont(fontPath, fontSize);
    this->fontIndex = FontLib.size() - 1;
}

void TextRenderer::render(const std::string& text, float x, float y, float scale, glm::fvec3 color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // configure _VAO/_VBO for texture quads
    glGenVertexArrays(1, &this->_VAO);
    glGenBuffers(1, &this->_VBO);
    glBindVertexArray(this->_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // activate corresponding render state	
    this->_shader.use();
    this->_shader.SetVector3f("textColor", color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->_VAO);

    float global_x = this->_pos0.x + x, global_y = this->_pos0.y + y;
    // iterate through all characters
    for (auto c = text.begin(); c != text.end(); c++)
    {
        const Character* ch = &FontLib[this->fontIndex].Characters[*c];

        float xpos = global_x + ch->bearing.x * scale;
        float ypos = global_y + (FontLib[this->fontIndex].Characters['H'].bearing.y - ch->bearing.y) * scale;

        float w = ch->size.x * scale;
        float h = ch->size.y * scale;
        // update _VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos,     ypos,       0.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch->ID);
        // update content of _VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData!
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph
        global_x += (ch->advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    this->_shader.reset();

    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}
