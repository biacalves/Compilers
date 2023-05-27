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
    bool _isPublic;
    bool _isForward;
    bool _isForeign;
    bool _isAuto;
    std::string _identifier;
    cdk::expression_node *_initializer;

  public:
    inline variable_decl_node(int lineno, bool isPublic, bool isForward, bool isForeign, bool isAuto, 
      std::shared_ptr<cdk::basic_type> varType, const std::string &identifier, cdk::expression_node *initializer) :
        cdk::typed_node(lineno), _isPublic(isPublic), _isForward(isForward), _isForeign(isForeign), 
        _isAuto(isAuto), _identifier(identifier), _initializer(initializer) {
          type(varType);
    }

    inline variable_decl_node(int lineno, bool isPublic, bool isForward, bool isForeign, bool isAuto, 
        const std::string &identifier, cdk::expression_node *initializer) :
        cdk::typed_node(lineno), _isPublic(isPublic), _isForward(isForward), _isForeign(isForeign), 
        _isAuto(isAuto), _identifier(identifier), _initializer(initializer) {
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

    inline cdk::expression_node* initializer() {
      return _initializer;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_decl_node(this, level);
    }

  };

} // mml

#endif
