#include "LD20_Cursor.h"

t_LD20_CursorState LD20_Cursor::mg_state = k_LD20_CursorState_Up;
t_seal_CollisionDetectionAABB LD20_Cursor::mg_currentCollisionDetectionAABB = { 0., 0., .5, .5 };
t_seal_CollisionDetectionAABB LD20_Cursor::mg_downCollisionDetectionAABB = { 0., 0., .5, .5 };
t_seal_CollisionDetectionAABB LD20_Cursor::mg_upCollisionDetectionAABB = { 0., 0., .5, .5 };
