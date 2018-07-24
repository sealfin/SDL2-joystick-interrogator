#ifndef LD20_Dialog_h

#define LD20_Dialog_h

#include "LD20_DialogContentList.h"
#include "LD20_VerticalScrollbar.h"

typedef enum
{
  k_LD20_DialogVisibility_Invisible,
  k_LD20_DialogVisibility_FadingIn,
  k_LD20_DialogVisibility_Opaque,
  k_LD20_DialogVisibility_FadingOut
}
t_LD20_DialogVisibility;

class LD20_Dialog
{
  private:
    static size_t mg_numberOfInstances;

    t_LD20_DialogVisibility m_visibility;
    GLfloat m_opacity;
    double m_topLeftX, m_topLeftY;
    size_t m_maximumWidth, m_maximumHeight;
    double m_width, m_height;

    seal_Quad_Triangles m_bodyGeometry;

    static seal_Texture *mg_cornerTexture;
    seal_Quad_Triangles m_topLeftCornerGeometry, m_topRightCornerGeometry, m_bottomRightCornerGeometry, m_bottomLeftCornerGeometry;

    static seal_Texture *mg_horizontalEdgeTexture;
    seal_Quad_Triangles m_topEdgeGeometry, m_bottomEdgeGeometry;

    static seal_Texture *mg_verticalEdgeTexture;
    seal_Quad_Triangles m_leftEdgeGeometry, m_rightEdgeGeometry;

    bool m_buttonsMayBePressed;

    LD20_Sprite *m_closeButton;
    bool m_closeButtonPressed;

    LD20_DialogContentList *m_content;
    double m_heightOfContent, m_contentScrolled;

    LD20_VerticalScrollbar *m_verticalScrollbar;
    static size_t mg_verticalScrollbarWidth;

  public:
    static size_t f_CornerWidthInPixels( void )
    {
      return 12;
    }

    static size_t f_CornerHeightInPixels( void )
    {
      return 12;
    }

    LD20_Dialog( const double p_topLeftX, const double p_topLeftY, const size_t p_maximumWidth, const size_t p_maximumHeight, const bool p_hasCloseButton );
    ~LD20_Dialog( void );

    size_t f_NumberOfItemsOfContent( void )
    {
      return m_content->f_Number();
    }

    void p_SetContent( LD20_DialogContent *p )
    {
      m_content->p_Set( p );
      m_heightOfContent = m_content->f_Height();
      m_contentScrolled = 0.;
    }

    void p_TidyUpContent( void )
    {
      m_content->p_TidyUp();
      m_heightOfContent = m_content->f_Height();
      m_contentScrolled = 0.;
    }

    void p_ResizeToFitContent( void );

    void p_RemoveContent( LD20_DialogContent *p )
    {
      m_content->p_Remove( p );
      m_heightOfContent = m_content->f_Height();
      m_contentScrolled = 0.;
    }

    void p_RemoveAllContent( void )
    {
      m_content->p_Empty();
      m_heightOfContent = 0.;
      m_contentScrolled = 0.;
    }

    LD20_DialogContent *f_ContentPressed( void )
    {
      if( m_buttonsMayBePressed )
        if( LD20_Cursor::f_WasDown())
        {
          const t_seal_CollisionDetectionAABB dialog = { m_topLeftX + m_width / 2., m_topLeftY - m_height / 2., m_width / 2., m_height / 2. };

          /* Test if both the mouse button press & release were in the dialog. */
          if( f_seal_AABBCollision( LD20_Cursor::f_DownCollisionDetectionAABB(), &dialog ) && f_seal_AABBCollision( LD20_Cursor::f_UpCollisionDetectionAABB(), &dialog ))
          {
            LD20_DialogContent *down = m_content->f_GetCollidedWith( LD20_Cursor::f_DownCollisionDetectionAABB());
            LD20_DialogContent *up = m_content->f_GetCollidedWith( LD20_Cursor::f_UpCollisionDetectionAABB());

            /* Test if both the mouse button press & release were on the same item of content. */
            if( down == up )
              return down;
          }
        }
      return NULL;
    }

  private:
    void p_UpdateSize( double * const p_size, const size_t p_maximumSize, const double p_contentSize, const Uint32 p_ticks );
    void p_UpdateSizesAndPositions( void );
    double f_ClampScrolling( const double p );

    bool f_ButtonPressed( const t_seal_CollisionDetectionAABB * const p )
    {
      return f_seal_AABBCollision( LD20_Cursor::f_DownCollisionDetectionAABB(), m_closeButton->f_CollisionDetectionAABB()) && f_seal_AABBCollision( p, m_closeButton->f_CollisionDetectionAABB());
    }

  public:
    void p_Update( const Uint32 p );

    bool f_CloseButtonPressed( void )
    {
      const bool b = m_closeButtonPressed;

      m_closeButtonPressed = false;
      return b;
    }

    void p_Render( void );

    bool f_IsInvisible( void )
    {
      return m_visibility == k_LD20_DialogVisibility_Invisible;
    }

    bool f_IsOpaque( void )
    {
      return m_visibility == k_LD20_DialogVisibility_Opaque;
    }

    void p_MakeInvisible( void )
    {
      m_visibility = k_LD20_DialogVisibility_Invisible;
      m_opacity = 0.;
    }

    void p_MakeOpaque( void )
    {
      m_visibility = k_LD20_DialogVisibility_Opaque;
      m_opacity = 1.;
    }

    void p_FadeIn( void )
    {
      if( m_visibility != k_LD20_DialogVisibility_Opaque )
        m_visibility = k_LD20_DialogVisibility_FadingIn;
    }

    void p_FadeOut( void )
    {
      if( m_visibility != k_LD20_DialogVisibility_Invisible )
        m_visibility = k_LD20_DialogVisibility_FadingOut;
    }
};

#endif
