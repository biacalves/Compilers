#ifndef __MML_AST_WRITE_NODE_H__
#define __MML_AST_WRITE_NODE_H__

#include <cdk/ast/expression_node.h>

namespace mml {

  /**
   * Class for describing write nodes.
   */
  class write_node: public cdk::basic_node {

  public:
    inline write_node(int lineno) :
        cdk::basic_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_write_node(this, level);
    }

  };

} // mml

#endif
