#ifndef RECTFEATURE_H
#define RECTFEATURE_H

#include "RaylibInterface.h"

class RectFeature
{
public:
    virtual ~RectFeature(void) = default;

    virtual Rectangle getRect(void) const = 0;

protected:
};

#endif // RECTFEATURE_H
