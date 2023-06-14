#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include ".auto/all_nodes.h"  // all_nodes.h is automatically generated

//---------------------------------------------------------------------------

void mml::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}

void mml::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void mml::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (isGlobal()) { 
    _pf.SDOUBLE(node->value()); // double literal is on the stack: push a double
  } 
  else {
    _pf.DOUBLE(node->value()); // double literal is on the DATA segment
  }
}

void mml::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();
}

void mml::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

void mml::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (isGlobal()) { 
    _pf.SINT(node->value()); // integer literal is on the DATA segment
  } else {
    _pf.INT(node->value()); // integer literal is on the stack: push an integer
  }
}

void mml::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  if (isGlobal()) {
  // global variable initializer
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  } 
  else {
    // local variable initializer
    _pf.TEXT(); // return to the TEXT segment
    _pf.ADDR(mklbl(lbl1));  // the string to be printed
  }
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl); // determine the value
  if(node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DNEG(); 
  else
    _pf.NEG();
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  } 
  else if (node->type()->name() == cdk::TYPE_POINTER && node->left()->type()->name() == cdk::TYPE_INT) {
    if(std::dynamic_pointer_cast<cdk::reference_type>(node->type())->referenced()->name() == cdk::TYPE_DOUBLE) {
      _pf.INT(3);
    }
    else {
      _pf.INT(2);
    }
    _pf.SHTL();
  }

  node->right()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  } 
  else if (node->type()->name() == cdk::TYPE_POINTER && node->right()->type()->name() == cdk::TYPE_INT) {
    if(std::dynamic_pointer_cast<cdk::reference_type>(node->type())->referenced()->name() == cdk::TYPE_DOUBLE) {
      _pf.INT(3);
    }
    else {
      _pf.INT(2);
    }
    _pf.SHTL();
  }

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DADD();
  else
    _pf.ADD();
}

void mml::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  } else if (node->type()->name() == cdk::TYPE_POINTER && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DSUB();
  else
    _pf.SUB();
}

void mml::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  }

  if(node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DMUL();
  else
    _pf.MUL();
}

void mml::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  }

  if(node->type()->name() == cdk::TYPE_DOUBLE)
    _pf.DDIV();
  else
    _pf.DIV();
}

void mml::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->left()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  }

  node->right()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT) {
    _pf.I2D();
  }
  
  _pf.MOD();
}

void mml::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }
  
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == cdk::TYPE_INT && node->left()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }

  if(node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  else
    _pf.LT();
}

void mml::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }
  
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == cdk::TYPE_INT && node->left()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }

  if(node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  else
    _pf.LE();
}

void mml::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }
  
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == cdk::TYPE_INT && node->left()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }

  if(node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  else
    _pf.GE();
}

void mml::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }
  
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == cdk::TYPE_INT && node->left()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }

  if(node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  else
    _pf.GT();
}

void mml::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }
  
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == cdk::TYPE_INT && node->left()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }

  if(node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  else
    _pf.NE();
}

void mml::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }
  
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == cdk::TYPE_INT && node->left()->type()->name() == cdk::TYPE_DOUBLE){
    _pf.I2D();
  }

  if(node->left()->type()->name() == cdk::TYPE_DOUBLE || node->right()->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  }
  else
    _pf.EQ();
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<mml::symbol> symb = _symtab.find(node->name());
  // simplified generation: all variables are global
  
  _pf.ADDR(symb->identifier());
}

void mml::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if(!(node->type()->name() == cdk::TYPE_DOUBLE))
    _pf.LDINT();
  else
    _pf.LDDOUBLE();
}

void mml::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl); 
  if(node->is_typed(cdk::TYPE_DOUBLE)){
    if(node->rvalue()->is_typed(cdk::TYPE_INT)){
      _pf.I2D();
    }
    _pf.DUP64();
  }
  else
    _pf.DUP32();

  if (new_symbol() != nullptr) {
    _pf.DATA();
    _pf.ALIGN();
    _pf.LABEL(new_symbol()->identifier());
    reset_new_symbol();
    _pf.SINT(0);
    _pf.TEXT();
  }

  node->lvalue()->accept(this, lvl);
  if(node->lvalue()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.STDOUBLE();
  }
  else {
    _pf.STINT();
  }
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_program_node(mml::program_node * const node, int lvl) {
  // Note that MML doesn't have functions. Thus, it doesn't need
  // a function node. However, it must start in the main function.
  // The ProgramNode (representing the whole program) doubles as a
  // main function node.

  ASSERT_SAFE_EXPRESSIONS;

  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL("_main");
  _pf.ENTER(0);  // MML doesn't implement local variables

  if (node->declarations() != nullptr) {
    node->declarations()->accept(this, lvl);
  }
  if (node->instructions() != nullptr) {
    node->instructions()->accept(this, lvl);
  }
  
  // end the main function
  _pf.INT(0);
  _pf.STFVAL32();
  _pf.LEAVE();
  _pf.RET();

  // these are just a few library function imports
  _pf.EXTERN("readi");
  _pf.EXTERN("readd");
  _pf.EXTERN("printi");
  _pf.EXTERN("prints");
  _pf.EXTERN("printd");
  _pf.EXTERN("println");
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_evaluation_node(mml::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->is_typed(cdk::TYPE_STRING)) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else if (node->argument()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.TRASH(8); // delete the evaluated value
  } else if (node->argument()->is_typed(cdk::TYPE_POINTER)) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void mml::postfix_writer::do_print_node(mml::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print

  for(size_t i = 0; i < node->argument()->size(); i++){
    cdk::expression_node *expression = dynamic_cast<cdk::expression_node *>(node->argument()->node(i));
  
    if (expression->is_typed(cdk::TYPE_INT)) {
      _functions_to_declare.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4); // delete the printed value
    } else if (expression->is_typed(cdk::TYPE_STRING)) {
      _functions_to_declare.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4); // delete the printed value's address
    } else if (expression->is_typed(cdk::TYPE_DOUBLE)) {
      _functions_to_declare.insert("printd");
      _pf.CALL("printd");
      _pf.TRASH(8); // trash double
    } else {
      std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
      exit(1);
    }
  }

  if (node->is_newline()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println"); // print a newline
  }
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_while_node(mml::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_if_node(mml::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_if_else_node(mml::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void mml::postfix_writer::do_stop_node(mml::stop_node * const node, int lvl) {
  // EMPTY
}

void mml::postfix_writer::do_next_node(mml::next_node * const node, int lvl) {
  // EMPTY
}

void mml::postfix_writer::do_return_node(mml::return_node * const node, int lvl) {
  // EMPTY
}

void mml::postfix_writer::do_block_node(mml::block_node * const node, int lvl) {
  _symtab.push();

  if(node->declarations()){
    node->declarations()->accept(this, lvl+2);
  }

  if(node->instructions()){
    node->instructions()->accept(this, lvl+2);
  }

  _symtab.pop();
}

void mml::postfix_writer::do_input_node(mml::input_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _functions_to_declare.insert("readd");
    _pf.CALL("readd");
    _pf.LDFVAL64();
  } 
  else if (node->is_typed(cdk::TYPE_INT)) {
    _functions_to_declare.insert("readi");
    _pf.CALL("readi");
    _pf.LDFVAL32();
  } 
  else {
    std::cerr << "FATAL: " << node->lineno() << ": cannot read type" << std::endl;
    return;
  }
}

void mml::postfix_writer::do_null_node(mml::null_node * const node, int lvl) {
  if(isGlobal()){
    _pf.SINT(0);
  }
  else{
    _pf.INT(0);
  }
}

void mml::postfix_writer::do_sizeof_node(mml::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _pf.INT(node->expression()->type()->size());
}

void mml::postfix_writer::do_index_node(mml::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->ptr()->accept(this, lvl);
  node->index()->accept(this,lvl);

  if(node->ptr()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.INT(3);
    _pf.SHTL();
    _pf.ADD();
  } 
  else { 
    _pf.INT(2);
    _pf.SHTL();
    _pf.ADD();
  }
}

void mml::postfix_writer::do_mem_alloc_node(mml::mem_alloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->argument()->accept(this, lvl);
  _pf.INT(3);
  _pf.SHTL();
  _pf.ALLOC(); // allocate
  _pf.SP(); // put base pointer in stack
}

void mml::postfix_writer::do_address_node(mml::address_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->lvalue()->accept(this, lvl + 2);
}

void mml::postfix_writer::do_variable_decl_node(mml::variable_decl_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto id = node->identifier();
  int typesize = node->type()->size();
  setGlobal(true);

  if (node->initializer() == nullptr) { //not initialized
    _pf.BSS();
    _pf.GLOBAL(id, _pf.OBJ());
    _pf.ALIGN();
    _pf.LABEL(id);
    _pf.SALLOC(typesize);
  }
  else { //initialized
    _pf.DATA();
    _pf.GLOBAL(id, _pf.OBJ());
    _pf.ALIGN();
    _pf.LABEL(id);

    if(node->is_typed(cdk::TYPE_DOUBLE) && node->initializer()->is_typed(cdk::TYPE_INT)){
      cdk::integer_node *dclini = dynamic_cast<cdk::integer_node*>(node->initializer());
      cdk::double_node ddi (dclini->lineno(), dclini->value());
      ddi.accept(this, lvl);
    } 
    else{
      node->initializer()->accept(this, lvl);
    }

    if(node->is_typed(cdk::TYPE_DOUBLE)){
      node->initializer()->accept(this, lvl);
    }
    else if(node->is_typed(cdk::TYPE_INT)){
      node->initializer()->accept(this, lvl);
    }
    else if(node->is_typed(cdk::TYPE_POINTER)){
      node->initializer()->accept(this, lvl);
    } 
    else if (node->is_typed(cdk::TYPE_STRING)) {
        node->initializer()->accept(this, lvl);
    } 
    else {
      std::cerr << node->lineno() << ": '" << id << "' has unexpected initializer\n";
    }
  }  
}

void mml::postfix_writer::do_func_definition_node(mml::func_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  auto id = node->identifier();
  std::shared_ptr<mml::symbol> symb = _symtab.find(id);
  int typesize = node->type()->size();

  if(symb != nullptr && isGlobal()) {
    _pf.GLOBAL(id, _pf.OBJ());
    _pf.BSS();
    _pf.ALIGN();
    _pf.LABEL(id);
    _pf.SBYTE(typesize);
  }

  
}

void mml::postfix_writer::do_func_call_node(mml::func_call_node * const node, int lvl) {
  // EMPTY
}
