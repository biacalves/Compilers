#ifndef __MML_AST_MEM_ALLOC_NODE_H__
#define __MML_AST_MEM_ALLOC_NODE_H__

#include <cdk/ast/unary_operation_node.h>

namespace mml {

  /**
   * Class for describing memory allocation nodes.
   */
  class mem_alloc_node: public cdk::unary_operation_node {

  public:
    inline mem_alloc_node(int lineno, cdk::expression_node *argument) :
        cdk::unary_operation_node(lineno, argument) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_mem_alloc_node(this, level);
    }

  };

} // mml

#endif
