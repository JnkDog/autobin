import express from 'express';

import { getRecords, addRecord, getRecordsWithLocation, getRecordsGroupByType } from '../controllers/records.js';

const router = express.Router();

router.get('/', getRecords);
router.post('/', addRecord);
router.get('/datawithmap', getRecordsWithLocation);
router.get('/piechart', getRecordsGroupByType);   

export default router;