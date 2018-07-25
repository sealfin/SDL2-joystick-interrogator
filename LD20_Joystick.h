#ifndef LD20_Joystick_h

#define LD20_Joystick_h

#include "LD20_Axes.h"
#include "LD20_Buttons.h"
#include "LD20_POVHats.h"
#include "LD20_Trackballs.h"
#include "LD20_Dialog.h"

typedef enum
{
  k_LD20_FailedToOpenJoystickError
}
t_LD20_FailedToOpenJoystickError;

typedef enum
{
  k_LD20_FailedToRetrieveJoystickInstanceIdError
}
t_LD20_FailedToRetrieveJoystickInstanceIdError;

class LD20_Joystick : public LD20_Axes, public LD20_Buttons, public LD20_POVHats, public LD20_Trackballs
{
  private:
    SDL_Joystick *m_joystick;
    SDL_JoystickID m_instanceId;
    LD20_NonMonospacedCharacterBackground *m_nameBackground, *m_hapticBackground;

  public:
    LD20_NonMonospacedCharacterBackground *m_namesOfJoysticksDialogContent;

    LD20_Joystick( const Sint32 p )
    {
      m_joystick = SDL_JoystickOpen( p );
      if( m_joystick == NULL )
      {
        fprintf( stderr, "Sorry, an error occurred whilst trying to open a joystick - SDL_GetError() states that \"%s\".\n", SDL_GetError());
        throw k_LD20_FailedToOpenJoystickError;
      }

      m_instanceId = SDL_JoystickInstanceID( m_joystick );
      if( m_instanceId < 0 )
      {
        fprintf( stderr, "Sorry, an error occurred whilst trying to retrieve the instance Id of a joystick - SDL_GetError() states that \"%s\".\n", SDL_GetError());
        throw k_LD20_FailedToRetrieveJoystickInstanceIdError;
      }

      const char *name = SDL_JoystickName( m_joystick );

      if( name == NULL )
        name = "(No name)";
      m_nameBackground = new LD20_NonMonospacedCharacterBackground();
      m_nameBackground->p_SetString( name );

      const int joystick_is_haptic = SDL_JoystickIsHaptic( m_joystick );
      LD20_StringQueue sQ;
      char *s;

      if( joystick_is_haptic < 0 )
      {
        fprintf( stderr, "Sorry, an error occurred whilst trying to determine if the \"%s\" has haptic features - SDL_GetError() states that \"%s\".\n", name, SDL_GetError());
        sQ.p_Push(( char* )"SDL_JoystickIsHaptic() == ", false );
        s = f_seal_IntToString( joystick_is_haptic, false );
        sQ.p_Push( s, true );
        sQ.p_Push(( char* )"\nPlease refer to stderr!", false );
      }
      else
      {
        if( joystick_is_haptic == 0 )
          sQ.p_Push(( char* )"No h", false );
        else
          sQ.p_Push(( char* )"H", false );
        sQ.p_Push(( char* )"aptic features detected.", false );
      }
      m_hapticBackground = new LD20_NonMonospacedCharacterBackground();
      s = sQ.f_String();
      m_hapticBackground->p_SetString( s );
      free( s );

      m_namesOfJoysticksDialogContent = new LD20_NonMonospacedCharacterBackground();
      m_namesOfJoysticksDialogContent->p_SetString( name );

      LD20_Axes::p_Initialise( m_joystick );
      LD20_Buttons::p_Initialise( m_joystick );
      LD20_POVHats::p_Initialise( m_joystick );
      LD20_Trackballs::p_Initialise( m_joystick );
    }

    ~LD20_Joystick( void )
    {
      delete m_namesOfJoysticksDialogContent;
      delete m_hapticBackground;
      delete m_nameBackground;
      SDL_JoystickClose( m_joystick );
    }

    SDL_JoystickID f_InstanceId( void )
    {
      return m_instanceId;
    }

    void p_SetDialog( LD20_Dialog * const p )
    {
      size_t y = 0;

      p_SetDialog( p, m_nameBackground, NULL, 0, &y, true, 0, 0 );
      p_SetDialog( p, m_hapticBackground, NULL, 0, &y, true, 0, 0 );
      p_SetDialog( p, LD20_Axes::m_numberAndIndicesBackground, LD20_Axes::m_valuesBackground, 0, &y, false, LD20_Axes::f_XOffset(), LD20_Axes::f_YOffset());

      size_t x = f_Highest( LD20_Axes::m_numberAndIndicesBackground->f_WidthInPixels(), LD20_Axes::m_valuesBackground->f_WidthInPixels() + LD20_Axes::f_XOffset());

      x += LD20_NonMonospacedCharacterBackground::f_SubTextureWidth();

      p_SetDialog( p, LD20_Buttons::m_numberAndIndicesBackground, LD20_Buttons::m_valuesBackground, x, &y, false, LD20_Buttons::f_XOffset(), LD20_Buttons::f_YOffset());

      const size_t a = f_Highest( LD20_Axes::m_numberAndIndicesBackground->f_HeightInPixels(), LD20_Axes::m_valuesBackground->f_HeightInPixels() + LD20_Axes::f_YOffset());
      const size_t b = f_Highest( LD20_Buttons::m_numberAndIndicesBackground->f_HeightInPixels(), LD20_Buttons::m_valuesBackground->f_HeightInPixels() + LD20_Buttons::f_YOffset());

      y += f_Highest( a, b );
      y += LD20_NonMonospacedCharacterBackground::f_SubTextureHeight();

      p_SetDialog( p, LD20_POVHats::m_numberAndIndicesBackground, LD20_POVHats::m_valuesBackground, 0, &y, false, LD20_POVHats::f_XOffset(), LD20_POVHats::f_YOffset());

      y += f_Highest( LD20_POVHats::m_numberAndIndicesBackground->f_HeightInPixels(), LD20_POVHats::m_valuesBackground->f_HeightInPixels() + LD20_POVHats::f_YOffset());
      y += LD20_NonMonospacedCharacterBackground::f_SubTextureHeight();

      p_SetDialog( p, LD20_Trackballs::m_background, NULL, 0, &y, false, 0, 0 );
    }

  private:
    void p_SetDialog( LD20_Dialog * const p_dialog, LD20_NonMonospacedCharacterBackground * const p_numberAndIndicesBackground, LD20_NonMonospacedCharacterBackground * const p_valuesBackground, const size_t p_x, size_t * const p_y, const bool p_updateY, const size_t p_xOffset, const size_t p_yOffset )
    {
      p_numberAndIndicesBackground->p_SetBottomLeftX( p_x );
      p_numberAndIndicesBackground->p_SetBottomLeftY( *p_y );
      p_dialog->p_SetContent( p_numberAndIndicesBackground );

      if( p_valuesBackground != NULL )
      {
        p_valuesBackground->p_SetBottomLeftX( p_x + p_xOffset );
        p_valuesBackground->p_SetBottomLeftY( *p_y + p_yOffset );
        p_dialog->p_SetContent( p_valuesBackground );
      }

      if( p_updateY )
      {
        ( *p_y ) += p_numberAndIndicesBackground->f_HeightInPixels();
        ( *p_y ) += LD20_NonMonospacedCharacterBackground::f_SubTextureHeight();
      }
    }

    size_t f_Highest( const size_t a, const size_t b )
    {
      return ( a > b )?a:b;
    }
};

#endif
