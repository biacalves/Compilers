#ifndef __MML_TARGETS_SYMBOL_H__
#define __MML_TARGETS_SYMBOL_H__

#include <string>
#include <memory>
#include <cdk/types/basic_type.h>

namespace mml {

  class symbol {
    std::shared_ptr<cdk::basic_type> _type;
    std::string _identifier;
    int _offset;
    bool _isPublic;
    bool _isForward;
    bool _isForeign;
    bool _isAuto;
    //std::vector<std::shared_ptr<cdk::basic_type>> _vars;
    //std::shared_ptr<cdk::basic_type> _returnValue;
    //block_node *_block;

  public:
    symbol(std::shared_ptr<cdk::basic_type> type, const std::string &identifier, int offset, bool isPublic,
    bool isForward, bool isForeign, bool isAuto):
        _type(type), _identifier(identifier), _offset(offset), _isPublic(isPublic), _isForward(isForward), _isForeign(isForeign),
        _isAuto(isAuto) {
    }

    /*symbol(std::shared_ptr<cdk::basic_type> type, const std::string &identifier, int offset, bool isPublic, bool isForward, 
    bool isForeign, bool isAuto, std::vector<std::shared_ptr<cdk::basic_type>> vars):
        _type(type), _identifier(identifier), _offset(offset), _isPublic(isPublic), _isForward(isForward), _isForeign(isForeign),
        _isAuto(isAuto), _vars(vars) {
    }*/

    virtual ~symbol() {
      // EMPTY
    }

    std::shared_ptr<cdk::basic_type> type() const {
      return _type;
    }

    bool is_typed(cdk::typename_type name) const {
      return _type->name() == name;
    }

    const std::string &identifier() const {
      return _identifier;
    }

    int offset(){
      return _offset;
    }

    void offset(int offset){
      _offset = offset;
    }

    bool isPublic() {
      return _isPublic;
    }

    void isPublic(bool isPublic) {
      _isPublic = isPublic;
    }

    bool isForward() {
      return _isForward;
    }

    bool isForeign() {
      return _isForeign;
    }

    bool isAuto() {
      return _isAuto;
    }

    /*std::vector<std::shared_ptr<cdk::basic_type>> vars() {
      return _vars;
    }*/

    /*std::shared_ptr<cdk::basic_type> returnValue() {
      return _returnValue;
    }

    block_node *block() {
      return _block;
    }*/
  };

} // mml

#endif
