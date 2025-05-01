const mongoose = require('mongoose');

const deviceDataSchema = new mongoose.Schema({
  deviceId: { type: String, required: true },
  voltage: Number,
  current: Number,
  power: Number,
  timestamp: { type: Date, default: Date.now }
});

module.exports = mongoose.model('DeviceData', deviceDataSchema);
