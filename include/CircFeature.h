#ifndef CIRCFEATURE_H
#define CIRCFEATURE_H

#include "RaylibInterface.h"

class CircFeature
{
public:
    virtual ~CircFeature(void) = default;

    virtual Vector2 getCenter(void) = 0;
    virtual float   getRadius(void) = 0;

protected:
    float m_radius = 0;
};

#endif // CIRCFEATURE_H
