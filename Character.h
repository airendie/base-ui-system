#ifndef __Character_h
#define __Character_h

#include <map>
#include <vector>
#include <string>

#include "Shape.h"
/// Holds all state information relevant to a character as loaded using FreeType
struct Character
{
    unsigned int ID; // ID handle of the glyph texture
    glm::ivec2   size;      // size of glyph
    glm::ivec2   bearing;   // offset from baseline to left/top of glyph
    unsigned int advance;   // horizontal offset to advance to next glyph
};

struct Font
{
    std::string path;
    unsigned size = 0;

    std::map<char, Character> Characters;

    explicit Font();
    Font(std::string new_path, unsigned new_size, std::map<char, Character> new_Characters);
    Font(const Font &other);

    Font &operator=(Font &rhs);

    void clear();
};


#endif
