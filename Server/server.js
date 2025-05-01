const express = require('express');
const mongoose = require('mongoose');
const cors = require('cors');

const app = express();

app.use(cors());
app.use(express.json());

// MongoDB connection
mongoose.connect('mongodb://localhost:27017/espdata', {
  useNewUrlParser: true,
  useUnifiedTopology: true,
});

const db = mongoose.connection;
db.on('error', console.error.bind(console, 'MongoDB error:'));
db.once('open', () => console.log('✅ Connected to MongoDB'));

// Schema
const DataSchema = new mongoose.Schema({
  command: Number,
  result: Number,
  timestamp: { type: Date, default: Date.now }
});
const DataModel = mongoose.model('Data', DataSchema);

// Variable to hold the latest command
let latestCommand = null;

// 1. Endpoint to receive command from frontend
app.post('/set-command', (req, res) => {
  const { command } = req.body;
  latestCommand = command;
  console.log("📥 Received command from frontend:", latestCommand);
  res.json({ status: 'Command received' });
});

// 2. Endpoint ESP32 hits to get the latest command
app.get('/get-command', (req, res) => {
  res.send(latestCommand !== null ? latestCommand.toString() : '');
});

// 3. Endpoint ESP32 posts incremented value
app.post('/esp-data', async (req, res) => {
  const { sensorValue } = req.body;
  console.log("📤 Received incremented value from ESP32:", sensorValue);

  try {
    const newData = new DataModel({
      command: latestCommand,
      result: sensorValue
    });
    await newData.save();
    res.json({ status: 'Data saved' });
  } catch (error) {
    res.status(500).json({ error: 'Failed to save data' });
  }
});

// 4. Clear the command after processing
app.post('/clear-command', (req, res) => {
  latestCommand = null;
  console.log("🧹 Cleared command");
  res.json({ status: 'Command cleared' });
});
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
  console.log(`🚀 Server running at http://localhost:${PORT}`);
});
