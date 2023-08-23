#include "Mouse.h"
#include <iostream>

constexpr auto MARGIN = 20;

Mouse::Mouse()
{
}

Mouse::Mouse(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
}

void Mouse::setMousePosition(int currentX, int currentY) {
	differenceMousePosition.x = currentX - prevMousePosition.x;
	differenceMousePosition.y = currentY - prevMousePosition.y;

	prevMousePosition.x = currentX;
	prevMousePosition.y = currentY;
    
    if (differenceMousePosition.x == 0) {
        if (currentX <= MARGIN) {
            mouseEdgeflags["leftEdge"] = true;
        }
        else if (currentX >= (windowWidth - MARGIN)) {
            mouseEdgeflags["rightEdge"] = true;
        }
    }
    else {
        mouseEdgeflags["leftEdge"] = false;
        mouseEdgeflags["rightEdge"] = false;
    }

    if (differenceMousePosition.y == 0) {
        if (currentY <= MARGIN) {
            mouseEdgeflags["upperEdge"] = true;
        }
        else if (currentY >= (windowHeight - MARGIN)) {
            mouseEdgeflags["bottomEdge"] = true;
        }
    }
    else {
        mouseEdgeflags["upperEdge"] = false;
        mouseEdgeflags["bottomEdge"] = false;
    }
}
