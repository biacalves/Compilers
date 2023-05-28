#ifndef __MML_AST_FUNC_DEFINITION_NODE_H__
#define __MML_AST_FUNC_DEFINITION_NODE_H__

#include <cdk/ast/typed_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "ast/block_node.h"
#include "ast/variable_decl_node.h"

namespace mml {

  /**
   * Class for describing function definition nodes.
   */
  class func_definition_node: public cdk::typed_node {
    bool _isPublic;
    bool _isForward;
    bool _isForeign;
    bool _isAuto;
    std::string _identifier;
    cdk::sequence_node *_variables;
    block_node *_block;

  public:
    inline func_definition_node(int lineno, bool isPublic, bool isForward, bool isForeign, bool isAuto,
        std::shared_ptr<cdk::basic_type> funcType, const std::string &identifier, cdk::sequence_node *variables, 
        std::shared_ptr<cdk::basic_type> returnValue, block_node *block) :
        cdk::typed_node(lineno), _isPublic(isPublic), _isForward(isForward), _isForeign(isForeign), 
        _isAuto(isAuto), _identifier(identifier), _variables(variables), _block(block) {
          type(funcType);
          type(returnValue);
    }

    inline func_definition_node(int lineno, bool isPublic, bool isForward, bool isForeign, bool isAuto,
        const std::string &identifier, cdk::sequence_node *variables, 
        std::shared_ptr<cdk::basic_type> returnValue, block_node *block) :
        cdk::typed_node(lineno), _isPublic(isPublic), _isForward(isForward), _isForeign(isForeign), 
        _isAuto(isAuto), _identifier(identifier), _variables(variables), _block(block) {
          type(returnValue);
    }
    
  public:
    inline bool isPublic() {
      return _isPublic;
    }

    inline bool isForward() {
      return _isForward;
    }

    inline bool isForeign() {
      return _isForeign;
    }

    inline bool isAuto() {
      return _isAuto;
    }

    inline std::string& identifier() {
      return _identifier;
    }

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
