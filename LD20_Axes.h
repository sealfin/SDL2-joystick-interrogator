#ifndef LD20_Axes_h

#define LD20_Axes_h

#include "LD20_JoystickFeature.h"

class LD20_Axes : protected LD20_JoystickFeature<Sint16>
{
  protected:
    LD20_Axes( void ) : LD20_JoystickFeature<Sint16>(( char* )"axes", SDL_JoystickNumAxes, ( char* )"SDL_JoystickNumAxes", SDL_JoystickGetAxis )
    {
    }

  public:
    void p_MoveAxis( const Uint8 p_axis, const Sint16 p_value )
    {
      p_SetValue( p_axis, p_value );
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
        s = f_seal_IntToString( f_GetValue( i ), true );
        sQ.p_Push( s, true );
        if( i + 1 < f_NumberOfValues())
          sQ.p_Push(( char* )"\n", false );
      }
      s = sQ.f_String();
      m_valuesBackground->p_SetString( s );
      free( s );
    }
};

#endif
