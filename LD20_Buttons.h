#ifndef LD20_Buttons_h

#define LD20_Buttons_h

#include "LD20_JoystickFeature.h"

class LD20_Buttons : protected LD20_JoystickFeature<Uint8> /* `SDL_JoystickGetButton` returns `Uint8`, not `bool`. */
{
  protected:
    LD20_Buttons( void ) : LD20_JoystickFeature<Uint8>(( char* )"buttons", SDL_JoystickNumButtons, ( char* )"SDL_JoystickNumButtons", SDL_JoystickGetButton )
    {
    }

  public:
    void p_PressButton( const Uint8 p )
    {
      p_SetValue( p, true );
    }

    void p_ReleaseButton( const Uint8 p )
    {
      p_SetValue( p, false );
    }

  private:
    void p_UpdateValuesBackground( void )
    {
      int i = 0;
      LD20_StringQueue sQ;
      char *s;

      for( ; i < f_NumberOfValues(); i ++ )
      {
        sQ.p_Push(( char* )" : ", false );
        sQ.p_Push(( char* )(( !f_GetValue( i ))?"Up":"Down" ), false );
        if( i + 1 < f_NumberOfValues())
          sQ.p_Push(( char* )"\n", false );
      }
      s = sQ.f_String();
      m_valuesBackground->p_SetString( s );
      free( s );
    }
};

#endif
