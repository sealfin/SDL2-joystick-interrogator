#ifndef LD20_CharacterTree_h

#define LD20_CharacterTree_h

#include "seal_tree.h"

class LD20_CharacterTree : public seal_Tree<char, char>
{
  private:
    t_seal_TREE_BRANCH_DIRECTION f_Compare_TT( char p_old, char p_new )
    {
      if( p_new < p_old )
        return k_seal_TREE_BRANCH_DIRECTION__LEFT;
      if( p_new == p_old )
        return k_seal_TREE_BRANCH_DIRECTION__STRAIGHT;
      return k_seal_TREE_BRANCH_DIRECTION__RIGHT;
    }

    t_seal_TREE_BRANCH_DIRECTION f_Compare_TU( char p_content, char p_identifier )
    {
      if( p_identifier < p_content )
        return k_seal_TREE_BRANCH_DIRECTION__LEFT;
      if( p_identifier == p_content )
        return k_seal_TREE_BRANCH_DIRECTION__STRAIGHT;
      return k_seal_TREE_BRANCH_DIRECTION__RIGHT;
    }

    char f_IsNotInTree( char p )
    {
    }
};

#endif
