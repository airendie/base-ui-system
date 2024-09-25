#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#include "Shape.h"
#include "constants.h"

//enum TextAlignment
//{
//    TopLeft,
//    MidLeft,
//    ButtomLeft,
//    TopMiddle,
//    MidMiddle,
//    ButtomMiddle,
//    TopRight,
//    MidRight,
//    ButtomRight
//};

class Box : public Shape
{
private:
    glm::fvec2 pos0;

    glm::fvec2 v0, v1;

    // background color
    glm::fvec3 bg_color;
public:
    Box();

    /// <param name="top_left_pos">Координаты левого верхнего угла</param>
    /// <param name="size">Вектор размеров (длина X, длина Y)</param>    
    /// <param name="ol_color">Цвет контура фигуры</param>
    /// <param name="bg_color">Цвет наполнения фигуры</param>                                       
    Box( const glm::fvec2& v0, 
         const glm::fvec2& v1,
         float r, float g, float b );

    /// <param name="x0">X первой вершины </param>
    /// <param name="y0">Y первой вершины </param>
    /// <param name="x1">X второй вершины </param>       
    /// <param name="y0">Y второй вершины </param>         
    /// <param name="ol_color">Цвет контура фигуры</param>
    /// <param name="bg_color">Цвет наполнения фигуры</param>
    Box( float x0, float y0,
         float x1, float y1,
         float r, float g, float b );

    /// <param name="top_left_pos">Координаты левого верхнего угла</param>
    /// <param name="width">Длина горизонтальной стороны</param>
    /// <param name="height">Длина вертикальной высоты</param>
    /// <param name="ol_color">Цвет контура фигуры</param>
    /// <param name="bg_color">Цвет наполнения фигуры</param>                                          
    Box( glm::fvec2 top_left_pos,
         float width, float height,
         float r, float g, float b );

    ~Box() { glDeleteVertexArrays( 1, &_VAO ); }

    void init( const glm::fvec2& pos0 );
    void render();
    glm::fvec2 getBoxSize();
};

class BorderedBox //: public Shape
{
private:
    Box contentBox;
    Box borderBox;

public:
    void init( glm::fvec2 pos0 );
    void render();
    glm::fvec2 getBoxSize();
};
