#include <iostream>

#include "FieldView.h"
#include "constants.h"

FieldView::FieldView() : _field( FIELD_SIZE, std::vector<char>( FIELD_SIZE ) )
{
    this->_fieldTextRenderer = TextRenderer();
}

void FieldView::render()
{
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            char cell = this->_field[i][j];

            if (cell == 0)
                continue;

            if (cell == 1)
                this->_fieldTextRenderer.render( "x", 15.0f * j + 15.0f, 15.0f * i + 15.0f, 1.f, glm::fvec3( 1, 0, 0 ) );
            if (cell == -1)
                this->_fieldTextRenderer.render( "o", 15.0f * j + 15.0f, 15.0f * i + 15.0f, 1.f, glm::fvec3( 0, 0, 1 ) );
        }
    }
}

/// <summary>
/// Set cell value depending on the player's turn in y row, x column of the field.
/// </summary>
/// <param name="turn"> - ONLY LOWERED value: 'x' or 'o' is allowed.</param>
/// <param name="x"> - column.</param>
/// <param name="y"> - row.</param>
void FieldView::setTurn( char turn, unsigned int x, unsigned int y )
{
    char& cell = this->_field[y][x];
    if (cell != 0)
        return;

    switch (turn)
    {
    case 'x':
        cell = 1;
        break;
    case 'o':
        cell = -1;
        break;
    default:
        std::cerr << "ERROR! '" << turn << "' is not allowed!";
        break;
    }
}
