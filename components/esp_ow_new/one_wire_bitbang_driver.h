#include "one_wire_driver_base.h"

namespace esphome {
namespace onewire_bus {

class OneWireBitbangDriver : public OneWireDriverBase {
public:
  void init() override;
  void read() override;
  void write() override;
  void setupA();
  void setupB();
  void modeA();
  void modeB();
};

}  // namespace onewire_bus
}  // namespace esphome