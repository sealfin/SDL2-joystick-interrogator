#ifndef LD20_Trackballs_h

#define LD20_Trackballs_h

#include "LD20_NonMonospacedCharacterBackground.h"
#include "LD20_StringQueue.h"
#include "seal_intToString.h"
#include <SDL2/SDL.h>

class LD20_Trackballs
{
  private:
    bool m_initialised;
    int m_numberOfTrackballs;

  protected:
    LD20_NonMonospacedCharacterBackground *m_background;

    LD20_Trackballs( void )
    {
      m_initialised = false;
    }

    ~LD20_Trackballs( void )
    {
      if( m_initialised )
        delete m_background;
    }

    void p_Initialise( SDL_Joystick * const p )
    {
      if( m_initialised )
      {
        fprintf( stderr, "LD20_Trackballs::p_Initialise - m_initialised == true.\n" );
        throw "LD20_Trackballs::p_Initialise - an error occurred; a description of the error has been printed to stderr.";
      }

      const char *name = SDL_JoystickName( p );

      if( name == NULL )
        name = "(No name)";

      m_numberOfTrackballs = SDL_JoystickNumBalls( p );
      if( m_numberOfTrackballs < 0 )
        fprintf( stderr, "Sorry, an error occurred whilst trying to retrieve the number of trackballs for the \"%s\" - SDL_GetError() states that \"%s\".\n", name, SDL_GetError());
      m_background = new LD20_NonMonospacedCharacterBackground();
      m_initialised = true;
      p_UpdateBackground();
    }

  private:
    void p_UpdateBackground( void )
    {
      if( !m_initialised )
      {
        fprintf( stderr, "LD20_Trackballs::p_UpdateBackground - m_initialised == false.\n" );
        throw "LD20_Trackballs::p_UpdateBackground - an error occurred; a description of the error has been printed to stderr.";
      }

      LD20_StringQueue sQ;
      char *s;

      if( m_numberOfTrackballs < 0 )
      {
        sQ.p_Push(( char* )"SDL_JoystickNumBalls() == ", false );
        s = f_seal_IntToString( m_numberOfTrackballs, false );
        sQ.p_Push( s, true );
        sQ.p_Push(( char* )"\nPlease refer to stderr!", false );
      }
      else
      {
        if( m_numberOfTrackballs == 0 )
          sQ.p_Push(( char* )"No", false );
        else
        {
          s = f_seal_IntToString( m_numberOfTrackballs, false );
          sQ.p_Push( s, true );
        }
        sQ.p_Push(( char* )" trackball", false );
        if( m_numberOfTrackballs != 1 )
          sQ.p_Push(( char* )"s", false );
        sQ.p_Push(( char* )" detected.", false );
      }
      s = sQ.f_String();
      m_background->p_SetString( s );
      free( s );
    }
};

#endif
