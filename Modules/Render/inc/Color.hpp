#include <cstdint>

namespace nate::Modules::Render {
    class Colour {
      public:
      private:
        union {
            std::uint8_t  Bytes[4];
            std::uint32_t ColorVal;
        } CololValUnion;

      public:
        Colour(std::uint32_t val) { CololValUnion.ColorVal = val; }
        Colour() { CololValUnion.ColorVal = 0; }

        std::uint32_t RawValue() const { return CololValUnion.ColorVal; }
        std::uint8_t  Byte0() const { return CololValUnion.Bytes[0]; }
        std::uint8_t  Byte1() const { return CololValUnion.Bytes[1]; }
        std::uint8_t  Byte2() const { return CololValUnion.Bytes[2]; }
        std::uint8_t  Byte3() const { return CololValUnion.Bytes[3]; }

        void RawValue(std::uint32_t value) { CololValUnion.ColorVal = value; }
        void Byte0(std::uint8_t value) { CololValUnion.Bytes[0] = value; }
        void Byte1(std::uint8_t value) { CololValUnion.Bytes[1] = value; }
        void Byte2(std::uint8_t value) { CololValUnion.Bytes[2] = value; }
        void Byte3(std::uint8_t value) { CololValUnion.Bytes[3] = value; }
    };

} // namespace nate::Modules::Render