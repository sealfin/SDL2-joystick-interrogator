#include "LD20_Dialog.h"
#include "seal_quadColours_triangles.h"

static const size_t k_LD20_PixelsBetweenDialogCloseButtonAndContent = 2;
static const size_t k_LD20_PixelsBetweenDialogVerticalScrollbarAndContent = 1;
static const double k_LD20_DialogSizeChangePerTick = 19. / 1000.;
static const GLfloat k_LD20_DialogOpacityChangePerTick = 1. / 333.;
static const double k_LD20_PixelsScrolledPerTickByContentOfDialog = 95. / 1000.;

size_t LD20_Dialog::mg_numberOfInstances = 0;
seal_Texture *LD20_Dialog::mg_cornerTexture = NULL;
seal_Texture *LD20_Dialog::mg_horizontalEdgeTexture = NULL;
seal_Texture *LD20_Dialog::mg_verticalEdgeTexture = NULL;
size_t LD20_Dialog::mg_verticalScrollbarWidth = 0;

LD20_Dialog::LD20_Dialog( const double p_topLeftX, const double p_topLeftY, const size_t p_maximumWidth, const size_t p_maximumHeight, const bool p_hasCloseButton )
{
  if( mg_numberOfInstances ++ == 0 )
  {
    mg_cornerTexture = new seal_Texture( "dialog_corner.png", f_CornerWidthInPixels(), f_CornerHeightInPixels());

    mg_horizontalEdgeTexture = new seal_Texture( "dialog_edge-horizontal.png", 1, f_CornerHeightInPixels());
    mg_verticalEdgeTexture = new seal_Texture( "dialog_edge-vertical.png", f_CornerWidthInPixels(), 1 );

    seal_Texture *texture = new seal_Texture( "dialog_close_button.png", 20, 20 );

    seal_Sprite::p_NewArchetype(( char* )"dialog close button", texture, true, k_seal_SPRITE_ANIMATION_BEHAVIOUR__HALTS, 0, 2, 0, 1, 1, 1 );
  }

  m_visibility = k_LD20_DialogVisibility_Opaque;
  m_opacity = 1.;
  m_topLeftX = p_topLeftX;
  m_topLeftY = p_topLeftY;
  m_maximumWidth = p_maximumWidth;
  m_maximumHeight = p_maximumHeight;
  m_width = 0.;
  m_height = 0.;

  m_topLeftCornerGeometry.p_Set( m_topLeftX - mg_cornerTexture->f_SubTextureWidth(), m_topLeftY, mg_cornerTexture->f_SubTextureWidth(), mg_cornerTexture->f_SubTextureHeight());

  m_buttonsMayBePressed = false;

  m_closeButton = NULL;
  if( p_hasCloseButton )
  {
    m_closeButton = new LD20_Sprite(( char* )"dialog close button" );
    m_closeButton->p_SetBottomLeftX( m_topLeftX );
    m_closeButton->p_SetBottomLeftY( m_topLeftY - m_closeButton->f_HeightInPixels());
    m_closeButton->p_SetOpacityEnabled( true );
  }
  m_closeButtonPressed = false;

  m_content = new LD20_DialogContentList( m_topLeftX + (( m_closeButton == NULL )?0:m_closeButton->f_WidthInPixels() + k_LD20_PixelsBetweenDialogCloseButtonAndContent ), m_topLeftY );
  m_content->p_SetBlendingEnabled( true );
  m_heightOfContent = 0.;
  m_contentScrolled = 0.;

  m_verticalScrollbar = new LD20_VerticalScrollbar( m_topLeftX, m_topLeftY, &m_width, &m_height, &m_heightOfContent, &m_contentScrolled, &m_opacity );

  if( mg_numberOfInstances == 1 )
  {
    seal_Sprite *sprite = new seal_Sprite(( char* )"vertical scrollbar button" );

    mg_verticalScrollbarWidth = sprite->f_WidthInPixels();
    delete sprite;
  }

  p_UpdateSizesAndPositions();
}

LD20_Dialog::~LD20_Dialog( void )
{
  delete m_verticalScrollbar;

  delete m_content;

  if( m_closeButton != NULL )
    delete m_closeButton;

  if( -- mg_numberOfInstances == 0 )
  {
    seal_Sprite::p_DeleteArchetype(( char* )"dialog close button" );

    delete mg_verticalEdgeTexture;
    delete mg_horizontalEdgeTexture;

    delete mg_cornerTexture;
  }
}

void LD20_Dialog::p_ResizeToFitContent( void )
{
  double d = m_content->f_Width();

  if( m_closeButton != NULL )
    d += ( m_closeButton->f_WidthInPixels() + k_LD20_PixelsBetweenDialogCloseButtonAndContent );
  if( m_heightOfContent > m_maximumHeight )
    d += ( mg_verticalScrollbarWidth + k_LD20_PixelsBetweenDialogVerticalScrollbarAndContent );
  m_width = ( d < m_maximumWidth )?d:m_maximumWidth;

  d = m_content->f_Height();
  m_height = ( d < m_maximumHeight )?d:m_maximumHeight;

  p_UpdateSizesAndPositions();
}

void LD20_Dialog::p_UpdateSize( double * const p_size, const size_t p_maximumSize, const double p_contentSize, const Uint32 p_ticks )
{
  double d = p_contentSize;

  if( d > p_maximumSize )
    d = p_maximumSize;
  if( *p_size < d )
  {
    ( *p_size ) += ( k_LD20_DialogSizeChangePerTick * p_ticks );
    if( *p_size > d )
      *p_size = d;
    else
      m_buttonsMayBePressed = false;
  }
  else
    if( *p_size > d )
    {
      ( *p_size ) -= ( k_LD20_DialogSizeChangePerTick * p_ticks );
      if( *p_size < d )
        *p_size = d;
      else
        m_buttonsMayBePressed = false;
    }
}

void LD20_Dialog::p_UpdateSizesAndPositions( void )
{
  m_bodyGeometry.p_Set( m_topLeftX, m_topLeftY - m_height, m_width, m_height );

  m_topRightCornerGeometry.p_Set( m_topLeftX + m_width, m_topLeftY, mg_cornerTexture->f_SubTextureWidth(), mg_cornerTexture->f_SubTextureHeight());
  m_topRightCornerGeometry.p_HFlip();

  m_bottomRightCornerGeometry.p_Set( m_topLeftX + m_width, m_topLeftY - m_height - mg_cornerTexture->f_SubTextureHeight(), mg_cornerTexture->f_SubTextureWidth(), mg_cornerTexture->f_SubTextureHeight());
  m_bottomRightCornerGeometry.p_HFlip();
  m_bottomRightCornerGeometry.p_VFlip();

  m_bottomLeftCornerGeometry.p_Set( m_topLeftX - mg_cornerTexture->f_SubTextureWidth(), m_topLeftY - m_height - mg_cornerTexture->f_SubTextureHeight(), mg_cornerTexture->f_SubTextureWidth(), mg_cornerTexture->f_SubTextureHeight());
  m_bottomLeftCornerGeometry.p_VFlip();

  m_topEdgeGeometry.p_Set( m_topLeftX, m_topLeftY, m_width, mg_horizontalEdgeTexture->f_SubTextureHeight());

  m_bottomEdgeGeometry.p_Set( m_topLeftX, m_topLeftY - m_height - mg_horizontalEdgeTexture->f_SubTextureHeight(), m_width, mg_horizontalEdgeTexture->f_SubTextureHeight());
  m_bottomEdgeGeometry.p_VFlip();

  m_leftEdgeGeometry.p_Set( m_topLeftX - mg_verticalEdgeTexture->f_SubTextureWidth(), m_topLeftY - m_height, mg_verticalEdgeTexture->f_SubTextureWidth(), m_height );

  m_rightEdgeGeometry.p_Set( m_topLeftX + m_width, m_topLeftY - m_height, mg_verticalEdgeTexture->f_SubTextureWidth(), m_height );
  m_rightEdgeGeometry.p_HFlip();

  m_verticalScrollbar->p_UpdateSizesAndPositions();
}

double LD20_Dialog::f_ClampScrolling( const double p )
{
  const double y_c = m_topLeftY - m_heightOfContent + p;
  const double y_d = m_topLeftY - m_height;
  const double h_c = m_heightOfContent;
  const double h_d = m_height;

  if( y_c > y_d )
    return h_c - h_d;
  else
    if( y_c + h_c < y_d + h_d )
      return 0;
    else
      return p;
}

void LD20_Dialog::p_Update( const Uint32 p )
{
  if( m_visibility == k_LD20_DialogVisibility_FadingIn )
  {
    m_opacity += ( k_LD20_DialogOpacityChangePerTick * p );
    if( m_opacity >= 1. )
    {
      m_visibility = k_LD20_DialogVisibility_Opaque;
      m_opacity = 1.;
    }
  }
  else
    if( m_visibility == k_LD20_DialogVisibility_FadingOut )
    {
      m_opacity -= ( k_LD20_DialogOpacityChangePerTick * p );
      if( m_opacity <= 0. )
      {
        m_visibility = k_LD20_DialogVisibility_Invisible;
        m_opacity = 0.;
        m_buttonsMayBePressed = false;
        return;
      }
    }

  m_buttonsMayBePressed = m_visibility == k_LD20_DialogVisibility_Opaque;

  if( m_closeButton != NULL )
    m_closeButton->p_SetOpacity( m_opacity );

  m_content->p_SetOpacity( m_opacity );

  p_UpdateSize( &m_width, m_maximumWidth, m_content->f_Width() + (( m_closeButton == NULL )?0:m_closeButton->f_WidthInPixels() + k_LD20_PixelsBetweenDialogCloseButtonAndContent ) + (( m_heightOfContent > m_maximumHeight )?mg_verticalScrollbarWidth + k_LD20_PixelsBetweenDialogVerticalScrollbarAndContent:0 ), p );

  p_UpdateSize( &m_height, m_maximumHeight, m_content->f_Height(), p );

  p_UpdateSizesAndPositions();

  if( m_buttonsMayBePressed )
  {
    if( m_closeButton != NULL )
    {
      m_closeButton->p_SetFrame( 0 );
      if( LD20_Cursor::f_Down())
      {
        if( f_ButtonPressed( LD20_Cursor::f_CurrentCollisionDetectionAABB()))
          m_closeButton->p_SetFrame( 1 );
      }
      else
        if( LD20_Cursor::f_WasDown())
          if( f_ButtonPressed( LD20_Cursor::f_UpCollisionDetectionAABB()))
            m_closeButtonPressed = true;
    }

    m_verticalScrollbar->p_UpdateButtons();
    if( m_verticalScrollbar->f_ButtonPressed() != k_LD20_VerticalScrollButtonPressed_None )
    {
      m_contentScrolled += ( m_verticalScrollbar->f_ButtonPressed() * -1 * k_LD20_PixelsScrolledPerTickByContentOfDialog * p );
      m_contentScrolled = f_ClampScrolling( m_contentScrolled );
    }
  }
}

void LD20_Dialog::p_Render( void )
{
  if( m_visibility != k_LD20_DialogVisibility_Invisible )
  {
    glVertexPointer( 2, GL_FLOAT, 0, m_bodyGeometry.m_coordinates );

    glEnableClientState( GL_COLOR_ARRAY );

    seal_QuadColours_Triangles colour( 182. / 255., 198. / 255., 212. / 255., m_opacity );

    glColorPointer( 4, GL_FLOAT, 0, colour.m_rgba );

    glDisable( GL_TEXTURE_2D );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );

    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnable( GL_TEXTURE_2D );

    colour.p_Set( 1., 1., 1., m_opacity );

    #define M_RenderCorner( p )                                                   \
      glVertexPointer( 2, GL_FLOAT, 0, m_ ## p ## CornerGeometry.m_coordinates ); \
      glDrawArrays( GL_TRIANGLES, 0, 6 );

    seal_Quad_Triangles texture_coordinates;

    mg_cornerTexture->p_GetSubTextureQuad( 0, &texture_coordinates );
    glTexCoordPointer( 2, GL_FLOAT, 0, texture_coordinates.m_coordinates );
    mg_cornerTexture->p_Bind();
    M_RenderCorner( topLeft )
    M_RenderCorner( topRight )
    M_RenderCorner( bottomRight )
    M_RenderCorner( bottomLeft )

    #undef M_RenderCorner

    #define M_RenderEdge( p )                                                   \
      glVertexPointer( 2, GL_FLOAT, 0, m_ ## p ## EdgeGeometry.m_coordinates ); \
      glDrawArrays( GL_TRIANGLES, 0, 6 );

    mg_horizontalEdgeTexture->p_GetSubTextureQuad( 0, &texture_coordinates );
    glTexCoordPointer( 2, GL_FLOAT, 0, texture_coordinates.m_coordinates );
    mg_horizontalEdgeTexture->p_Bind();
    M_RenderEdge( top )
    M_RenderEdge( bottom )

    mg_verticalEdgeTexture->p_GetSubTextureQuad( 0, &texture_coordinates );
    glTexCoordPointer( 2, GL_FLOAT, 0, texture_coordinates.m_coordinates );
    mg_verticalEdgeTexture->p_Bind();
    M_RenderEdge( left )
    M_RenderEdge( right )

    #undef M_RenderEdge

    glDisableClientState( GL_COLOR_ARRAY );

    if( m_closeButton != NULL )
      m_closeButton->p_Render();

    glEnable( GL_SCISSOR_TEST );
    glScissor( m_topLeftX, m_topLeftY - m_height, m_width + 1, m_height + 1 );
    glPushMatrix();
    glTranslatef( 0., ( GLfloat )( GLint )m_contentScrolled, 0. );
    m_content->p_Render();
    glPopMatrix();
    glDisable( GL_SCISSOR_TEST );

    if( m_heightOfContent > m_maximumHeight )
      m_verticalScrollbar->p_Render();
  }
}
