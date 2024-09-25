#pragma once

#include <vector>

#include "TextRenderer.h"

class FieldView : Shape {
private:
	TextRenderer _fieldTextRenderer;
	std::vector<std::vector<char>> _field;
public:
	FieldView();

	void init()
	{
		_fieldTextRenderer.loadFont(DEFAULT_FONT, 24);
		_fieldTextRenderer.init({ 0, 0 });
	}
	void render();

	void setTurn(char turn, unsigned int x, unsigned int y);
};
