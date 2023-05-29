#ifndef __MML_AST_STOP_NODE_H__
#define __MML_AST_STOP_NODE_H__

#include <cdk/ast/basic_node.h>

namespace mml {

  /**
   * Class for describing stop nodes.
   */
  class stop_node: public cdk::basic_node {
    cdk::expression_node *_level;

  public:
    inline stop_node(int lineno, cdk::expression_node *level) :
        cdk::basic_node(lineno), _level(level) {
    }

  public:
    inline cdk::expression_node *level()  {
      return _level;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_stop_node(this, level);
    }

  };

} // mml

#endif
