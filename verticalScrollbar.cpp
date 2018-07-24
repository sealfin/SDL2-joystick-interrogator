#include "LD20_VerticalScrollbar.h"
#include "seal_quadColours_triangles.h"
#include <stdio.h>

size_t LD20_VerticalScrollbar::mg_numberOfInstances = 0;
seal_Texture *LD20_VerticalScrollbar::mg_backgroundTexture = NULL;

LD20_VerticalScrollbar::LD20_VerticalScrollbar( /* The following parameters are all of- or in- the parent dialog. */ const double p_topLeftX, const double p_topLeftY, double *p_width, double *p_height, double *p_heightOfContent, double *p_distanceContentScrolled, GLfloat *p_opacity )
{
  if( mg_numberOfInstances ++ == 0 )
  {
    mg_backgroundTexture = new seal_Texture( "scrollbar_background-vertical.png", 20, 1 );

    seal_Texture *texture = new seal_Texture( "scrollbar_button-vertical.png", 20, 20 );

    seal_Sprite::p_NewArchetype(( char* )"vertical scrollbar button", texture, true, k_seal_SPRITE_ANIMATION_BEHAVIOUR__HALTS, 0, 2, 0, 1, 1, 1 );

    texture = new seal_Texture( "scrollbar_tab-vertical.png", 20, 20 );
    seal_Sprite::p_NewArchetype(( char* )"vertical scrollbar tab", texture, true, k_seal_SPRITE_ANIMATION_BEHAVIOUR__HALTS, 0, 1, 0, 1 );
  }
  m_scrollUpButton = new LD20_Sprite(( char* )"vertical scrollbar button" );
  m_scrollUpButton->p_SetOpacityEnabled( true );

  m_scrollDownButton = new LD20_Sprite(( char* )"vertical scrollbar button" );
  m_scrollDownButton->p_SetOpacityEnabled( true );
  m_scrollDownButton->p_VFlip();

  m_scrollButtonPressed = k_LD20_VerticalScrollButtonPressed_None;

  m_tab = new seal_Sprite(( char* )"vertical scrollbar tab" );
  m_tab->p_SetOpacityEnabled( true );

  m_topLeftX = p_topLeftX;
  m_topLeftY = p_topLeftY;

  #define M_Assign( p )                                                                                                          \
    if( p_ ## p == NULL )                                                                                                        \
    {                                                                                                                            \
      fprintf( stderr, "LD20_VerticalScrollbar::<constructor> - p_ " # p " == NULL.\n" );                                        \
      throw "LD20_VerticalScrollbar::<constructor> - an error occurred; a description of the error has been printed to stderr."; \
    }                                                                                                                            \
    m_ ## p = p_ ## p;

  M_Assign( width )
  M_Assign( height )
  M_Assign( heightOfContent )
  M_Assign( distanceContentScrolled )
  M_Assign( opacity )

  #undef M_Assign

  p_UpdateSizesAndPositions();
}

LD20_VerticalScrollbar::~LD20_VerticalScrollbar( void )
{
  delete m_tab;
  delete m_scrollDownButton;
  delete m_scrollUpButton;
  if( -- mg_numberOfInstances == 0 )
  {
    seal_Sprite::p_DeleteArchetype(( char* )"vertical scrollbar tab" );
    seal_Sprite::p_DeleteArchetype(( char* )"vertical scrollbar button" );
    delete mg_backgroundTexture;
  }
}

void LD20_VerticalScrollbar::p_UpdateSizesAndPositions( void )
{
  m_backgroundGeometry.p_Set( m_topLeftX + *m_width - mg_backgroundTexture->f_SubTextureWidth(),
                              m_topLeftY - *m_height + m_scrollDownButton->f_HeightInPixels(),
                              mg_backgroundTexture->f_SubTextureWidth(),
                              *m_height - ( m_scrollUpButton->f_HeightInPixels() + m_scrollDownButton->f_HeightInPixels()));

  m_scrollUpButton->p_SetBottomLeftX( m_topLeftX + *m_width - m_scrollUpButton->f_WidthInPixels());
  m_scrollUpButton->p_SetBottomLeftY( m_topLeftY - m_scrollUpButton->f_HeightInPixels());

  m_scrollDownButton->p_SetBottomLeftX( m_topLeftX + *m_width - m_scrollDownButton->f_WidthInPixels());
  m_scrollDownButton->p_SetBottomLeftY( m_topLeftY - *m_height );

  const double d = *m_heightOfContent - *m_height;
  const double step = ( *m_height - ( m_scrollUpButton->f_HeightInPixels() + m_scrollDownButton->f_HeightInPixels()) - m_tab->f_HeightInPixels()) / d;
  m_tab->p_SetBottomLeftX( m_topLeftX + *m_width - m_tab->f_WidthInPixels());
  m_tab->p_SetBottomLeftY( m_topLeftY - m_scrollUpButton->f_HeightInPixels() - *m_distanceContentScrolled * step - m_tab->f_HeightInPixels());
}

void LD20_VerticalScrollbar::p_UpdateButtons( void )
{
  m_scrollUpButton->p_SetFrame( 0 );
  m_scrollDownButton->p_SetFrame( 0 );
  m_scrollButtonPressed = k_LD20_VerticalScrollButtonPressed_None;
  if( LD20_Cursor::f_Down())
    if( f_ButtonPressed( m_scrollUpButton->f_CollisionDetectionAABB()))
    {
      m_scrollUpButton->p_SetFrame( 1 );
      m_scrollButtonPressed = k_LD20_VerticalScrollButtonPressed_Up;
    }
    else
      if( f_ButtonPressed( m_scrollDownButton->f_CollisionDetectionAABB()))
      {
        m_scrollDownButton->p_SetFrame( 1 );
        m_scrollButtonPressed = k_LD20_VerticalScrollButtonPressed_Down;
      }
}

void LD20_VerticalScrollbar::p_Render( void )
{
  glVertexPointer( 2, GL_FLOAT, 0, m_backgroundGeometry.m_coordinates );
  glEnableClientState( GL_COLOR_ARRAY );

  seal_QuadColours_Triangles colour( 1., 1., 1., *m_opacity );

  glColorPointer( 4, GL_FLOAT, 0, colour.m_rgba );

  seal_Quad_Triangles texture_coordinates;

  mg_backgroundTexture->p_GetSubTextureQuad( 0, &texture_coordinates );
  glTexCoordPointer( 2, GL_FLOAT, 0, texture_coordinates.m_coordinates );
  mg_backgroundTexture->p_Bind();
  glDrawArrays( GL_TRIANGLES, 0, 6 );
  glDisableClientState( GL_COLOR_ARRAY );

  m_scrollUpButton->p_SetOpacity( *m_opacity );
  m_scrollUpButton->p_Render();

  m_scrollDownButton->p_SetOpacity( *m_opacity );
  m_scrollDownButton->p_Render();

  m_tab->p_SetOpacity( *m_opacity );
  m_tab->p_Render();
}
