#ifndef LD20_DialogContent_JoystickIdMap_h

#define LD20_DialogContent_JoystickIdMap_h

#include "LD20_DialogContent.h"
#include "seal_list.h"
#include <SDL2/SDL.h>

typedef struct t_LD20_DialogContent_JoystickIdMapNode_struct
{
  LD20_DialogContent *m_dialogContent;
  SDL_JoystickID m_joystickId;
}
t_LD20_DialogContent_JoystickIdMapNode;

class LD20_DialogContent_JoystickIdMap : public seal_List<t_LD20_DialogContent_JoystickIdMapNode*>
{
  public:
    ~LD20_DialogContent_JoystickIdMap( void )
    {
      p_Empty();
    }

  private:
    void p_Delete( t_LD20_DialogContent_JoystickIdMapNode *p )
    {
      free( p );
    }

  public:
    void p_Set( LD20_DialogContent *p_dialogContent, SDL_JoystickID p_joystickId )
    {
      t_LD20_DialogContent_JoystickIdMapNode *n = ( t_LD20_DialogContent_JoystickIdMapNode* )malloc( sizeof( t_LD20_DialogContent_JoystickIdMapNode ));

      n->m_dialogContent = p_dialogContent;
      n->m_joystickId = p_joystickId;
      seal_List<t_LD20_DialogContent_JoystickIdMapNode*>::p_Set( n );
    }

  private:
    bool f_Compare( t_LD20_DialogContent_JoystickIdMapNode *p_a, t_LD20_DialogContent_JoystickIdMapNode *p_b )
    {
      return ( p_a->m_dialogContent == p_b->m_dialogContent ) && ( p_a->m_joystickId == p_b->m_joystickId );
    }

  public:
    void p_Remove( LD20_DialogContent *p_dialogContent, SDL_JoystickID p_joystickId )
    {
      t_LD20_DialogContent_JoystickIdMapNode n = { p_dialogContent, p_joystickId };

      seal_List<t_LD20_DialogContent_JoystickIdMapNode*>::p_Remove( &n );
    }

    SDL_JoystickID f_GetJoystickIdFromDialogContent( LD20_DialogContent *p )
    {
      size_t i = 0;

      for( ; i < f_Number(); i ++ )
      {
        t_LD20_DialogContent_JoystickIdMapNode *n = f_GetByIndex( i );

        if( n->m_dialogContent == p )
          return n->m_joystickId;
      }
    }
};

#endif
