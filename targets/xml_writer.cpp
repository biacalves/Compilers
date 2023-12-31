#include <string>
#include <iostream>
#include <memory>
#include <cdk/types/types.h>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "targets/symbol.h"
#include ".auto/all_nodes.h"  // automatically generated

//---------------------------------------------------------------------------

void mml::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void mml::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  openTag(node, lvl);
  closeTag(node, lvl);
}
void mml::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}
void mml::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}
void mml::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void mml::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_unary_operation(cdk::unary_operation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void mml::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_binary_operation(cdk::binary_operation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void mml::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}
void mml::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_operation(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void mml::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void mml::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_program_node(mml::program_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("declarations", lvl + 2);
  if(node->declarations() != nullptr) { node->declarations()->accept(this, lvl + 4);}
  closeTag("declarations", lvl + 2);
  openTag("instrs", lvl + 2); 
  if(node->instructions() != nullptr) { node->instructions()->accept(this, lvl + 4);}
  closeTag("instrs", lvl + 2); 
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_evaluation_node(mml::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void mml::xml_writer::do_print_node(mml::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label()
  << " new_line='" << node->is_newline() << "'>" << std::endl;

  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_while_node(mml::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_if_node(mml::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void mml::xml_writer::do_if_else_node(mml::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void mml::xml_writer::do_stop_node(mml::stop_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label()
  << " level='" << node->level() << "'>" << std::endl;

  closeTag(node, lvl);
}

void mml::xml_writer::do_next_node(mml::next_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label()
  << " level='" << node->level() << "'>" << std::endl;

  closeTag(node, lvl);

}

void mml::xml_writer::do_return_node(mml::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if (node->retval() != nullptr) node->retval()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void mml::xml_writer::do_block_node(mml::block_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("declarations", lvl);
  if (node->declarations()) {
    node->declarations()->accept(this, lvl + 4);
  }
  closeTag("declarations", lvl);

  openTag("instructions", lvl);
  if (node->instructions()) {
    node->instructions()->accept(this, lvl + 4);
  }
  closeTag("instructions", lvl);

  closeTag(node, lvl);
}

void mml::xml_writer::do_input_node(mml::input_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void mml::xml_writer::do_null_node(mml::null_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void mml::xml_writer::do_sizeof_node(mml::sizeof_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->expression()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void mml::xml_writer::do_index_node(mml::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  openTag("ptr", lvl + 2);
  if(node->ptr() != nullptr){
    node->ptr()->accept(this, lvl + 4);
  }
  closeTag("ptr", lvl + 2);
  
  openTag("index", lvl + 2);
  if(node->index() != nullptr){
    node->index()->accept(this, lvl + 4);
  }
  closeTag("index", lvl + 2);

  closeTag(node, lvl);
}

void mml::xml_writer::do_mem_alloc_node(mml::mem_alloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  do_unary_operation(node, lvl);
}

void mml::xml_writer::do_address_node(mml::address_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void mml::xml_writer::do_variable_decl_node(mml::variable_decl_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if(node->type() == nullptr){
    os() << std::string(lvl, ' ') << "<" << node->label() << " public='" << node->isPublic() 
    << "' forward='" << node->isForward() << "' foreign='" << node->isForeign() << "' auto='" << node->isAuto() 
    << "' identifier='" << node->identifier() << "'>" << std::endl;
  }

  else {
  os() << std::string(lvl, ' ') << "<" << node->label() << " public='" << node->isPublic() 
    << "' forward='" << node->isForward() << "' foreign='" << node->isForeign() << "' auto='" << node->isAuto() 
    << "' type='" << cdk::to_string(node->type()) << "' identifier='" << node->identifier() << "'>" << std::endl;
  }
  
  openTag("initializer", lvl + 2);
  if(node->initializer() != nullptr){
    node->initializer()->accept(this, lvl + 4);
  }
  closeTag("initializer", lvl + 2);
  
  closeTag(node, lvl);
}

void mml::xml_writer::do_func_definition_node(mml::func_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if(node->type() == nullptr){
    os() << std::string(lvl, ' ') << "<" << node->label() << " public='" << node->isPublic() 
    << "' forward='" << node->isForward() << "' foreign='" << node->isForeign() << "' auto='" << node->isAuto() 
    << "' identifier='" << node->identifier() << "' return='" << cdk::to_string(node->type()) << "'>" << std::endl;
  }

  else {
  os() << std::string(lvl, ' ') << "<" << node->label() << " public='" << node->isPublic() 
    << "' forward='" << node->isForward() << "' foreign='" << node->isForeign() << "' auto='" << node->isAuto() 
    << "' type='" << cdk::to_string(node->type()) << "' identifier='" << node->identifier() 
    << "' return='" << cdk::to_string(node->type()) << "'>" << std::endl;
  }

  openTag("variables", lvl + 2);
  if(node->variables() != nullptr) { 
    node->variables()->accept(this, lvl + 4); 
  }
  closeTag("variables", lvl + 2);

  openTag("block", lvl + 2);
  if(node->block() != nullptr) { 
    node->block()->accept(this, lvl + 4); 
  }
  closeTag("block", lvl + 2);
  
  closeTag(node, lvl);
}


void mml::xml_writer::do_func_call_node(mml::func_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() 
  << " identifier='" << node->identifier() << "'>" << std::endl;

  openTag("arguments", lvl + 2);
  if(node->arguments() != nullptr) {
    node->arguments()->accept(this, lvl + 4);
  }
  closeTag("arguments", lvl + 2);

  closeTag(node, lvl);
}