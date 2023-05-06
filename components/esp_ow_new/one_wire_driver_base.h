namespace esphome {
namespace onewire_bus {

class OneWireDriverBase {
public:
  virtual void init() = 0;
  virtual void read() = 0;
  virtual void write() = 0;
  virtual void setupA() = 0;
  virtual void setupB() = 0;
  virtual void modeA() = 0;
  virtual void modeB() = 0;
};


}  // namespace onewire_bus
}  // namespace esphome
