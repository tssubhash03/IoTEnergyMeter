const express = require('express');
const app = express();
app.use(express.json());
const cors = require('cors');
const mongoose = require('mongoose');
const dotenv = require('dotenv');
const Reading = require('./models/Iot.models.js'); // Adjust the path as necessary
const path = require('path');
dotenv.config();

// Enable CORS
app.use(cors());

// MongoDB connection with updated options to avoid deprecation warnings
const mongoURI = process.env.MONGO_URL; // Ensure you have this in your .env file
mongoose
  .connect(mongoURI)
  .then(() => {
    console.log('Connected to MongoDB');
  })
  .catch((err) => {
    console.error('Error connecting to MongoDB:', err);
  });

// Example route
app.get('/', (req, res) => {
  res.send('Backend is running!');
});
app.post('/add-reading', async (req, res) => {
    try {
      const { voltage, current, time, date } = req.body;
  
      const reading = new Reading({ voltage, current, time, date });
      await reading.save();
  
      res.status(201).json({ message: 'Reading saved', reading });
    } catch (err) {
        console.error('Error saving reading:', err);
      res.status(500).json({ error: err.message });
    }
  });
// Start the server
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
