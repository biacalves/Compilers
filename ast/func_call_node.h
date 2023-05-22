#ifndef __MML_AST_FUNC_CALL_NODE_H__
#define __MML_AST_FUNC_CALL_NODE_H__

#include <cdk/ast/expression_node.h>

namespace mml {

  /**
   * Class for describing function call nodes.
   */
  class func_call_node: public cdk::expression_node {
    std::string _identifier;
    cdk::sequence_node *_arguments;

  public:
    inline func_call_node(int lineno, const std::string &identifier) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(new cdk::sequence_node(lineno)) {
    }

    inline func_call_node(int lineno, const std::string &identifier, cdk::sequence_node *arguments) :
        cdk::expression_node(lineno), _identifier(identifier), _arguments(arguments) {
    }

  public:
    inline const std::string& identifier() const {
      return _identifier;
    }

    inline cdk::sequence_node* arguments() {
      return _arguments;
    }

    inline cdk::expression_node* argument(size_t ax) {
      return dynamic_cast<cdk::expression_node*>(_arguments->node(ax));
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_call_node(this, level);
    }

  };

} // mml

#endif
