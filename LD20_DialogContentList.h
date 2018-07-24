#ifndef LD20_DialogContentList_h

#define LD20_DialogContentList_h

#include "LD20_DialogContent.h"
#include "seal_list.h"

class LD20_DialogContentList : public seal_List<LD20_DialogContent*>
{
  private:
    double m_topLeftX, m_topLeftY;
    bool m_blendingEnabled;
    GLfloat m_opacity;

  public:
    LD20_DialogContentList( const double p_topLeftX, const double p_topLeftY )
    {
      m_topLeftX = p_topLeftX;
      m_topLeftY = p_topLeftY;
      m_blendingEnabled = false;
      m_opacity = 1.;
    }

    void p_Set( LD20_DialogContent *p )
    {
      p->p_SetBottomLeftX( m_topLeftX + p->f_GetBottomLeftX());
      p->p_SetBottomLeftY( m_topLeftY - p->f_Top());
      p->p_SetBlendingEnabled( m_blendingEnabled );
      p->p_SetOpacity( m_opacity );
      seal_List<LD20_DialogContent*>::p_Set( p );
    }

    void p_TidyUp( void )
    {
      size_t i = 0;
      double y = m_topLeftY;

      for( ; i < f_Number(); i ++ )
      {
        LD20_DialogContent *c = f_GetByIndex( i );

        c->p_SetBottomLeftX( m_topLeftX );
        y -= c->f_HeightInPixels();
        c->p_SetBottomLeftY( y );
      }
    }

    void p_Render( void )
    {
      size_t i = 0;

      for( ; i < f_Number(); i ++ )
        f_GetByIndex( i )->p_Render();
    }

    double f_Width( void )
    {
      size_t i = 0;
      double greatest_width = 0.;

      for( ; i < f_Number(); i ++ )
      {
        const double width = f_GetByIndex( i )->f_Right() - m_topLeftX;

        if( width > greatest_width )
          greatest_width = width;
      }
      return greatest_width;
    }

    double f_Height( void )
    {
      size_t i = 0;
      double greatest_height = 0.;

      for( ; i < f_Number(); i ++ )
      {
        const double height = m_topLeftY - f_GetByIndex( i )->f_Bottom();

        if( height > greatest_height )
          greatest_height = height;
      }
      return greatest_height;
    }

    LD20_DialogContent *f_GetCollidedWith( const t_seal_CollisionDetectionAABB * const p )
    {
      size_t i = 0;

      for( ; i < f_Number(); i ++ )
      {
        LD20_DialogContent *c = f_GetByIndex( i );

        if( f_seal_AABBCollision( p, c->f_CollisionDetectionAABB()))
          return c;
      }
      return NULL;
    }

    bool f_GetBlendingEnabled( void )
    {
      return m_blendingEnabled;
    }

    void p_SetBlendingEnabled( const bool p )
    {
      m_blendingEnabled = p;

      size_t i = 0;

      for( ; i < f_Number(); i ++ )
        f_GetByIndex( i )->p_SetBlendingEnabled( m_blendingEnabled );
    }

    GLfloat f_GetOpacity( void )
    {
      return m_opacity;
    }

    void p_SetOpacity( const GLfloat p )
    {
      m_opacity = p;

      size_t i = 0;

      for( ; i < f_Number(); i ++ )
        f_GetByIndex( i )->p_SetOpacity( m_opacity );
    }
};

#endif
