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
    int _qualifier;
    std::string _identifier;
    cdk::sequence_node *_arguments;
    block_node *_block;

  public:
    inline func_definition_node(int lineno, int qualifier, const std::string &identifier, 
      cdk::sequence_node *arguments, block_node *block) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier), 
        _arguments(arguments), _block(block) {
          type(cdk::primitive_type::create(0, cdk::TYPE_VOID));
    }

    inline func_definition_node(int lineno, int qualifier, const std::string &identifier, 
      cdk::sequence_node *arguments, std::shared_ptr<cdk::basic_type> funcType, block_node *block) :
        cdk::typed_node(lineno), _qualifier(qualifier), _identifier(identifier), 
        _arguments(arguments), _block(block) {
          type(funcType);
    }

  public:
    int qualifier() {
      return _qualifier;
    }

    const std::string& identifier() const {
      return _identifier;
    }

    cdk::sequence_node* arguments() {
      return _arguments;
    }

    cdk::typed_node* argument(size_t ax) {
      return dynamic_cast<cdk::typed_node*>(_arguments->node(ax));
    }

    block_node* block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_func_definition_node(this, level);
    }

  };

} // mml

#endif
