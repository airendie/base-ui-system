#ifndef __TextObject_h_
#define __TextObject_h_

#include <string>
#include <map>

#include "Shape.h"
#include "Shader.h"
#include "Character.h"
#include "constants.h"

#include <glm/glm.hpp>

#include "TextRenderer.h"

enum TextAlignment
{
    TopLeft,
    MidLeft,
    ButtomLeft,
    TopMiddle,
    MidMiddle,
    ButtomMiddle,
    TopRight,
    MidRight,
    ButtomRight
};

/*
// A renderer class for rendering text displayed by a font loaded using the
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer
{
public:
    // holds a list of pre-compiled Characters
    std::map<char, Character> Characters;
    // shader used for text rendering
    Shader TextShader;
    // constructor
    TextRenderer(unsigned int width, unsigned int height);
    // pre-compiles a list of characters from the given font
    void Load(const std::string & font, unsigned int fontSize);
    // renders a string of text using the precompiled list of characters
    void RenderText(const std::string & text, float x, float y, float scale, Color color = { 1, 1, 1 });
private:
    // render state
    unsigned int VAO, VBO;
};*/

// Порядок выполнения:
// Инициализация словаря Characters (один раз на всю программу!)




// Хранит в себе строку, размер шрифта, цвет, шрифт, координаты левого нижнего угла прямоугольника, в который вписывается эта надпись.
class TextObject : public Shape
{
private:
    glm::fvec2 _pos0;
    std::string _text;
    glm::fvec2 _textPosition;
    glm::fvec3 _textColor;
    glm::fvec2 _textSize;

    std::string _fontPath;
    unsigned int _fontSize;

    TextRenderer _textRenderer;
public:
    // [TODO] screen size temporarly
    TextObject(const std::string & text,
        const glm::fvec2 &pos,
        std::string font = DEFAULT_FONT,
        unsigned int font_size = 24,
        glm::fvec3 textColor = { 1, 1, 1 });

    void init(glm::fvec2 pos0 = { 0,0 });
    void render();

    std::string getText() { return _text; }
};

/*
class TextObject : public UIObject {
private:
    std::string _text;
    Color _textColor;

    std::map<char, Character> _characters;

    Shader TextShader;

    // TODO: alignment WIP

    TextAlignment _alignment;
    Vector2 _textOffset;

    void setValues(const char* text, TextAlignment alignment, const Vector2& extra_offset);

    Vector2 getAlignmentTextOffset(TextAlignment alignment);

    Vector2 getSize(const char* text, TextAlignment alignment, const Vector2& extra_offset);

public:

    TextObject() : UIObject() { setValues(nullptr, MidMiddle, { 0, 0 }); }
    TextObject(const char* text);
    TextObject(const Vector2& pos) : UIObject(pos) { setValues(nullptr, MidMiddle, { 0, 0 }); }
    TextObject(const Vector2& pos, const char* text);
    TextObject(const Vector2& pos, TextAlignment alignment, const char* text);
    TextObject(const Vector2& pos, TextAlignment alignment, const Vector2& extra_offset, const char* text);

    void changeText(const char* new_text) { this->_text = new_text; }

};
*/

#endif // __TextObject_h_