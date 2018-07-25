#ifndef LD20_StringQueue_h

#define LD20_StringQueue_h

#include "seal_queue.h"
#include <stdio.h>
#include <string.h>

typedef struct t_LD20_StringQueueNode_struct
{
  char *m_string;
  bool m_deleteString;
}
t_LD20_StringQueueNode;

class LD20_StringQueue : public seal_Queue<t_LD20_StringQueueNode>
{
  public:
    ~LD20_StringQueue( void )
    {
      p_Empty();
    }

    void p_Push( char *p_string, const bool p_deleteString )
    {
      if( p_string == NULL )
      {
        fprintf( stderr, "LD20_StringQueue::p_Push() - p_string == NULL.\n" );
        throw "LD20_StringQueue::p_Push() - an error occurred; a description of the error has been printed to stderr.";
      }

      t_LD20_StringQueueNode n = { p_string, p_deleteString };

      seal_Queue<t_LD20_StringQueueNode>::p_Push( n );
    }

  private:
    void p_Delete( t_LD20_StringQueueNode p )
    {
      if( p.m_deleteString )
        free( p.m_string );
    }

  public:
    char *f_String( void )
    {
      seal_QueueNode<t_LD20_StringQueueNode> *n = m_first;
      size_t s_length = 0;

      while( n != NULL )
      {
        s_length += strlen( n->m_content.m_string );
        n = n->m_n;
      }

      char *s = ( char* )malloc( sizeof( char ) * ( s_length + 1 ));

      n = m_first;
      s[ 0 ] = '\0';
      while( n != NULL )
      {
        s = strcat( s, n->m_content.m_string );
        n = n->m_n;
      }
      return s;
    }
};

#endif
