import express from 'express';
import cors from 'cors';
import helmet from 'helmet';
import morgan from 'morgan';
import mongoose from 'mongoose';
import dotenv from 'dotenv';

dotenv.config();

const app = express();
const port = process.env.PORT || 4000;
const mongoUri = process.env.MONGODB_URI || 'mongodb://127.0.0.1:27017/pipemedic';

app.use(helmet());
// Allow requests from file:// (null origin) and local dev servers
app.use(cors({ origin: true, credentials: true }));
app.use(express.json({ limit: '1mb' }));
app.use(morgan('dev'));

const requiredString = { type: String, required: true, trim: true };

const BookingSchema = new mongoose.Schema({
  name: requiredString,
  email: { ...requiredString, lowercase: true },
  phone: requiredString,
  address: requiredString,
  issueType: requiredString,
  preferredDate: requiredString,
  description: requiredString,
}, { timestamps: true });

const OrderItemSchema = new mongoose.Schema({
  sku: requiredString,
  name: requiredString,
  qty: { type: Number, required: true, min: 1 },
  price: { type: Number, required: true, min: 0 }
}, { _id: false });

const OrderSchema = new mongoose.Schema({
  name: requiredString,
  email: { ...requiredString, lowercase: true },
  phone: requiredString,
  address: requiredString,
  city: requiredString,
  pincode: requiredString,
  items: { type: [OrderItemSchema], validate: v => Array.isArray(v) && v.length > 0 }
}, { timestamps: true });

const Booking = mongoose.models.Booking || mongoose.model('Booking', BookingSchema);
const Order = mongoose.models.Order || mongoose.model('Order', OrderSchema);

app.get('/health', (_req, res) => res.json({ ok: true }));

app.post('/api/bookings', async (req, res) => {
  try {
    const booking = new Booking(req.body);
    await booking.validate();
    const saved = await booking.save();
    res.status(201).json({ id: saved._id });
  } catch (err) {
    res.status(400).json({ message: err.message || 'Invalid booking' });
  }
});

app.post('/api/orders', async (req, res) => {
  try {
    const order = new Order(req.body);
    await order.validate();
    const saved = await order.save();
    res.status(201).json({ id: saved._id });
  } catch (err) {
    res.status(400).json({ message: err.message || 'Invalid order' });
  }
});

async function start() {
  try {
    await mongoose.connect(mongoUri, { dbName: process.env.MONGODB_DB || 'pipemedic' });
    console.log('Connected to MongoDB');
    app.listen(port, () => console.log(`API listening on :${port}`));
  } catch (e) {
    console.error('Failed to start', e);
    process.exit(1);
  }
}

start();


