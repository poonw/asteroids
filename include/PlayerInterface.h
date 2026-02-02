#ifndef PLAYERINTERFACE_H
#define PLAYERINTERFACE_H

#include <memory>
#include <vector>
#include "RaylibInterface.h"

class PlayerInterface
{
public:
    PlayerInterface(void) {};
    virtual ~PlayerInterface(void) {};

    virtual void    update(void)                                 = 0;
    virtual void    draw(void)                                   = 0;
    virtual Vector2 getCenter(void)                              = 0;
    virtual float   getRadius(void)                              = 0;
    virtual void    setTextures(std::vector<Texture2D> textures) = 0;
    virtual void    setInvincible(void)                          = 0;
    virtual void    setDispersedlaser(void)                      = 0;

    bool m_discard = false;

protected:
    std::shared_ptr<RaylibInterface> m_raylibPtr = nullptr;
    Vector2                          m_position  = {0, 0};
    Vector2                          m_direction = {0, 0};
    float                            m_speed     = 0;
    float                            m_radius    = 0;
    std::vector<Texture2D>           m_textures;
    bool                             m_invincible     = false;
    bool                             m_dispersedLaser = false;
};

#endif // PLAYERINTERFACE_H
