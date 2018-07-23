#ifndef LD20_DialogContent_h

#define LD20_DialogContent_h

#include "seal_collisionDetection.h"
#include <stdlib.h>
#include <OpenGL/OpenGL.h>

class LD20_DialogContent
{
  public:
    virtual void p_Render( void ) = 0;
    virtual double f_GetBottomLeftX( void ) = 0;
    virtual double f_GetBottomLeftY( void ) = 0;
    virtual void p_SetBottomLeftX( const double p ) = 0;
    virtual void p_SetBottomLeftY( const double p ) = 0;
    virtual size_t f_WidthInPixels( void ) = 0;
    virtual size_t f_HeightInPixels( void ) = 0;
    virtual double f_Top( void ) = 0;
    virtual double f_Bottom( void ) = 0;
    virtual double f_Left( void ) = 0;
    virtual double f_Right( void ) = 0;
    virtual const t_seal_CollisionDetectionAABB * const f_CollisionDetectionAABB( void ) = 0;
    virtual bool f_GetBlendingEnabled( void ) = 0;
    virtual void p_SetBlendingEnabled( const bool p ) = 0;
    virtual GLfloat f_GetOpacity( void ) = 0;
    virtual void p_SetOpacity( const GLfloat p ) = 0;
};

#endif
