#ifndef HAXOR_COMPILER_HH
#define HAXOR_COMPILER_HH

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "haxor/hc/node/all.hh"
#include "haxor/hc/lexer.hh"
#include "haxor/common/cmd.hh"
#include "haxor/common/hdr.hh"
#include "parser.hh"

namespace haxor {
  class instr_spec {
    public:
    instr_spec(cmd cmd, std::string args, bool rel, bool x8) {
      this->cmd = cmd;
      this->args = args;
      this->rel = rel;
      this->x8 = x8;
    }
    instr_spec(cmd cmd, std::string args) {
      this->cmd = cmd;
      this->args = args;
      this->rel = false;
      this->x8 = false;
    }

    cmd cmd;
    std::string args;
    bool rel;
    bool x8;
  };

  class compiler {
    friend class parser;

    public:
    compiler();
    void compile();

    private:
    void set_ast(std::vector<node::base*> *ast);

    void verify_code();
    void unwind_pseudo();
    void mark_sections();
    void order_sections();
    void calc_addresses();
    void collect_labels();
    hdr_t build_hdr();
    int64_t calc_bss_size();
    uint8_t resolve_reg(const std::string &name);

    void increase_location(unsigned int loc); //
    unsigned int get_location() const; //

    lexer lexer;
    parser parser;

    unsigned int location; //

    std::vector<node::base*> *ast;
    std::map<std::string, word_t> labels;

    std::map<std::string, uint8_t> reg_aliases = {
      { "$zero", 0 }, { "$at",  1 },
      { "$a0",   2 }, { "$a1",  3 }, { "$a2",  4 }, { "$a3",  5 }, { "$a4",  6 },
      { "$a5",   7 }, { "$a6",  8 }, { "$a7",  9 }, { "$a8", 10 }, { "$a9", 11 },
      { "$t0",  12 }, { "$t1", 13 }, { "$t2", 14 }, { "$t3", 15 }, { "$t4", 16 },
      { "$t5",  17 }, { "$t6", 18 }, { "$t7", 19 }, { "$t8", 20 }, { "$t9", 21 },
      { "$sp",  61 }, { "$ra", 62 }, { "$sc", 63 }
    };

    std::map<std::string, instr_spec> instr_list = {
      { "nop",     instr_spec(cmd_nop,     "") },
      { "exiti",   instr_spec(cmd_exiti,   "i") },
      { "syscall", instr_spec(cmd_syscall, "") },

      { "add",     instr_spec(cmd_add,     "rrr") },
      { "addi",    instr_spec(cmd_addi,    "rri") },
      { "sub",     instr_spec(cmd_sub,     "rrr") },
      { "mult",    instr_spec(cmd_mult,    "rrr") },
      { "div",     instr_spec(cmd_div,     "rrr") },
      { "mod",     instr_spec(cmd_mod,     "rrr") },

      { "lw",      instr_spec(cmd_lw,      "rri") },
      { "sw",      instr_spec(cmd_sw,      "rir") },
      { "lui",     instr_spec(cmd_lui,     "ri") },

      { "and",     instr_spec(cmd_and,     "rrr") },
      { "andi",    instr_spec(cmd_andi,    "rri") },
      { "or",      instr_spec(cmd_or,      "rrr") },
      { "ori",     instr_spec(cmd_ori,     "rri") },
      { "xor",     instr_spec(cmd_xor,     "rrr") },
      { "nor",     instr_spec(cmd_nor,     "rrr") },
      { "slt",     instr_spec(cmd_slt,     "rrr") },
      { "slti",    instr_spec(cmd_slti,    "rri") },

      { "slli",    instr_spec(cmd_slli,    "rri") },
      { "srli",    instr_spec(cmd_srli,    "rri") },
      { "sll",     instr_spec(cmd_sll,     "rrr") },
      { "srl",     instr_spec(cmd_srl,     "rrr") },

      { "beq",     instr_spec(cmd_beq,     "rri", true,  true) },
      { "beql",    instr_spec(cmd_beql,    "rri", true,  true) },
      { "bne",     instr_spec(cmd_bne,     "rri", true,  true) },
      { "bnel",    instr_spec(cmd_bnel,    "rri", true,  true) },
      { "j",       instr_spec(cmd_j,       "i",   false, true) },
      { "jr",      instr_spec(cmd_jr,      "r") },
      { "jal",     instr_spec(cmd_jal,     "i",   false, true) },

      // pseudo

      { "push",    instr_spec(cmd_pseudo,  "r") },
      { "pushi",   instr_spec(cmd_pseudo,  "i") },
      { "pushm",   instr_spec(cmd_pseudo,  "i") },
      { "pop",     instr_spec(cmd_pseudo,  "r") },
      { "popm",    instr_spec(cmd_pseudo,  "i") },
      { "move",    instr_spec(cmd_pseudo,  "rr") },
      { "clear",   instr_spec(cmd_pseudo,  "r") },
      { "not",     instr_spec(cmd_pseudo,  "rr") },
      { "ret",     instr_spec(cmd_pseudo,  "") },
      { "b",       instr_spec(cmd_pseudo,  "i") },
      { "bal",     instr_spec(cmd_pseudo,  "i") },
      { "bgt",     instr_spec(cmd_pseudo,  "rri") },
      { "blt",     instr_spec(cmd_pseudo,  "rri") },
      { "bge",     instr_spec(cmd_pseudo,  "rri") },
      { "ble",     instr_spec(cmd_pseudo,  "rri") },
      { "blez",    instr_spec(cmd_pseudo,  "ri") },
      { "bgtz",    instr_spec(cmd_pseudo,  "ri") },
      { "beqz",    instr_spec(cmd_pseudo,  "ri") },
      { "prol",    instr_spec(cmd_pseudo,  "i") },
      { "epil",    instr_spec(cmd_pseudo,  "") }
    };
  };
}

#endif