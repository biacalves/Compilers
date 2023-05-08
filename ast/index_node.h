#ifndef __MML_AST_INDEX_NODE_H__
#define __MML_AST_INDEX_NODE_H__

#include <cdk/ast/lvalue_node.h>

namespace mml {

  /**
   * Class for describing indexing nodes.
   */
  class index_node: public cdk::lvalue_node {
    cdk::expression_node *_ptr;
    cdk::expression_node *_index;

  public:
    inline index_node(int lineno, cdk::expression_node *ptr, cdk::expression_node *index) :
        cdk::lvalue_node(lineno), _ptr(ptr), _index(index) {
    }

  public:
    inline cdk::expression_node *ptr() {
      return _ptr;
    }

    inline cdk::expression_node *index() {
      return _index;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // mml

#endif
