function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};

  if (port === 1) {
    decoded.battery = {};
    decoded.battery.temperature = ((bytes[0] << 8) + bytes[1])/100.0;
    decoded.battery.percentage = bytes[2];
    decoded.battery.is_healthy = (bytes[3] !== 0);
    decoded.battery.voltage = ((bytes[4] << 8) + bytes[5])/1000.0;
    decoded.cooling_water_level = bytes[6];
    decoded.movement = {};
    decoded.movement.is_moving = (bytes[6] == 0xFF);
    decoded.movement.speed = ((bytes[8] << 8) + bytes[9]);
    decoded.controller = {};
    decoded.controller.send_messages = ((bytes[10] << 24) + (bytes[11] << 16) + (bytes[12] << 8) + bytes[13]);
    decoded.controller.failed_messages = ((bytes[14] << 24) + (bytes[15] << 16) + (bytes[16] << 8) + bytes[17]);
    decoded.firmware_id = bytes[18];
  }

  return decoded;
}