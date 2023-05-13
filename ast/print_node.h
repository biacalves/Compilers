#ifndef __MML_AST_PRINT_NODE_H__
#define __MML_AST_PRINT_NODE_H__

#include <cdk/ast/expression_node.h>

namespace mml {

  /**
   * Class for describing print nodes.
   */
  class print_node: public cdk::basic_node {
    cdk::expression_node *_argument;
    bool _is_newline;

  public:
    inline print_node(int lineno, cdk::expression_node *argument, bool is_newline) :
        cdk::basic_node(lineno), _argument(argument), _is_newline(is_newline) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    inline bool is_newline() {
      return _is_newline;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_print_node(this, level);
    }

  };

} // mml

#endif
