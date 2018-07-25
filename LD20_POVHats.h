#ifndef LD20_POVHats_h

#define LD20_POVHats_h

#include "LD20_JoystickFeature.h"

class LD20_POVHats : protected LD20_JoystickFeature<Uint8>
{
  protected:
    LD20_POVHats( void ) : LD20_JoystickFeature<Uint8>(( char* )"POV hats", SDL_JoystickNumHats, ( char* )"SDL_JoystickNumHats", SDL_JoystickGetHat )
    {
    }

  public:
    void p_MovePOVHat( const Uint8 p_POVHat, const Uint8 p_value )
    {
      p_SetValue( p_POVHat, p_value );
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
        switch( f_GetValue( i ))
        {
          case SDL_HAT_LEFTUP: sQ.p_Push(( char* )"left-up", false ); break;
          case SDL_HAT_LEFT: sQ.p_Push(( char* )"left", false ); break;
          case SDL_HAT_LEFTDOWN: sQ.p_Push(( char* )"left-down", false ); break;

          case SDL_HAT_UP: sQ.p_Push(( char* )"up", false ); break;
          case SDL_HAT_CENTERED: sQ.p_Push(( char* )"centred", false ); break;
          case SDL_HAT_DOWN: sQ.p_Push(( char* )"down", false ); break;

          case SDL_HAT_RIGHTUP: sQ.p_Push(( char* )"right-up", false ); break;
          case SDL_HAT_RIGHT: sQ.p_Push(( char* )"right", false ); break;
          case SDL_HAT_RIGHTDOWN: sQ.p_Push(( char* )"right-down", false );
        }
        if( i + 1 < f_NumberOfValues())
          sQ.p_Push(( char* )"\n", false );
      }
      s = sQ.f_String();
      m_valuesBackground->p_SetString( s );
      free( s );
    }
};

#endif
