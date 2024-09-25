#include "Character.h"


void Font::clear()
{
    this->path = "";
    this->size = 0;
    this->Characters.clear();
}

Font::Font()
{
    this->path = "";
    this->size = 0;
    this->Characters.clear();
}

Font::Font(std::string new_path, unsigned new_size, std::map<char, Character> new_Characters)
{
    this->clear();
    
    this->path = new_path;
    this->size = new_size;
    this->Characters = new_Characters;
}

Font::Font(const Font &other)
{
    this->clear();

    this->path = other.path;
    this->size = other.size;
    this->Characters = other.Characters;
}

Font &Font::operator=(Font &rhs)
{
    this->clear();

    this->path = rhs.path;
    this->size = rhs.size;
    this->Characters = rhs.Characters;

    return *this;
}

void swap(Font &source, Font &other)
{
    Font *t = &source;
    source = other;
    other = *t;
}