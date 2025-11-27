#ifndef SPRITE_H
#define SPRITE_H

class Sprite
{
public:
    Sprite(void);
    virtual ~Sprite(void);

    virtual void update(void) = 0;
    virtual void draw(void)   = 0;

private:
};

#endif // SPRITE_H
