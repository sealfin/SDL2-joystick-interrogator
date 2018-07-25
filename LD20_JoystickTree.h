#ifndef LD20_JoystickTree_h

#define LD20_JoystickTree_h

#include "LD20_Joystick.h"
#include "LD20_DialogContent_JoystickIdMap.h"

const size_t k_LD20_PixelsBetweenWindowAndDialog = 2;

class LD20_JoystickTree : public seal_Tree<LD20_Joystick*, SDL_JoystickID>
{
  private:
    LD20_Dialog *m_namesOfJoysticksDialog;
    LD20_DialogContent_JoystickIdMap m_map;
    LD20_Dialog *m_joystickDialog;
    LD20_Joystick *m_joystickShowing;

  public:
    LD20_JoystickTree( const size_t p_windowWidth, const size_t p_windowHeight )
    {
      const size_t x = k_LD20_PixelsBetweenWindowAndDialog + LD20_Dialog::f_CornerWidthInPixels();
      const size_t y = p_windowHeight - ( k_LD20_PixelsBetweenWindowAndDialog + LD20_Dialog::f_CornerHeightInPixels());

      const size_t w = p_windowWidth - ( k_LD20_PixelsBetweenWindowAndDialog + LD20_Dialog::f_CornerWidthInPixels()) * 2;
      const size_t h = p_windowHeight - ( k_LD20_PixelsBetweenWindowAndDialog + LD20_Dialog::f_CornerHeightInPixels()) * 2;

      m_namesOfJoysticksDialog = new LD20_Dialog( x, y, w, h, false );
      m_namesOfJoysticksDialog->p_MakeInvisible();

      m_joystickDialog = new LD20_Dialog( x, y, w, h, true );
      m_joystickDialog->p_MakeInvisible();

      m_joystickShowing = NULL;
    }

    ~LD20_JoystickTree( void )
    {
      delete m_joystickDialog;
      delete m_namesOfJoysticksDialog;
      p_Empty();
    }

    void p_Open( const Sint32 p )
    {
      LD20_Joystick *n;

      try
      {
        n = new LD20_Joystick( p );
      }
      catch( t_LD20_FailedToOpenJoystickError p )
      {
        return;
      }
      catch( t_LD20_FailedToRetrieveJoystickInstanceIdError p )
      {
        return;
      }
      m_namesOfJoysticksDialog->p_SetContent( n->m_namesOfJoysticksDialogContent );
      m_namesOfJoysticksDialog->p_TidyUpContent();
      if( m_namesOfJoysticksDialog->f_NumberOfItemsOfContent() == 1 )
      {
        m_namesOfJoysticksDialog->p_ResizeToFitContent();
        m_namesOfJoysticksDialog->p_FadeIn();
      }
      m_map.p_Set( n->m_namesOfJoysticksDialogContent, n->f_InstanceId());
      p_Set( n );
    }

    void p_Close( const Sint32 p )
    {
      LD20_Joystick *n = f_Get( p );

      if( n != NULL )
      {
        m_namesOfJoysticksDialog->p_RemoveContent( n->m_namesOfJoysticksDialogContent );
        if( m_namesOfJoysticksDialog->f_NumberOfItemsOfContent() == 0 )
          m_namesOfJoysticksDialog->p_MakeInvisible();
        else
          m_namesOfJoysticksDialog->p_TidyUpContent();
        m_map.p_Remove( n->m_namesOfJoysticksDialogContent, n->f_InstanceId());
        if( n == m_joystickShowing )
        {
          m_joystickShowing = NULL;
          m_joystickDialog->p_RemoveAllContent();
          m_joystickDialog->p_MakeInvisible();
        }
        p_Remove( p );
      }
    }

    void p_Render( void )
    {
      m_namesOfJoysticksDialog->p_Render();
      m_joystickDialog->p_Render();
    }

    void p_MoveAxis( const SDL_JoystickID p_instanceId, const Uint8 p_axis, const Sint16 p_value )
    {
      LD20_Joystick *n = f_Get( p_instanceId );

      if( n != NULL )
        n->p_MoveAxis( p_axis, p_value );
    }

    void p_PressButton( const SDL_JoystickID p_instanceId, const Uint8 p_button )
    {
      LD20_Joystick *n = f_Get( p_instanceId );

      if( n != NULL )
        n->p_PressButton( p_button );
    }

    void p_ReleaseButton( const SDL_JoystickID p_instanceId, const Uint8 p_button )
    {
      LD20_Joystick *n = f_Get( p_instanceId );

      if( n != NULL )
        n->p_ReleaseButton( p_button );
    }

    void p_MovePOVHat( const SDL_JoystickID p_instanceId, const Uint8 p_POVHat, const Uint8 p_value )
    {
      LD20_Joystick *n = f_Get( p_instanceId );

      if( n != NULL )
        n->p_MovePOVHat( p_POVHat, p_value );
    }

    void p_Update( const Uint32 p )
    {
      m_namesOfJoysticksDialog->p_Update( p );
      if( m_joystickDialog->f_IsInvisible())
      {
        LD20_DialogContent *c = m_namesOfJoysticksDialog->f_ContentPressed();

        if( c != NULL )
        {
          m_joystickShowing = f_Get( m_map.f_GetJoystickIdFromDialogContent( c ));
          m_joystickShowing->p_SetDialog( m_joystickDialog );
          m_joystickDialog->p_ResizeToFitContent();
          m_joystickDialog->p_FadeIn();
        }
      }
      m_joystickDialog->p_Update( p );
      if( m_joystickDialog->f_CloseButtonPressed())
        m_joystickDialog->p_FadeOut();
      if(( m_joystickShowing != NULL ) && m_joystickDialog->f_IsInvisible())
      {
        m_joystickShowing = NULL;
        m_joystickDialog->p_RemoveAllContent();
      }
    }

  private:
    t_seal_TREE_BRANCH_DIRECTION f_Compare_TT( LD20_Joystick *p_old, LD20_Joystick *p_new )
    {
      if( p_new->f_InstanceId() < p_old->f_InstanceId())
        return k_seal_TREE_BRANCH_DIRECTION__LEFT;
      if( p_new->f_InstanceId() == p_old->f_InstanceId())
        return k_seal_TREE_BRANCH_DIRECTION__STRAIGHT;
      return k_seal_TREE_BRANCH_DIRECTION__RIGHT;
    }

    t_seal_TREE_BRANCH_DIRECTION f_Compare_TU( LD20_Joystick *p_content, SDL_JoystickID p_identifier )
    {
      if( p_identifier < p_content->f_InstanceId())
        return k_seal_TREE_BRANCH_DIRECTION__LEFT;
      if( p_identifier == p_content->f_InstanceId())
        return k_seal_TREE_BRANCH_DIRECTION__STRAIGHT;
      return k_seal_TREE_BRANCH_DIRECTION__RIGHT;
    }

    LD20_Joystick *f_IsNotInTree( SDL_JoystickID p )
    {
      return NULL;
    }

    void p_Delete( LD20_Joystick *p )
    {
      delete p;
    }
};

#endif
