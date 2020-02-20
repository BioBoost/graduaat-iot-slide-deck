function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};

  if (port === 1) {
    decoded.temperature = ((bytes[0] << 8) + bytes[1])/100.0;
  }

  return decoded;
}