#ifndef LD20_Sprite_h

#define LD20_Sprite_h

#include "seal_sprite.h"
#include "seal_collisionDetection.h"

class LD20_Sprite : public seal_Sprite
{
  private:
    t_seal_CollisionDetectionAABB m_collisionDetectionAABB;

  public:
    LD20_Sprite( char *p ) : seal_Sprite( p )
    {
      m_collisionDetectionAABB.m_halfWidth = f_WidthInPixels() / 2.;
      m_collisionDetectionAABB.m_originX = f_GetBottomLeftX() + m_collisionDetectionAABB.m_halfWidth;
      m_collisionDetectionAABB.m_halfHeight = f_HeightInPixels() / 2.;
      m_collisionDetectionAABB.m_originY = f_GetBottomLeftY() + m_collisionDetectionAABB.m_halfHeight;
    }

    void p_SetBottomLeftX( const double p )
    {
      seal_Sprite::p_SetBottomLeftX( p );
      m_collisionDetectionAABB.m_originX = p + m_collisionDetectionAABB.m_halfWidth;
    }

    void p_SetBottomLeftY( const double p )
    {
      seal_Sprite::p_SetBottomLeftY( p );
      m_collisionDetectionAABB.m_originY = p + m_collisionDetectionAABB.m_halfHeight;
    }

    const t_seal_CollisionDetectionAABB * const f_CollisionDetectionAABB( void )
    {
      return &m_collisionDetectionAABB;
    }
};

#endif
