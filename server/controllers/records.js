import express from 'express';

import RecordMessage from '../models/recordMessage.js';

const router = express.Router();

export const getRecords = async (req, res) => { 
    try {
        const recordsMessages = await RecordMessage.find().sort({createdAt: -1});
                
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
    const { name, type, binCode, accuracy, location, coordinate, numType } = req.body;

    const newRecord = new RecordMessage({ name, type, binCode, accuracy , location, coordinate, numType })

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

export const getRecordsGroupByType = async(req, res) => {
    try {
        const recycleCount = await RecordMessage.find({numType: 0}).count();
        const ResidualCount = await RecordMessage.find({numType: 1}).count();
        const FoodCount = await RecordMessage.find({numType: 2}).count();
        const HazardousCount = await RecordMessage.find({numType: 3}).count();

        const result = [
            {
                type: 'Recyclable Waste',
                value: recycleCount,
            },
            {
                type: 'Residual Waste',
                value: ResidualCount,
            },
            {
                type: 'Food Waste',
                value: FoodCount,
            },
            {
                type: 'Hazardous Waste',
                value: HazardousCount,
            }
        ];

        res.status(200).json(result);
    } catch (error) {
        res.status(404).json({ message: error.message });
    }
}

export default router;