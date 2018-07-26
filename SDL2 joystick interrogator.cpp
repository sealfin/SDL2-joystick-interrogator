#include "LD20_JoystickTree.h"

static LD20_NonMonospacedCharacterBackground *g_number_of_joysticks_background;

void p_UpdateNumberOfJoysticksBackground( void )
{
  const int number_of_joysticks = SDL_NumJoysticks();
  LD20_StringQueue sQ;
  char *s;

  if( number_of_joysticks < 0 )
  {
    fprintf( stderr, "Sorry, an error occurred whilst trying to retrieve the number of joysticks - SDL_GetError() states that \"%s\".\n", SDL_GetError());
    sQ.p_Push(( char* )"SDL_NumJoysticks() == ", false );
    s = f_seal_IntToString( number_of_joysticks, false );
    sQ.p_Push( s, true );
    sQ.p_Push(( char* )"\nPlease refer to stderr!", false );
  }
  else
  {
    if( number_of_joysticks == 0 )
      sQ.p_Push(( char* )"No", false );
    else
    {
      s = f_seal_IntToString( number_of_joysticks, false );
      sQ.p_Push( s, true );
    }
    sQ.p_Push(( char* )" joystick", false );
    if( number_of_joysticks != 1 )
      sQ.p_Push(( char* )"s", false );
    sQ.p_Push(( char* )" detected.", false );
  }
  s = sQ.f_String();
  g_number_of_joysticks_background->p_SetString( s );
  free( s );
}

int main( const int argc, const char * const argv[] )
{
  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC ) < 0 )
  {
    fprintf( stderr, "Sorry, an error occurred whilst trying to initialise SDL - SDL_GetError() states that \"%s\".\n", SDL_GetError());
    SDL_Quit();
    exit( EXIT_FAILURE );
  }

  const size_t window_width = 800, window_height = 600;
  SDL_Window *window = SDL_CreateWindow( "SDL2 joystick interrogator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL );

  if( window == NULL )
  {
    fprintf( stderr, "Sorry, an error occurred whilst trying to create a window - SDL_GetError() states that \"%s\".\n", SDL_GetError());
    SDL_Quit();
    exit( EXIT_FAILURE );
  }

  SDL_GLContext context = SDL_GL_CreateContext( window );

  if( context == NULL )
  {
    fprintf( stderr, "Sorry, an error occurred whilst trying to create an OpenGL context - SDL_GetError() states that \"%s\".\n", SDL_GetError());
    SDL_DestroyWindow( window );
    SDL_Quit();
    exit( EXIT_FAILURE );
  }

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0., window_width, 0., window_height, -1., 1. );
  glMatrixMode( GL_MODELVIEW );
  glClearColor( 0.5, 0.5, 0.5, 1. );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable( GL_BLEND );
  glEnable( GL_TEXTURE_2D );
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnableClientState( GL_TEXTURE_COORD_ARRAY );

  seal_Texture *font = new seal_Texture( "font.png", 19, 19 );

  LD20_NonMonospacedCharacterBackground::p_SetTexture( font );
  LD20_NonMonospacedCharacterBackground::p_SetCharacterWidth( "17", 8, 18 );

  LD20_NonMonospacedCharacterBackground::p_SetCharacterWidth( "abcdefghijklmnopqrstuvwxyz", 17, 17, 17, 16, 17, 16, 16, 17, 8, 16, 17, 8, 18, 17, 17, 17, 17, 17, 17, 16, 17, 17, 18, 17, 17, 16 );
  LD20_NonMonospacedCharacterBackground::p_SetCharacterBaseline( "gpq", -3, -3, -3 );

  LD20_NonMonospacedCharacterBackground::p_SetCharacterWidth( "FPZ", 18, 18, 18 );

  LD20_NonMonospacedCharacterBackground::p_SetCharacterWidth( "(),:.!", 10, 10, 7, 8, 7, 8 );
  LD20_NonMonospacedCharacterBackground::p_SetCharacterBaseline( ",!", -2, 1 );

  g_number_of_joysticks_background = new LD20_NonMonospacedCharacterBackground();
  p_UpdateNumberOfJoysticksBackground();

  LD20_Dialog *number_of_joysticks_dialog = new LD20_Dialog( k_LD20_PixelsBetweenWindowAndDialog + LD20_Dialog::f_CornerWidthInPixels(), window_height - ( k_LD20_PixelsBetweenWindowAndDialog + LD20_Dialog::f_CornerHeightInPixels()), window_width - ( k_LD20_PixelsBetweenWindowAndDialog + LD20_Dialog::f_CornerWidthInPixels()) * 2, window_height - ( k_LD20_PixelsBetweenWindowAndDialog + LD20_Dialog::f_CornerHeightInPixels()) * 2, false );

  number_of_joysticks_dialog->p_SetContent( g_number_of_joysticks_background );
  number_of_joysticks_dialog->p_TidyUpContent();
  number_of_joysticks_dialog->p_ResizeToFitContent();

  LD20_JoystickTree *joysticks = new LD20_JoystickTree( window_width, window_height - ( k_LD20_PixelsBetweenWindowAndDialog + LD20_Dialog::f_CornerHeightInPixels() * 2 + LD20_NonMonospacedCharacterBackground::f_SubTextureHeight()));

  bool quit = false;

  Uint32 ticks_before_iteration = SDL_GetTicks();

  Uint8 number_of_iterations = 0;
  const Uint8 target_number_of_iterations_per_second = 30;
  Uint32 ticks_before_first_iteration = SDL_GetTicks(), delay_by_ticks = 0;

  do
  {
    SDL_Event event;

    while( SDL_PollEvent( &event ))
      switch( event.type )
      {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_MOUSEMOTION:
          LD20_Cursor::p_Move( event.motion.x, window_height - event.motion.y );
          break;
        case SDL_MOUSEBUTTONDOWN:
          if( event.button.button == SDL_BUTTON_LEFT )
            LD20_Cursor::p_Press( event.button.x, window_height - event.button.y );
          break;
        case SDL_MOUSEBUTTONUP:
          if( event.button.button == SDL_BUTTON_LEFT )
            LD20_Cursor::p_Release( event.button.x, window_height - event.button.y );
          break;
        case SDL_JOYDEVICEADDED:
          p_UpdateNumberOfJoysticksBackground();
          joysticks->p_Open( event.jdevice.which );
          break;
        case SDL_JOYDEVICEREMOVED:
          p_UpdateNumberOfJoysticksBackground();
          joysticks->p_Close( event.jdevice.which );
          break;
        case SDL_JOYAXISMOTION:
          joysticks->p_MoveAxis( event.jaxis.which, event.jaxis.axis, event.jaxis.value );
          break;
        case SDL_JOYBUTTONDOWN:
          joysticks->p_PressButton( event.jbutton.which, event.jbutton.button );
          break;
        case SDL_JOYBUTTONUP:
          joysticks->p_ReleaseButton( event.jbutton.which, event.jbutton.button );
          break;
        case SDL_JOYHATMOTION:
          joysticks->p_MovePOVHat( event.jhat.which, event.jhat.hat, event.jhat.value );
      }
    glClear( GL_COLOR_BUFFER_BIT );

    const Uint32 ticks = SDL_GetTicks() - ticks_before_iteration;
    ticks_before_iteration = SDL_GetTicks();

    number_of_joysticks_dialog->p_Update( ticks );
    number_of_joysticks_dialog->p_Render();

    joysticks->p_Update( ticks );
    joysticks->p_Render();

    LD20_Cursor::p_SetState_Up_IfState_WasDown();

    SDL_GL_SwapWindow( window );

    if( ++ number_of_iterations == target_number_of_iterations_per_second )
    {
      number_of_iterations = 0;

      const Sint64 ticks = ( Sint64 )1000 - ( SDL_GetTicks() - ticks_before_first_iteration );

      ticks_before_first_iteration = SDL_GetTicks();
      if( ticks > 0 )
        delay_by_ticks = ticks / target_number_of_iterations_per_second;
      else
        delay_by_ticks = 0;
    }
    SDL_Delay( delay_by_ticks );
  }
  while( !quit );

  delete joysticks;
  delete number_of_joysticks_dialog;
  delete g_number_of_joysticks_background;
  LD20_NonMonospacedCharacterBackground::p_RemoveTexture();
  delete font;

  SDL_GL_DeleteContext( context );
  SDL_DestroyWindow( window );
  SDL_Quit();
  exit( EXIT_SUCCESS );
}
