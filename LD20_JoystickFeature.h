#ifndef LD20_JoystickFeature_h

#define LD20_JoystickFeature_h

#include "LD20_NonMonospacedCharacterBackground.h"
#include "LD20_StringQueue.h"
#include "seal_intToString.h"
#include "seal_stringConcatenation.h"
#include <ctype.h>
#include <SDL2/SDL.h>

template <class T>
class LD20_JoystickFeature
{
  private:
    char *m_featureName; /* Eg., "axes", not "Axes". */
    int ( *m_getNumberOfValues )( SDL_Joystick* );
    char *m_getNumberOfValuesName;
    T ( *m_getValue )( SDL_Joystick*, int );

    bool m_initialised;
    int m_numberOfValues;
    T *m_values;
    size_t m_xOffset, m_yOffset;

  protected:
    LD20_NonMonospacedCharacterBackground *m_numberAndIndicesBackground, *m_valuesBackground;

    #define M_Assign( p )                                                                                                        \
      if( p_ ## p == NULL )                                                                                                      \
      {                                                                                                                          \
        fprintf( stderr, "LD20_JoystickFeature::<constructor> - p_" # p " == NULL.\n" );                                         \
        throw "LD20_JoystickFeature::<constructor> - an error occurred; a description of the error has been printed to stderr."; \
      }                                                                                                                          \
      m_ ## p = p_ ## p;

    LD20_JoystickFeature( char *p_featureName /* Eg., "axes", not "Axes". */, int ( *p_getNumberOfValues )( SDL_Joystick* ), char *p_getNumberOfValuesName, T ( *p_getValue )( SDL_Joystick*, int ))
    {
      m_initialised = false;
      M_Assign( featureName )
      if( strlen( p_featureName ) == 0 )
      {
        fprintf( stderr, "LD20_JoystickFeature::<constructor> - strlen( p_featureName ) == 0.\n" );
        throw "LD20_JoystickFeature::<constructor> - an error occurred; a description of the error has been printed to stderr.";
      }
      M_Assign( getNumberOfValues )
      M_Assign( getNumberOfValuesName )
      M_Assign( getValue )
      m_xOffset = 0;
      m_yOffset = 0;
    }

    #undef M_Assign

    ~LD20_JoystickFeature( void )
    {
      if( m_initialised )
      {
        delete m_valuesBackground;
        delete m_numberAndIndicesBackground;
        if( m_numberOfValues > 0 )
          free( m_values );
      }
    }

    void p_Initialise( SDL_Joystick * const p )
    {
      if( m_initialised )
      {
        fprintf( stderr, "LD20_JoystickFeature::p_Initialise - m_initialised == true.\n" );
        throw "LD20_JoystickFeature::p_Initialise - an error occurred; a description of the error has been printed to stderr.";
      }

      const char *name = SDL_JoystickName( p );

      if( name == NULL )
        name = "(No name)";

      int i = 0;

      m_numberOfValues = m_getNumberOfValues( p );
      if( m_numberOfValues < 0 )
        fprintf( stderr, "Sorry, an error occurred whilst trying to retrieve the number of %s for the \"%s\" - SDL_GetError() states that \"%s\".\n", m_featureName, name, SDL_GetError());
      else
        if( m_numberOfValues > 0 )
        {
          m_values = ( T* )malloc( sizeof( T ) * m_numberOfValues );
          for( ; i < m_numberOfValues; i ++ )
            m_values[ i ] = m_getValue( p, i );
        }

      LD20_StringQueue sQ;
      char *s;

      if( m_numberOfValues < 0 )
      {
        sQ.p_Push( m_getNumberOfValuesName, false );
        sQ.p_Push(( char* )"() == ", false );
        s = f_seal_IntToString( m_numberOfValues, false );
        sQ.p_Push( s, true );
        sQ.p_Push(( char* )"\nPlease refer to stderr!", false );
      }
      else
        if( m_numberOfValues == 0 )
        {
          sQ.p_Push(( char* )"No ", false );
          sQ.p_Push( m_featureName, false );
          sQ.p_Push(( char* )" detected.", false );
        }
        else
        {
          s = ( char* )malloc( sizeof( char ) * ( strlen( m_featureName ) + 1 ));
          strcpy( s, m_featureName );
          s[ 0 ] = toupper( s[ 0 ] );
          sQ.p_Push( s, true );
          sQ.p_Push(( char* )" (", false );
          s = f_seal_IntToString( m_numberOfValues, false );
          sQ.p_Push( s, true );
          sQ.p_Push(( char* )") detected:\n", false );

          s = sQ.f_String();
          m_yOffset = LD20_NonMonospacedCharacterBackground::f_StringHeightInPixels( s );
          free( s );

          for( i = 0; i < m_numberOfValues; i ++ )
          {
            sQ.p_Push(( char* )"  ", false );
            s = f_seal_IntToString( i, false );
            sQ.p_Push( s, true );

            s = f_seal_ConcatenateStrings(( char* )"  ", s, NULL );

            const size_t offset = LD20_NonMonospacedCharacterBackground::f_StringWidthInPixels( s );

            if( offset > m_xOffset )
              m_xOffset = offset;
            free( s );

            if( i + 1 < m_numberOfValues )
              sQ.p_Push(( char* )"\n", false );
          }
        }
      s = sQ.f_String();
      m_numberAndIndicesBackground = new LD20_NonMonospacedCharacterBackground();
      m_numberAndIndicesBackground->p_SetString( s );
      free( s );

      m_valuesBackground = new LD20_NonMonospacedCharacterBackground();
      m_initialised = true;
      p_UpdateValuesBackground();
    }

    #define M_CheckIfNotInitialised( p )                                                                                   \
      if( !m_initialised )                                                                                                 \
      {                                                                                                                    \
        fprintf( stderr, "LD20_JoystickFeature::" # p " - m_initialised == false.\n" );                                    \
        throw "LD20_JoystickFeature::" # p " - an error occurred; a description of the error has been printed to stderr."; \
      }

    int f_NumberOfValues( void )
    {
      M_CheckIfNotInitialised( f_NumberOfValues )
      return m_numberOfValues;
    }

    #define M_CheckIfNotAnIndexOfAValue( p )                                                                               \
      if(( p_index < 0 ) || ( p_index >= m_numberOfValues ))                                                               \
      {                                                                                                                    \
        fprintf( stderr, "LD20_JoystickFeature::" # p " - p_index < 0 || p_index ≥ m_numberOfValues.\n" );                 \
        throw "LD20_JoystickFeature::" # p " - an error occurred; a description of the error has been printed to stderr."; \
      }

    T f_GetValue( const int p_index )
    {
      M_CheckIfNotInitialised( f_GetValue )
      M_CheckIfNotAnIndexOfAValue( f_GetValue )
      return m_values[ p_index ];
    }

    void p_SetValue( const int p_index, T p_value )
    {
      M_CheckIfNotInitialised( p_SetValue )
      M_CheckIfNotAnIndexOfAValue( p_SetValue )
      m_values[ p_index ] = p_value;
      p_UpdateValuesBackground();
    }

    #undef M_CheckIfNotInitialised

    #undef M_CheckIfNotAnIndexOfAValue

    virtual void p_UpdateValuesBackground( void ) = 0;

    size_t f_XOffset( void )
    {
      return m_xOffset;
    }

    size_t f_YOffset( void )
    {
      return m_yOffset;
    }
};

#endif
