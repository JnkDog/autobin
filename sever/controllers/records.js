import express from 'express';

import RecordMessage from '../models/recordMessage.js';

const router = express.Router();

export const getRecords = async (req, res) => { 
    try {
        const recordsMessages = await RecordMessage.find();
                
        res.status(200).json(recordsMessages);
    } catch (error) {
        res.status(404).json({ message: error.message });
    }
}

export const getRecord = async (req, res) => { 
    const { id } = req.params;

    try {
        const post = await RecordMessage.findById(id);
        
        res.status(200).json(post);
    } catch (error) {
        res.status(404).json({ message: error.message });
    }
}

export const addRecord = async (req, res) => {
    const { name, type, binCode, accuracy } = req.body;

    const newRecord = new RecordMessage({ name, type, binCode, accuracy })

    try {
        await newRecord.save();

        res.status(201).json(newRecord);
    } catch (error) {
        res.status(409).json({ message: error.message });
    }
}

export const getRecordsWithLocation = async(req, res) => {
    try {
        // need change and test
        const recordsWithMap = await RecordMessage.find();
                
        res.status(200).json(recordsWithMap);
    } catch (error) {
        res.status(404).json({ message: error.message });
    }
}

export default router;