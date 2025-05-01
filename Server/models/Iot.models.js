// models/Reading.js
const mongoose = require('mongoose');

const readingSchema = new mongoose.Schema({
  voltage: {
    type: Number,
    required: true
  },
  current: {
    type: Number,
    required: true
  },
  time: {
    type: String, // e.g., "14:35"
    required: true
  },
  date: {
    type: String, // e.g., "2025-05-01"
    required: true
  }
});

const Reading = mongoose.model('Reading', readingSchema);

module.exports = Reading;
