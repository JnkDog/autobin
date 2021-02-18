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
    const { title, message, selectedFile, creator, tags } = req.body;

    const newPostMessage = new PostMessage({ title, message, selectedFile, creator, tags })

    try {
        await newPostMessage.save();

        res.status(201).json(newPostMessage );
    } catch (error) {
        res.status(409).json({ message: error.message });
    }
}

export default router;