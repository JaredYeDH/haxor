#ifndef HAXOR_CPU_HH
#define HAXOR_CPU_HH

#include "haxor/common/haxor.hh"
#include "haxor/hvm/regs.hh"
#include "haxor/common/opcode.hh"

namespace haxor {
  class vm;

  enum class memory_segment {
    ivt,
    code,
    data,
    stack
  };

  class cpu {
    public:
    explicit cpu(class vm &vm);
    void cycle();
    void execute(const opcode_t &op);
    void set_ip(const uint64_t ip);
    memory_segment determine_segment(const word_t addr);
    class regs& get_regs();

    private:
    void link();
    void branch(const uint64_t target);
    void jump(const uint64_t target);
    void validate_reg_write(const uint8_t reg);

    class vm &vm;
    class regs regs;
    opcode_t opcode;
    uint64_t ip;
  };
}

#endif
