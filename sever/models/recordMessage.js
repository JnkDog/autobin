import mongoose from 'mongoose';

/* 
 * A basic table for a record
 * The binCode is a code for
 * finding the trash location
*/

const recordSchema = mongoose.Schema({
    name: String,
    type: String,
    binCode: String,
    location: String,
    coordinate: Array,
    accuracy: Number,
    createdAt: {
        type: Date,
        default: new Date(),
    },
})

const RecordMessage = mongoose.model('RecordMessage', recordSchema);

export default RecordMessage;