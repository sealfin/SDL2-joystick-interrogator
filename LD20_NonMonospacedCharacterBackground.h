#ifndef LD20_NonMonospacedCharacterBackground_h

#define LD20_NonMonospacedCharacterBackground_h

#include "LD20_DialogContent.h"
#include "LD20_CharacterTree.h"
#include "seal_texture.h"

class LD20_NonMonospacedCharacterBackground : public LD20_DialogContent
{
  private:
    static LD20_CharacterTree *mg_unsupportedCharactersEncountered;
    static size_t mg_numberOfInstances;
    static seal_Texture *mg_texture;
    static unsigned int *mg_characterWidth;
    static signed int *mg_characterBaseline;

    double m_bottomLeftX, m_bottomLeftY;
    size_t m_widthInCharacters, m_heightInCharacters, m_widthInPixels, m_heightInPixels;
    t_seal_CollisionDetectionAABB m_collisionDetectionAABB;
    bool m_blendingEnabled;
    GLfloat m_opacity;
    bool m_stringSet;
    GLfloat *m_geometryCoordinates, *m_coordinateColours, *m_textureCoordinates;

  private:
    static size_t f_CharacterIndex( const char p );

  public:
    static void p_SetTexture( seal_Texture *p );
    static size_t f_SubTextureWidth( void );
    static size_t f_SubTextureHeight( void );
    static void p_RemoveTexture( void );
    static void p_SetCharacterWidth( const char * const p, ... );
    static void p_SetCharacterBaseline( const char * const p, ... );
    static size_t f_StringWidthInCharacters( const char * const p );
    static size_t f_StringHeightInCharacters( const char * const p );
    static size_t f_StringWidthInPixels( const char * const p );

  private:
    static void p_MinimumAndMaximumBaselineOfLine( const char * const p_string, size_t * const p_index, signed int * const p_minimumBaseline, signed int * const p_maximumBaseline );

  public:
    static size_t f_StringHeightInPixels( const char * const p );

    LD20_NonMonospacedCharacterBackground( void )
    {
      mg_numberOfInstances ++;
      m_bottomLeftX = 0.;
      m_bottomLeftY = 0.;
      m_widthInCharacters = 0;
      m_heightInCharacters = 0;
      m_widthInPixels = 0;
      m_heightInPixels = 0;
      m_collisionDetectionAABB.m_halfWidth = 0.;
      m_collisionDetectionAABB.m_originX = 0.;
      m_collisionDetectionAABB.m_halfHeight = 0.;
      m_collisionDetectionAABB.m_originY = 0.;
      m_blendingEnabled = false;
      m_opacity = 1.;
      m_stringSet = false;
    }

    ~LD20_NonMonospacedCharacterBackground( void )
    {
      if( m_stringSet )
      {
        free( m_geometryCoordinates );
        free( m_coordinateColours );
        free( m_textureCoordinates );
      }
      mg_numberOfInstances --;
    }

    void p_SetString( const char * const p );
    void p_Render( void );

    double f_GetBottomLeftX( void )
    {
      return m_bottomLeftX;
    }

    double f_GetBottomLeftY( void )
    {
      return m_bottomLeftY;
    }

    void p_SetBottomLeftX( const double p )
    {
      m_bottomLeftX = p;
      m_collisionDetectionAABB.m_originX = m_bottomLeftX + m_collisionDetectionAABB.m_halfWidth;
    }

    void p_SetBottomLeftY( const double p )
    {
      m_bottomLeftY = p;
      m_collisionDetectionAABB.m_originY = m_bottomLeftY + m_collisionDetectionAABB.m_halfHeight;
    }

    size_t f_WidthInPixels( void )
    {
      return m_widthInPixels;
    }

    size_t f_HeightInPixels( void )
    {
      return m_heightInPixels;
    }

    double f_Top( void )
    {
      return m_bottomLeftY + m_heightInPixels;
    }

    double f_Bottom( void )
    {
      return m_bottomLeftY;
    }

    double f_Left( void )
    {
      return m_bottomLeftX;
    }

    double f_Right( void )
    {
      return m_bottomLeftX + m_widthInPixels;
    }

    const t_seal_CollisionDetectionAABB * const f_CollisionDetectionAABB( void )
    {
      return &m_collisionDetectionAABB;
    }

    bool f_GetBlendingEnabled( void )
    {
      return m_blendingEnabled;
    }

    void p_SetBlendingEnabled( const bool p )
    {
      m_blendingEnabled = p;
    }

    GLfloat f_GetOpacity( void )
    {
      return m_opacity;
    }

    void p_SetOpacity( const GLfloat p )
    {
      m_opacity = p;

      size_t i = 3;

      for( ; i < m_widthInCharacters * m_heightInCharacters * 24; i += 4 )
        m_coordinateColours[ i ] = m_opacity;
    }
};

#endif
