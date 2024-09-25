#ifndef __Shape_h_
#define __Shape_h_


#include "Shader.h"

// NOTE: just a placeholder before OpenGL implementation

class Shape {
protected:
    Shader _shader;

    // size.x = width and size.y = height
    glm::fvec2 size;

    unsigned int _VAO = 0, _VBO = 0, _EBO = 0;
public:
    virtual ~Shape() {  }

    virtual void setPosition( glm::fvec2 new_pos ) {}

    virtual void init() {}
    virtual void draw() {};
    virtual glm::fvec2 getBoxSize() { return { 0, 0 }; };
};

#endif //__Shape_h_
