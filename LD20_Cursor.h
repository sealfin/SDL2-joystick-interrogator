#ifndef LD20_Cursor_h

#define LD20_Cursor_h

#include "seal_collisionDetection.h"
#include <SDL2/SDL.h>

typedef enum
{
  k_LD20_CursorState_Up,
  k_LD20_CursorState_Down,
  k_LD20_CursorState_WasDown
}
t_LD20_CursorState;

class LD20_Cursor
{
  private:
    static t_LD20_CursorState mg_state;
    static t_seal_CollisionDetectionAABB mg_currentCollisionDetectionAABB, mg_downCollisionDetectionAABB, mg_upCollisionDetectionAABB;

  public:

    #define M_UpdateCollisionDetectionAABBOrigin( p )     \
      mg_ ## p ## CollisionDetectionAABB.m_originX = p_x; \
      mg_ ## p ## CollisionDetectionAABB.m_originY = p_y;

    static void p_Move( const Sint32 p_x, const Sint32 p_y )
    {
      M_UpdateCollisionDetectionAABBOrigin( current )
    }

    static void p_Press( const Sint32 p_x, const Sint32 p_y )
    {
      mg_state = k_LD20_CursorState_Down;
      M_UpdateCollisionDetectionAABBOrigin( down )
      M_UpdateCollisionDetectionAABBOrigin( current )
    }

    static void p_Release( const Sint32 p_x, const Sint32 p_y )
    {
      if( mg_state == k_LD20_CursorState_Down )
      {
        mg_state = k_LD20_CursorState_WasDown;
        M_UpdateCollisionDetectionAABBOrigin( up )
        M_UpdateCollisionDetectionAABBOrigin( current )
      }
    }

    #undef M_UpdateCollisionDetectionAABBOrigin

    static bool f_Down( void )
    {
      return mg_state == k_LD20_CursorState_Down;
    }

    static bool f_WasDown( void )
    {
      return mg_state == k_LD20_CursorState_WasDown;
    }

    static void p_SetState_Up_IfState_WasDown( void )
    {
      if( mg_state == k_LD20_CursorState_WasDown )
        mg_state = k_LD20_CursorState_Up;
    }

    static const t_seal_CollisionDetectionAABB * const f_CurrentCollisionDetectionAABB( void )
    {
      return &mg_currentCollisionDetectionAABB;
    }

    static const t_seal_CollisionDetectionAABB * const f_DownCollisionDetectionAABB( void )
    {
      return &mg_downCollisionDetectionAABB;
    }

    static const t_seal_CollisionDetectionAABB * const f_UpCollisionDetectionAABB( void )
    {
      return &mg_upCollisionDetectionAABB;
    }
};

#endif
