import express from 'express';

import { getRecords, addRecord, getRecordsWithLocation } from '../controllers/records.js';

const router = express.Router();

router.get('/', getRecords);
router.post('/', addRecord);
router.get('/datawithmap', getRecordsWithLocation);   

export default router;