#ifndef __MML_AST_VARIABLE_DECL_NODE_H__
#define __MML_AST_VARIABLE_DECL_NODE_H__

#include <cdk/ast/typed_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace mml {

  /**
   * Class for describing variable declaration nodes.
   */
  class variable_decl_node: public cdk::typed_node {
    int _qualifier;
    bool _isAuto;
    std::string _identifier;
    cdk::expression_node *_initializer;

  public:
    inline variable_decl_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> varType, 
      bool isAuto, const std::string &identifier, cdk::expression_node *initializer) :
        cdk::typed_node(lineno), _qualifier(qualifier), _isAuto(isAuto), _identifier(identifier), _initializer(initializer) {
          type(varType);
    }

    inline variable_decl_node(int lineno, int qualifier, bool isAuto,
      const std::string &identifier, cdk::expression_node *initializer) :
        cdk::typed_node(lineno), _qualifier(qualifier), _isAuto(isAuto), _identifier(identifier), _initializer(initializer) {
    }

  public:
    inline int qualifier() {
      return _qualifier;
    }

    inline std::string& identifier() {
      return _identifier;
    }

    inline bool isAuto() {
      return _isAuto;
    }

    inline cdk::expression_node* initializer() {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_decl_node(this, level);
    }

  };

} // mml

#endif
