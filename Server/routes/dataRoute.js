const express = require('express');
const router = express.Router();
const DeviceData = require('../models/DeviceData');

router.post('/data', async (req, res) => {
  try {
    const { deviceId, voltage, current, power } = req.body;

    const newData = new DeviceData({ deviceId, voltage, current, power });
    await newData.save();
    console.log("📤 Received data from device:", newData);
    res.status(201).json({ message: 'Data stored successfully' });
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: 'Failed to store data' });
  }
});

module.exports = router;
