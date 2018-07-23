#include "LD20_NonMonospacedCharacterBackground.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

static const size_t k_LD20_NumberOfNonMonospacedCharacters = 71;

LD20_CharacterTree *LD20_NonMonospacedCharacterBackground::mg_unsupportedCharactersEncountered = NULL;
size_t LD20_NonMonospacedCharacterBackground::mg_numberOfInstances = 0;
seal_Texture *LD20_NonMonospacedCharacterBackground::mg_texture = NULL;
unsigned int *LD20_NonMonospacedCharacterBackground::mg_characterWidth = NULL;
signed int *LD20_NonMonospacedCharacterBackground::mg_characterBaseline = NULL;

size_t LD20_NonMonospacedCharacterBackground::f_CharacterIndex( const char p )
{
  if( isdigit( p ))
    return p - '0';
  if( islower( p ))
    return p - 'a' + 10;
  if( isupper( p ))
    return p - 'A' + 36;

  size_t i = 0;
  static const char * const characters = "()+-,:.! ";

  for( ; i < strlen( characters ); i ++ )
    if( p == characters[ i ] )
      return i + 62;
  if( mg_unsupportedCharactersEncountered == NULL )
    mg_unsupportedCharactersEncountered = new LD20_CharacterTree();
  if( !mg_unsupportedCharactersEncountered->f_IsInTree( p ))
  {
    fprintf( stderr, "LD20_NonMonospacedCharacterBackground::f_CharacterIndex - there is no sub-texture for the character '%c'.\n", p );
    mg_unsupportedCharactersEncountered->p_Set( p );
  }
  return 70;
}

#define M_CheckIfParameterIsNULL( p_method )                                                                                                   \
  if( p == NULL )                                                                                                                              \
  {                                                                                                                                            \
    fprintf( stderr, "LD20_NonMonospacedCharacterBackground::" # p_method " - p == NULL.\n" );                                                 \
    throw "LD20_NonMonospacedCharacterBackground::" # p_method " - an error occurred; a description of the error has been printed to stderr."; \
  }

void LD20_NonMonospacedCharacterBackground::p_SetTexture( seal_Texture *p )
{
  if( mg_texture != NULL )
  {
    fprintf( stderr, "LD20_NonMonospacedCharacterBackground::p_SetTexture - mg_texture != NULL.\n" );
    throw "LD20_NonMonospacedCharacterBackground::p_SetTexture - an error occurred; a description of the error has been printed to stderr.";
  }
  M_CheckIfParameterIsNULL( p_SetTexture )
  mg_texture = p;
  mg_characterWidth = ( unsigned int* )malloc( sizeof( unsigned int ) * k_LD20_NumberOfNonMonospacedCharacters );
  mg_characterBaseline = ( signed int* )malloc( sizeof( signed int ) * k_LD20_NumberOfNonMonospacedCharacters );

  size_t i = 0;

  for( ; i < k_LD20_NumberOfNonMonospacedCharacters; i ++ )
  {
    mg_characterWidth[ i ] = mg_texture->f_SubTextureWidth();
    mg_characterBaseline[ i ] = 0;
  }
}

#define M_CheckIfATextureHasNotBeenSet( p_method )                                                                                             \
  if( mg_texture == NULL )                                                                                                                     \
  {                                                                                                                                            \
    fprintf( stderr, "LD20_NonMonospacedCharacterBackground::" # p_method " - mg_texture == NULL.\n" );                                        \
    throw "LD20_NonMonospacedCharacterBackground::" # p_method " - an error occurred; a description of the error has been printed to stderr."; \
  }

size_t LD20_NonMonospacedCharacterBackground::f_SubTextureWidth( void )
{
  M_CheckIfATextureHasNotBeenSet( f_SubTextureWidth )
  return mg_texture->f_SubTextureWidth();
}

size_t LD20_NonMonospacedCharacterBackground::f_SubTextureHeight( void )
{
  M_CheckIfATextureHasNotBeenSet( f_SubTextureHeight )
  return mg_texture->f_SubTextureHeight();
}

void LD20_NonMonospacedCharacterBackground::p_RemoveTexture( void )
{
  if( mg_numberOfInstances > 0 )
  {
    fprintf( stderr, "LD20_NonMonospacedCharacterBackground::p_RemoveTexture - mg_numberOfInstances > 0.\n" );
    throw "LD20_NonMonospacedCharacterBackground::p_RemoveTexture - an error occurred; a description of the error has been printed to stderr.";
  }
  if( mg_texture != NULL )
  {
    mg_texture = NULL;
    free( mg_characterWidth );
    free( mg_characterBaseline );
  }
}

void LD20_NonMonospacedCharacterBackground::p_SetCharacterWidth( const char * const p, ... )
{
  M_CheckIfATextureHasNotBeenSet( p_SetCharacterWidth )
  M_CheckIfParameterIsNULL( p_SetCharacterWidth )

  va_list parameter;
  size_t i = 0;

  va_start( parameter, p );
  for( ; i < strlen( p ); i ++ )
    mg_characterWidth[ f_CharacterIndex( p[ i ] ) ] = va_arg( parameter, unsigned int );
  va_end( parameter );
}

void LD20_NonMonospacedCharacterBackground::p_SetCharacterBaseline( const char * const p, ... )
{
  M_CheckIfATextureHasNotBeenSet( p_SetCharacterBaseline )
  M_CheckIfParameterIsNULL( p_SetCharacterBaseline )

  va_list parameter;
  size_t i = 0;

  va_start( parameter, p );
  for( ; i < strlen( p ); i ++ )
    mg_characterBaseline[ f_CharacterIndex( p[ i ] ) ] = va_arg( parameter, signed int );
  va_end( parameter );
}

size_t LD20_NonMonospacedCharacterBackground::f_StringWidthInCharacters( const char * const p )
{
  M_CheckIfParameterIsNULL( f_StringWidthInCharacters )

  size_t i = 0, width = 0, greatest_width = 0;

  for( ; i < strlen( p ); i ++ )
    if( p[ i ] != '\n' )
      width ++;
    else
    {
      if( width > greatest_width )
        greatest_width = width;
      width = 0;
    }
  if( width > greatest_width )
    greatest_width = width;
  return greatest_width;
}

size_t LD20_NonMonospacedCharacterBackground::f_StringHeightInCharacters( const char * const p )
{
  M_CheckIfParameterIsNULL( f_StringHeightInCharacters )

  size_t i = 0, height = ( strlen( p ) == 0 )?0:1;

  for( ; i < strlen( p ); i ++ )
    if( p[ i ] == '\n' )
      height ++;
  return height;
}

size_t LD20_NonMonospacedCharacterBackground::f_StringWidthInPixels( const char * const p )
{
  M_CheckIfATextureHasNotBeenSet( f_StringWidthInPixels )
  M_CheckIfParameterIsNULL( f_StringWidthInPixels )

  size_t i = 0, width = 0, greatest_width = 0;

  for( ; i < strlen( p ); i ++ )
    if( p[ i ] != '\n' )
      width += mg_characterWidth[ f_CharacterIndex( p[ i ] ) ];
    else
    {
      if( width > greatest_width )
        greatest_width = width;
      width = 0;
    }
  if( width > greatest_width )
    greatest_width = width;
  return greatest_width;
}

void LD20_NonMonospacedCharacterBackground::p_MinimumAndMaximumBaselineOfLine( const char * const p_string, size_t * const p_index, signed int * const p_minimumBaseline, signed int * const p_maximumBaseline )
{
  *p_minimumBaseline = *p_maximumBaseline = mg_characterBaseline[ f_CharacterIndex( p_string[ ( *p_index ) ++  ] ) ];
  while(( *p_index < strlen( p_string )) && ( p_string[ *p_index ] != '\n' ))
  {
    const size_t i = f_CharacterIndex( p_string[ *p_index ] );

    if( mg_characterBaseline[ i ] > *p_maximumBaseline )
      *p_maximumBaseline = mg_characterBaseline[ i ];
    if( mg_characterBaseline[ i ] < *p_minimumBaseline )
      *p_minimumBaseline = mg_characterBaseline[ i ];
    ( *p_index ) ++;
  }
}

size_t LD20_NonMonospacedCharacterBackground::f_StringHeightInPixels( const char * const p )
{
  M_CheckIfATextureHasNotBeenSet( f_StringHeightInPixels )
  M_CheckIfParameterIsNULL( f_StringHeightInPixels )

  size_t i = 0;
  signed int minimum_baseline_of_last_line, maximum_baseline;
  bool first_line = true;
  signed int maximum_baseline_of_first_line;
  size_t height = 0;

  for( ; i < strlen( p ); i ++ )
  {
    p_MinimumAndMaximumBaselineOfLine( p, &i, &minimum_baseline_of_last_line, &maximum_baseline );
    if( first_line )
    {
      first_line = false;
      maximum_baseline_of_first_line = maximum_baseline;
    }
    height += mg_texture->f_SubTextureHeight();
  }
  return height + maximum_baseline_of_first_line - minimum_baseline_of_last_line;
}

void LD20_NonMonospacedCharacterBackground::p_SetString( const char * const p )
{
  M_CheckIfATextureHasNotBeenSet( p_SetString )
  M_CheckIfParameterIsNULL( p_SetString )

  if( m_stringSet )
  {
    free( m_geometryCoordinates );
    free( m_coordinateColours );
    free( m_textureCoordinates );
  }

  m_widthInCharacters = f_StringWidthInCharacters( p );
  m_heightInCharacters = f_StringHeightInCharacters( p );
  m_widthInPixels = f_StringWidthInPixels( p );
  m_heightInPixels = f_StringHeightInPixels( p );

  m_collisionDetectionAABB.m_halfWidth = m_widthInPixels / 2.;
  m_collisionDetectionAABB.m_originX = m_bottomLeftX + m_collisionDetectionAABB.m_halfWidth;
  m_collisionDetectionAABB.m_halfHeight = m_heightInPixels / 2.;
  m_collisionDetectionAABB.m_originY = m_bottomLeftY + m_collisionDetectionAABB.m_halfHeight;

  m_geometryCoordinates = ( GLfloat* )malloc( sizeof( GLfloat ) * m_widthInCharacters * m_heightInCharacters * 12 );
  m_textureCoordinates = ( GLfloat* )malloc( sizeof( GLfloat ) * m_widthInCharacters * m_heightInCharacters * 12 );

  seal_Quad_Triangles geometry_coordinates, texture_coordinates;
  size_t i = 0, m, k = 0;

  geometry_coordinates.p_Set( 0, 0, mg_texture->f_SubTextureWidth(), mg_texture->f_SubTextureHeight());
  mg_texture->p_GetSubTextureQuad( f_CharacterIndex( ' ' ), &texture_coordinates );
  for( ; i < m_widthInCharacters * m_heightInCharacters; i ++ )
    for( m = 0; m < 12; k ++, m ++ )
    {
      m_geometryCoordinates[ k ] = geometry_coordinates.m_coordinates[ m ];
      m_textureCoordinates[ k ] = texture_coordinates.m_coordinates[ m ];
    }

  size_t x_pixel = 0, y_pixel = m_heightInPixels - mg_texture->f_SubTextureHeight();

  k = 0;
  for( i = 0; i < strlen( p ); i ++ )
  {
    if( p[ i ] != '\n' )
    {
      const size_t character_index = f_CharacterIndex( p[ i ] );

      geometry_coordinates.p_Set( x_pixel, ( GLfloat )y_pixel + mg_characterBaseline[ character_index ], mg_texture->f_SubTextureWidth(), mg_texture->f_SubTextureHeight());
      mg_texture->p_GetSubTextureQuad( character_index, &texture_coordinates );
      for( m = 0; m < 12; k ++, m ++ )
      {
        m_geometryCoordinates[ k ] = geometry_coordinates.m_coordinates[ m ];
        m_textureCoordinates[ k ] = texture_coordinates.m_coordinates[ m ];
      }
      x_pixel += mg_characterWidth[ character_index ];
    }
    else
    {
      x_pixel = 0;
      y_pixel -= mg_texture->f_SubTextureHeight();
    }
  }

  k = 0;
  m_coordinateColours = ( GLfloat* )malloc( sizeof( GLfloat ) * m_widthInCharacters * m_heightInCharacters * 24 );
  for( i = 0; i < m_widthInCharacters * m_heightInCharacters * 24; i ++ )
    if( k ++ == 3 )
    {
      k = 0;
      m_coordinateColours[ i ] = m_opacity;
    }
    else
      m_coordinateColours[ i ] = 1.;

  m_stringSet = true;
}

#undef M_CheckIfParameterIsNULL

#undef M_CheckIfATextureHasNotBeenSet

void LD20_NonMonospacedCharacterBackground::p_Render( void )
{
  if( !m_stringSet )
  {
    fprintf( stderr, "LD20_NonMonospacedCharacterBackground::p_Render - m_stringSet == false.\n" );
    throw "LD20_NonMonospacedCharacterBackground::p_Render - an error occurred; a description of the error has been printed to stderr.";
  }
  glVertexPointer( 2, GL_FLOAT, 0, m_geometryCoordinates );
  if( m_blendingEnabled )
  {
    glEnableClientState( GL_COLOR_ARRAY );
    glColorPointer( 4, GL_FLOAT, 0, m_coordinateColours );
  }
  glTexCoordPointer( 2, GL_FLOAT, 0, m_textureCoordinates );
  mg_texture->p_Bind();
  glPushMatrix();
  glTranslatef(( GLfloat )( GLint )m_bottomLeftX, ( GLfloat )( GLint )m_bottomLeftY, 0 );
  glDrawArrays( GL_TRIANGLES, 0, m_widthInCharacters * m_heightInCharacters * 6 );
  glPopMatrix();
  if( m_blendingEnabled )
    glDisableClientState( GL_COLOR_ARRAY );
}
