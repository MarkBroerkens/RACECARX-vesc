// -*- mode:c++; fill-column: 100; -*-

#ifndef VESC_DRIVER_VESC_PACKET_H_
#define VESC_DRIVER_VESC_PACKET_H_

#include <string>
#include <vector>
#include <utility>

#include <boost/crc.hpp>
#include <boost/shared_ptr.hpp>

#include "vesc_driver/v8stdint.h"

namespace vesc_driver
{

typedef std::vector<uint8_t> Buffer;
typedef std::pair<Buffer::iterator, Buffer::iterator> BufferRange;
typedef std::pair<Buffer::const_iterator, Buffer::const_iterator> BufferRangeConst;

/** The raw frame for communicating with the VESC */
class VescFrame
{
public:
  virtual ~VescFrame() {}

  // getters
  virtual const Buffer& frame() const {return *frame_;}

  // VESC packet properties
  static const int VESC_MAX_PAYLOAD_SIZE = 1024;          ///< Maximum VESC payload size, in bytes
  static const int VESC_MIN_FRAME_SIZE = 5;               ///< Smallest VESC frame size, in bytes
  static const int VESC_MAX_FRAME_SIZE = 6 + VESC_MAX_PAYLOAD_SIZE; ///< Largest VESC frame size, in bytes
  static const unsigned int VESC_SOF_VAL_SMALL_FRAME = 2; ///< VESC start of "small" frame value
  static const unsigned int VESC_SOF_VAL_LARGE_FRAME = 3; ///< VESC start of "large" frame value
  static const unsigned int VESC_EOF_VAL = 3;             ///< VESC end-of-frame value

  /** CRC parameters for the VESC */
  typedef boost::crc_optimal<16, 0x1021, 0, 0, false, false> CRC;

protected:
  /** Construct frame with specified payload size. */
  VescFrame(int payload_size);

  boost::shared_ptr<Buffer> frame_; ///< Stores frame data, shared_ptr for shallow copy
  BufferRange payload_;             ///< View into frame's payload section

private:
  /** Construct from buffer. Used by VescPacketFactory factory. */
  VescFrame(const BufferRangeConst& frame, const BufferRangeConst& payload);

  /** Give VescPacketFactory access to private constructor. */
  friend class VescPacketFactory;
};

/*------------------------------------------------------------------------------------------------*/

/** A VescPacket is a VescFrame with a non-zero length payload */
class VescPacket : public VescFrame
{
public:
  virtual ~VescPacket() {}

  virtual const std::string& name() const {return name_;}

protected:
  VescPacket(const std::string& name, int payload_size, int payload_id);
  VescPacket(const std::string& name, boost::shared_ptr<VescFrame> raw);

private:
  std::string name_;
};

typedef boost::shared_ptr<VescPacket> VescPacketPtr;
typedef boost::shared_ptr<VescPacket const> VescPacketConstPtr;

/*------------------------------------------------------------------------------------------------*/

class VescPacketFWVersion : public VescPacket
{
public:
  VescPacketFWVersion(boost::shared_ptr<VescFrame> raw);

  int fwMajor() const;
  int fwMinor() const;

};

class VescPacketRequestFWVersion : public VescPacket
{
public:
  VescPacketRequestFWVersion();

};

/*------------------------------------------------------------------------------------------------*/

class VescPacketValues : public VescPacket
{
public:
  VescPacketValues(boost::shared_ptr<VescFrame> raw);

  double temp_fet() const;
  double temp_motor() const;
  double avg_motor_current() const;
  double avg_input_current() const;
  double avg_id() const;
  double avg_iq() const;
  double duty_cycle_now() const;
  double rpm() const;
  double GET_INPUT_VOLTAGE() const;
  double amp_hours() const;
  double amp_hours_charged() const;
  double watt_hours() const;
  double watt_hours_charged() const;
  int32_t tachometer() const;
  int32_t tachometer_abs() const;
  int fault_code() const;
  double pid_pos_now() const;
  int controller_id() const;
  double NTC_TEMP_MOS1() const;
  double NTC_TEMP_MOS2() const;
  double NTC_TEMP_MOS3() const;
  double avg_vd() const;
  double avg_vq() const;

};

class VescPacketRequestValues : public VescPacket
{
public:
  VescPacketRequestValues();
};

/*------------------------------------------------------------------------------------------------*/

class VescPacketSetDuty : public VescPacket
{
public:
  VescPacketSetDuty(double duty);

  //  double duty() const;
};

/*------------------------------------------------------------------------------------------------*/

class VescPacketSetCurrent : public VescPacket
{
public:
  VescPacketSetCurrent(double current);

  //  double current() const;
};

/*------------------------------------------------------------------------------------------------*/

class VescPacketSetCurrentBrake : public VescPacket
{
public:
  VescPacketSetCurrentBrake(double current_brake);

  //  double current_brake() const;
};

/*------------------------------------------------------------------------------------------------*/

class VescPacketSetRPM : public VescPacket
{
public:
  VescPacketSetRPM(double rpm);

  //  double rpm() const;
};

/*------------------------------------------------------------------------------------------------*/

class VescPacketSetPos : public VescPacket
{
public:
  VescPacketSetPos(double pos);

  //  double pos() const;
};

/*------------------------------------------------------------------------------------------------*/

class VescPacketSetServoPos : public VescPacket
{
public:
  VescPacketSetServoPos(double servo_pos);

  //  double servo_pos() const;
};

class VescPacketSetHandbrake : public VescPacket
{
public:
  VescPacketSetHandbrake(double handbrake);

  //  double handbrake() const;
};

class VescPacketCommRotorPosition : public VescPacket
{
public:
  VescPacketCommRotorPosition(boost::shared_ptr<VescFrame> raw);

  int32_t comm_rotor_postion() const;
};
} // namespace vesc_driver

#endif // VESC_DRIVER_VESC_PACKET_H_