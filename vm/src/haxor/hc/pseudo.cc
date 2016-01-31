#include <utility>
#include "haxor/hc/pseudo.hh"

namespace haxor {
  pseudo::pseudo() {
    ast = new std::vector<node::base*>();

    instr2func.insert(std::make_pair("push",  &pseudo::p_push));
    instr2func.insert(std::make_pair("pushi", &pseudo::p_pushi));
    instr2func.insert(std::make_pair("pushm", &pseudo::p_pushm));
    instr2func.insert(std::make_pair("pop",   &pseudo::p_pop));
    instr2func.insert(std::make_pair("popm",  &pseudo::p_popm));
    instr2func.insert(std::make_pair("move",  &pseudo::p_move));
    instr2func.insert(std::make_pair("clear", &pseudo::p_clear));
    instr2func.insert(std::make_pair("not",   &pseudo::p_not));
    instr2func.insert(std::make_pair("ret",   &pseudo::p_ret));
    instr2func.insert(std::make_pair("prol",  &pseudo::p_prol));
    instr2func.insert(std::make_pair("epil",  &pseudo::p_epil));
    instr2func.insert(std::make_pair("b",     &pseudo::p_b));
    instr2func.insert(std::make_pair("bal",   &pseudo::p_bal));
    instr2func.insert(std::make_pair("bgt",   &pseudo::p_bgt));
    instr2func.insert(std::make_pair("blt",   &pseudo::p_blt));
    instr2func.insert(std::make_pair("bge",   &pseudo::p_bge));
    instr2func.insert(std::make_pair("ble",   &pseudo::p_ble));
    instr2func.insert(std::make_pair("blez",  &pseudo::p_blez));
    instr2func.insert(std::make_pair("bgtz",  &pseudo::p_bgtz));
    instr2func.insert(std::make_pair("beqz",  &pseudo::p_beqz));
  }

  void pseudo::process(std::vector<node::base*> *input) {
    for (auto *item : *input) {
      if (item->get_type() == node::type::instr) {
        auto *instr = dynamic_cast<node::instr*>(item);
        auto it = instr2func.find(instr->get_name());

        if (it != instr2func.end()) {
          auto func = std::bind(it->second, this, instr);
          func();
        } else {
          ast->push_back(item);
        }
      } else {
        ast->push_back(item);
      }
    }
  }

  std::vector<node::base*> *pseudo::get_ast() {
    return ast;
  }

  void pseudo::p_push(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(-sizeof(word_t)));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node::instr("sw", args));
    }
  }

  void pseudo::p_pushi(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(-sizeof(word_t)));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      args->push_back(new node::reg("$at"));
      ast->push_back(new node::instr("sw", args));
    }
  }

  void pseudo::p_pushm(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node::instr("lw", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(-sizeof(word_t)));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      args->push_back(new node::reg("$at"));
      ast->push_back(new node::instr("sw", args));
    }
  }

  void pseudo::p_pop(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      ast->push_back(new node::instr("lw", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(sizeof(word_t)));
      ast->push_back(new node::instr("addi", args));
    }
  }

  void pseudo::p_popm(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      ast->push_back(new node::instr("lw", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(0));
      args->push_back(new node::reg("$at"));
      ast->push_back(new node::instr("sw", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(sizeof(word_t)));
      ast->push_back(new node::instr("addi", args));
    }
  }

  void pseudo::p_move(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(input->get_args()->at(1));
      args->push_back(new node::reg("$zero"));
      ast->push_back(new node::instr("add", args));
    }
  }

  void pseudo::p_clear(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(new node::reg("$zero"));
      args->push_back(new node::reg("$zero"));
      ast->push_back(new node::instr("add", args));
    }
  }

  void pseudo::p_not(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(input->get_args()->at(1));
      args->push_back(new node::reg("$zero"));
      ast->push_back(new node::instr("nor", args));
    }
  }

  void pseudo::p_ret(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$ra"));
      ast->push_back(new node::instr("jr", args));
    }
  }

  void pseudo::p_prol(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(- sizeof(word_t)));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      args->push_back(new node::reg("$ra"));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(- sizeof(word_t)));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      args->push_back(new node::reg("$fp"));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$fp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      auto arg1 = dynamic_cast<node::num*>(input->get_args()->at(0));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(arg1->get_value()));
      ast->push_back(new node::instr("addi", args));
    }
  }

  void pseudo::p_epil(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$fp"));
      args->push_back(new node::num(0));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$fp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      ast->push_back(new node::instr("lw", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(sizeof(word_t)));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$ra"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(0));
      ast->push_back(new node::instr("lw", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::reg("$sp"));
      args->push_back(new node::num(sizeof(word_t)));
      ast->push_back(new node::instr("addi", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$ra"));
      ast->push_back(new node::instr("jr", args));
    }
  }

  void pseudo::p_b(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$zero"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node::instr("beq", args));
    }
  }

  void pseudo::p_bal(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$zero"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node::instr("beql", args));
    }
  }

  void pseudo::p_bgt(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(input->get_args()->at(1));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node::instr("slt", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(2));
      ast->push_back(new node::instr("bne", args));
    }
  }

  void pseudo::p_blt(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(input->get_args()->at(0));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node::instr("slt", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(2));
      ast->push_back(new node::instr("bne", args));
    }
  }

  void pseudo::p_bge(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(input->get_args()->at(0));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node::instr("slt", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(2));
      ast->push_back(new node::instr("beq", args));
    }
  }

  void pseudo::p_ble(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(input->get_args()->at(1));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node::instr("slt", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(2));
      ast->push_back(new node::instr("beq", args));
    }
  }

  void pseudo::p_blez(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node::instr("slt", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node::instr("beq", args));
    }
  }

  void pseudo::p_bgtz(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node::instr("slt", args));
    }
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(new node::reg("$at"));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node::instr("beq", args));
    }
  }

  void pseudo::p_beqz(node::instr *input) {
    {
      auto args = new std::vector<class node::base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(new node::reg("$zero"));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node::instr("beq", args));
    }
  }
}