#include <iostream> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "TextObject.h"


TextObject::TextObject(const std::string& text, const glm::fvec2& pos, std::string fontPath, unsigned int fontSize, glm::fvec3 textColor)
{
    this->_pos0 = { 0,0 };
    this->_text = text;
    this->_textPosition = pos;
    this->_fontPath = fontPath;
    this->_fontSize = fontSize;
    this->_textColor = textColor;

    this->_textSize = { 0, 0 }; // after initialization
}

void TextObject::init(glm::fvec2 pos0)
{
    this->_pos0 = pos0;
    _textRenderer.loadFont(this->_fontPath, this->_fontSize);
    _textRenderer.init(this->_pos0);

    // Определенеие размера выводимого текста возможно только после загрузки глифов
    // соответствующего шрифта соответствующего размера.
    this->_textSize = this->_textRenderer.getBoxSize();
}

void TextObject::render()
{
    _textRenderer.render(this->_text, this->_textPosition.x, this->_textPosition.y, 1.f, this->_textColor);
}