#ifndef LD20_VerticalScrollbar_h

#define LD20_VerticalScrollbar_h

#include "LD20_Sprite.h"
#include "LD20_Cursor.h"

typedef enum
{
  k_LD20_VerticalScrollButtonPressed_Down = -1,
  k_LD20_VerticalScrollButtonPressed_None,
  k_LD20_VerticalScrollButtonPressed_Up
}
t_LD20_VerticalScrollButtonPressed;

class LD20_VerticalScrollbar
{
  private:
    static size_t mg_numberOfInstances;
    static seal_Texture *mg_backgroundTexture;

    seal_Quad_Triangles m_backgroundGeometry;
    LD20_Sprite *m_scrollUpButton, *m_scrollDownButton;
    t_LD20_VerticalScrollButtonPressed m_scrollButtonPressed;
    seal_Sprite *m_tab;
    // The following instance variables are all of- or in- the parent dialog.
    double m_topLeftX, m_topLeftY, *m_width, *m_height, *m_heightOfContent, *m_distanceContentScrolled;
    GLfloat *m_opacity;

  public:
    LD20_VerticalScrollbar( /* The following parameters are all of- or in- the parent dialog. */ const double p_topLeftX, const double p_topLeftY, double *p_width, double *p_height, double *p_heightOfContent, double *p_distanceContentScrolled, GLfloat *p_opacity );
    ~LD20_VerticalScrollbar( void );
    void p_UpdateSizesAndPositions( void );

  private:
    bool f_ButtonPressed( const t_seal_CollisionDetectionAABB * const p )
    {
      return f_seal_AABBCollision( LD20_Cursor::f_DownCollisionDetectionAABB(), p ) && f_seal_AABBCollision( LD20_Cursor::f_CurrentCollisionDetectionAABB(), p );
    }

  public:
    void p_UpdateButtons( void );

    t_LD20_VerticalScrollButtonPressed f_ButtonPressed( void )
    {
      return m_scrollButtonPressed;
    }

    void p_Render( void );
};

#endif
