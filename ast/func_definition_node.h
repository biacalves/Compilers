#ifndef __MML_AST_FUNC_DEFINITION_NODE_H__
#define __MML_AST_FUNC_DEFINITION_NODE_H__

#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include "ast/block_node.h"

namespace mml {

  /**
   * Class for describing function definition nodes.
   */
  class func_definition_node: public cdk::typed_node {
    cdk::sequence_node *_variables;
    block_node *_block;

  public:
    inline func_definition_node(int lineno, cdk::sequence_node *variables, block_node *block) :
        cdk::typed_node(lineno), _variables(variables), _block(block) {
          type(cdk::primitive_type::create(0, cdk::TYPE_VOID));
    }

    inline func_definition_node(int lineno, cdk::sequence_node *variables, 
      std::shared_ptr<cdk::basic_type> funcType, block_node *block) :
        cdk::typed_node(lineno), _variables(variables), _block(block) {
          type(funcType);
    }

  public:
    inline cdk::sequence_node* variables() {
      return _variables;
    }

    inline cdk::typed_node* variable(size_t ax) {
      return dynamic_cast<cdk::typed_node*>(_variables->node(ax));
    }

    inline block_node* block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_definition_node(this, level);
    }

  };

} // mml

#endif
