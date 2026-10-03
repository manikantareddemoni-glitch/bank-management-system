const express = require('express');
const mongoose = require('mongoose');
const cors = require('cors');
const fs = require('fs');
const path = require('path');
const dns = require('dns');
dns.setServers(['8.8.8.8', '8.8.4.4']);
require('dotenv').config();

const app = express();
app.use(cors());
app.use(express.json());

const LOCAL_DATA_PATH = fs.existsSync(path.join(__dirname, '..', 'database', 'data.json'))
    ? path.join(__dirname, '..', 'database', 'data.json')
    : (fs.existsSync(path.join(__dirname, '..', '3_database', 'data.json'))
        ? path.join(__dirname, '..', '3_database', 'data.json')
        : path.join(__dirname, '..', 'data.json'));

// Default initial seed data for local storage
const defaultSeedData = {
    customers: [
        { id: 1, accountNo: 1001, name: 'Aarav Sharma',    phone: '9876543210', email: 'aarav.sharma@example.com',    accountType: 'SAVINGS', balanceRupees: '25000.50', balancePaise: 2500050, createdAt: '2026-01-01 10:00:00' },
        { id: 2, accountNo: 1002, name: 'Priya Patel',     phone: '9823456789', email: 'priya.patel@example.com',     accountType: 'SAVINGS', balanceRupees:  '1200.00', balancePaise:  120000, createdAt: '2026-01-01 10:05:00' },
        { id: 3, accountNo: 1003, name: 'Rohan Verma',     phone: '9712345678', email: 'rohan.verma@example.com',     accountType: 'CURRENT', balanceRupees: '75000.00', balancePaise: 7500000, createdAt: '2026-01-01 10:10:00' },
        { id: 4, accountNo: 1004, name: 'Ananya Iyer',     phone: '9601234567', email: 'ananya.iyer@example.com',     accountType: 'SAVINGS', balanceRupees:   '650.75', balancePaise:   65075, createdAt: '2026-01-01 10:15:00' },
        { id: 5, accountNo: 1005, name: 'Vikram Malhotra', phone: '9543210987', email: 'vikram.m@example.com',        accountType: 'CURRENT', balanceRupees: '150000.00', balancePaise: 15000000, createdAt: '2026-01-01 10:20:00' },
        { id: 6, accountNo: 1006, name: 'Sanya Gupta',     phone: '9432109876', email: 'sanya.gupta@example.com',    accountType: 'SAVINGS', balanceRupees:   '500.00', balancePaise:   50000, createdAt: '2026-01-01 10:25:00' }
    ],
    transactions: [
        { txn_id: 1, account_no: 1001, txn_type: 'OPENING', amount: '25000.50', balance_after: '25000.50', txn_time: '2026-01-01 10:00:00' },
        { txn_id: 2, account_no: 1002, txn_type: 'OPENING', amount:  '1200.00', balance_after:  '1200.00', txn_time: '2026-01-01 10:05:00' },
        { txn_id: 3, account_no: 1003, txn_type: 'OPENING', amount: '75000.00', balance_after: '75000.00', txn_time: '2026-01-01 10:10:00' },
        { txn_id: 4, account_no: 1004, txn_type: 'OPENING', amount:   '650.75', balance_after:   '650.75', txn_time: '2026-01-01 10:15:00' },
        { txn_id: 5, account_no: 1005, txn_type: 'OPENING', amount: '150000.00', balance_after: '150000.00', txn_time: '2026-01-01 10:20:00' },
        { txn_id: 6, account_no: 1006, txn_type: 'OPENING', amount:   '500.00', balance_after:   '500.00', txn_time: '2026-01-01 10:25:00' }
    ],
    nextId: 7,
    nextAccNo: 1007,
    nextTxnId: 7
};

function loadLocalData() {
    if (fs.existsSync(LOCAL_DATA_PATH)) {
        try {
            const raw = fs.readFileSync(LOCAL_DATA_PATH, 'utf8');
            return JSON.parse(raw);
        } catch (e) {
            console.error("Error reading local data.json, resetting:", e);
        }
    }
    fs.writeFileSync(LOCAL_DATA_PATH, JSON.stringify(defaultSeedData, null, 2));
    return defaultSeedData;
}

function saveLocalData(data) {
    fs.writeFileSync(LOCAL_DATA_PATH, JSON.stringify(data, null, 2));
}

let localData = loadLocalData();

// MongoDB Mongoose Schemas & Models
const CustomerSchema = new mongoose.Schema({
    id: { type: Number, required: true, unique: true },
    accountNo: { type: Number, required: true, unique: true },
    name: { type: String, required: true },
    phone: { type: String, required: true },
    email: { type: String, default: '' },
    accountType: { type: String, required: true },
    balanceRupees: { type: String, required: true },
    balancePaise: { type: Number, required: true },
    createdAt: { type: String, required: true }
}, { collection: 'customers', timestamps: true });

const TransactionSchema = new mongoose.Schema({
    txn_id: { type: Number, required: true, unique: true },
    account_no: { type: Number, required: true },
    txn_type: { type: String, required: true },
    amount: { type: String, required: true },
    balance_after: { type: String, required: true },
    txn_time: { type: String, required: true }
}, { collection: 'transactions', timestamps: true });

const UserSchema = new mongoose.Schema({
    username: { type: String, required: true, unique: true },
    password: { type: String, required: true },
    name: { type: String, required: true },
    role: { type: String, default: 'manager' },
    accountNo: { type: Number, default: 0 }
}, { collection: 'users', timestamps: true });

const CustomerModel = mongoose.model('Customer', CustomerSchema);
const TransactionModel = mongoose.model('Transaction', TransactionSchema);
const UserModel = mongoose.model('User', UserSchema);

function getConnectionString() {
    if (process.env.MONGODB_URI) {
        return process.env.MONGODB_URI;
    }
    const dbConfigPath = fs.existsSync(path.join(__dirname, '..', 'database', 'config.txt'))
        ? path.join(__dirname, '..', 'database', 'config.txt')
        : path.join(__dirname, '..', '3_database', 'config.txt');
    const configPath = fs.existsSync(dbConfigPath) ? dbConfigPath : path.join(__dirname, '..', 'config.txt');
    if (fs.existsSync(configPath)) {
        const lines = fs.readFileSync(configPath, 'utf8').split('\n');
        for (let line of lines) {
            line = line.trim();
            if (line && !line.startsWith('#') && (line.startsWith('mongodb://') || line.startsWith('mongodb+srv://'))) {
                return line;
            }
        }
    }
    return "mongodb+srv://System:System@cluster0.n9s4euu.mongodb.net/apex_bank?retryWrites=true&w=majority";
}

async function onMongoDBConnected() {
    try {
        const count = await CustomerModel.countDocuments();
        if (count === 0 && localData.customers.length > 0) {
            console.log("Seeding MongoDB with local data.json records...");
            await CustomerModel.insertMany(localData.customers);
            await TransactionModel.insertMany(localData.transactions);
        } else if (count > 0) {
            const dbCusts = await CustomerModel.find().sort({ accountNo: 1 }).lean();
            const dbTxns = await TransactionModel.find().sort({ txn_id: 1 }).lean();
            localData.customers = dbCusts;
            localData.transactions = dbTxns;
            const maxId = dbCusts.reduce((max, c) => Math.max(max, c.id || 0), 0);
            const maxAcc = dbCusts.reduce((max, c) => Math.max(max, c.accountNo || 1000), 1000);
            const maxTxn = dbTxns.reduce((max, t) => Math.max(max, t.txn_id || 0), 0);
            localData.nextId = maxId + 1;
            localData.nextAccNo = maxAcc + 1;
            localData.nextTxnId = maxTxn + 1;
            saveLocalData(localData);
        }
    } catch (e) {
        console.error("Error on MongoDB sync:", e.message);
    }
}

let isConnecting = false;

async function connectMongoDB() {
    if (mongoose.connection.readyState === 1) return true;
    if (isConnecting) return false;
    isConnecting = true;

    const connStr = getConnectionString();

    // Attempt 1: Standard Connection
    try {
        await mongoose.connect(connStr, { serverSelectionTimeoutMS: 8000 });
        console.log("Connected to MongoDB Atlas!");
        await onMongoDBConnected();
        isConnecting = false;
        return true;
    } catch (err1) {
        console.log("Standard DNS Connection attempt failed:", err1.message);
    }

    // Attempt 2: Fallback with Google & Cloudflare Public DNS
    try {
        try { dns.setServers(['8.8.8.8', '8.8.4.4', '1.1.1.1']); } catch(e) {}
        await mongoose.connect(connStr, { serverSelectionTimeoutMS: 8000 });
        console.log("Connected to MongoDB Atlas using Fallback DNS!");
        await onMongoDBConnected();
        isConnecting = false;
        return true;
    } catch (err2) {
        console.log("MongoDB Atlas not connected (Using Localhost data.json):", err2.message);
        isConnecting = false;
        return false;
    }
}

connectMongoDB();

// Auto-reconnect background heartbeat interval
setInterval(() => {
    if (mongoose.connection.readyState !== 1) {
        connectMongoDB();
    }
}, 10000);

function parseMoneyToPaise(inputStr) {
    if (typeof inputStr === 'number') inputStr = inputStr.toString();
    if (!inputStr) return null;
    let str = inputStr.trim();
    if (str.startsWith('-')) return null;
    let dotPos = str.indexOf('.');
    let rupeesStr = '', paiseStr = '';
    if (dotPos === -1) {
        rupeesStr = str;
        paiseStr = '00';
    } else {
        if (str.indexOf('.', dotPos + 1) !== -1) return null;
        rupeesStr = str.substring(0, dotPos);
        paiseStr = str.substring(dotPos + 1);
        if (paiseStr.length === 0) paiseStr = '00';
        else if (paiseStr.length === 1) paiseStr += '0';
        else if (paiseStr.length > 2) return null;
    }
    if (!rupeesStr) rupeesStr = '0';
    if (!/^\d+$/.test(rupeesStr) || !/^\d+$/.test(paiseStr)) return null;
    return Number((BigInt(rupeesStr) * 100n) + BigInt(paiseStr));
}

function formatPaiseToRupees(paiseVal) {
    const isNeg = paiseVal < 0;
    const absPaise = Math.abs(paiseVal);
    const rupees = Math.floor(absPaise / 100);
    const remainingPaise = absPaise % 100;
    const paiseFormatted = remainingPaise < 10 ? '0' + remainingPaise : remainingPaise;
    return `${isNeg ? '-' : ''}${rupees}.${paiseFormatted}`;
}

// REST API Endpoints

app.get('/api/health', async (req, res) => {
    const isConnected = mongoose.connection.readyState === 1;
    if (isConnected) {
        return res.json({
            connected: true,
            mode: 'MongoDB Atlas Active',
            message: 'Connected to MongoDB Atlas Cluster'
        });
    } else {
        return res.json({
            connected: false,
            mode: 'Local Data Active (data.json)',
            message: 'Local Persistence Active (data.json).'
        });
    }
});

app.post('/api/reconnect', async (req, res) => {
    const success = await connectMongoDB();
    if (success) {
        return res.json({ connected: true, message: 'Connected to MongoDB Atlas Cluster' });
    } else {
        return res.status(500).json({ connected: false, message: 'MongoDB reconnection attempt failed.' });
    }
});

// Authentication Endpoints
app.post('/api/auth/login', async (req, res) => {
    const { username, password } = req.body;
    if (!username || !password) {
        return res.status(400).json({ error: 'Username and password are required.' });
    }

    const uLower = username.trim().toLowerCase();

    // Default admin fallback check
    if ((uLower === 'admin' || uLower === 'manager') && password === 'admin123') {
        return res.json({
            success: true,
            user: { username: 'admin', name: 'System Bank Manager', role: 'manager', accountNo: 0 }
        });
    }

    // Default customer demo fallback check
    if (uLower === '1001' && password === 'demo123') {
        return res.json({
            success: true,
            user: { username: '1001', name: 'Aarav Sharma', role: 'customer', accountNo: 1001 }
        });
    }

    if (mongoose.connection.readyState === 1) {
        try {
            const foundUser = await UserModel.findOne({ username: new RegExp('^' + uLower + '$', 'i') }).lean();
            if (foundUser && foundUser.password === password) {
                return res.json({
                    success: true,
                    user: { username: foundUser.username, name: foundUser.name, role: foundUser.role, accountNo: foundUser.accountNo || 0 }
                });
            }
        } catch (err) {
            console.error("Auth DB query error:", err.message);
        }
    }

    localData = loadLocalData();
    if (!localData.users) {
        localData.users = [
            { username: 'admin', password: 'admin123', name: 'System Bank Manager', role: 'manager', accountNo: 0 },
            { username: '1001', password: 'demo123', name: 'Aarav Sharma', role: 'customer', accountNo: 1001 }
        ];
        saveLocalData(localData);
    }

    const localUser = localData.users.find(u => u.username.toLowerCase() === uLower && u.password === password);
    if (localUser) {
        return res.json({
            success: true,
            user: { username: localUser.username, name: localUser.name, role: localUser.role, accountNo: localUser.accountNo || 0 }
        });
    }

    return res.status(401).json({ error: 'Invalid username or password.' });
});

app.post('/api/auth/register', async (req, res) => {
    const { username, password, name, role, accountNo } = req.body;
    if (!username || !password || !name) {
        return res.status(400).json({ error: 'Username, password, and name are required.' });
    }

    const uTrim = username.trim();
    localData = loadLocalData();
    if (!localData.users) localData.users = [];

    if (localData.users.some(u => u.username.toLowerCase() === uTrim.toLowerCase())) {
        return res.status(400).json({ error: 'Username already taken.' });
    }

    const newUser = {
        username: uTrim,
        password: password.trim(),
        name: name.trim(),
        role: role || 'manager',
        accountNo: parseInt(accountNo, 10) || 0
    };

    localData.users.push(newUser);
    saveLocalData(localData);

    if (mongoose.connection.readyState === 1) {
        try {
            await UserModel.create(newUser);
        } catch (err) {
            console.error("MongoDB User Register error:", err.message);
        }
    }

    res.json({ success: true, user: { username: newUser.username, name: newUser.name, role: newUser.role, accountNo: newUser.accountNo } });
});

// Load All Customers
app.get('/api/customers', async (req, res) => {
    if (mongoose.connection.readyState === 1) {
        try {
            const dbCusts = await CustomerModel.find().sort({ accountNo: 1 }).lean();
            if (dbCusts.length > 0) {
                return res.json(dbCusts);
            }
        } catch (err) {
            console.log("Serving from Localhost data.json:", err.message);
        }
    }
    localData = loadLocalData();
    res.json(localData.customers);
});

// Add Customer Account
app.post('/api/customers', async (req, res) => {
    const { name, phone, email, accountType, openingDeposit } = req.body;

    if (!name || !phone || !accountType) {
        return res.status(400).json({ error: 'Name, phone, and accountType are required.' });
    }
    const cleanPhone = phone.trim();
    if (cleanPhone.length !== 10 || !/^\d+$/.test(cleanPhone)) {
        return res.status(400).json({ error: 'Phone number must be exactly 10 digits.' });
    }

    const openingPaise = parseMoneyToPaise(openingDeposit);
    if (openingPaise === null || openingPaise <= 0) {
        return res.status(400).json({ error: 'Invalid opening deposit amount.' });
    }

    if (accountType === 'SAVINGS' && openingPaise < 50000) {
        return res.status(400).json({ error: 'SAVINGS account requires an initial deposit of at least Rs. 500.00.' });
    }

    localData = loadLocalData();

    // Check duplicate phone in MongoDB Atlas if connected, or localData
    if (mongoose.connection.readyState === 1) {
        const existingDb = await CustomerModel.findOne({ phone: cleanPhone }).lean();
        if (existingDb) {
            return res.status(400).json({ error: 'Phone number already registered.' });
        }
    } else {
        if (localData.customers.some(c => c.phone === cleanPhone)) {
            return res.status(400).json({ error: 'Phone number already registered.' });
        }
    }

    let newId, newAccNo, newTxnId;
    if (mongoose.connection.readyState === 1) {
        const dbCusts = await CustomerModel.find().lean();
        const dbTxns = await TransactionModel.find().lean();
        const maxId = dbCusts.reduce((max, c) => Math.max(max, c.id || 0), 0);
        const maxAcc = dbCusts.reduce((max, c) => Math.max(max, c.accountNo || 1000), 1000);
        const maxTxn = dbTxns.reduce((max, t) => Math.max(max, t.txn_id || 0), 0);
        newId = maxId + 1;
        newAccNo = maxAcc + 1;
        newTxnId = maxTxn + 1;
    } else {
        newId = localData.nextId || (localData.customers.length + 1);
        newAccNo = localData.nextAccNo || 1001;
        newTxnId = localData.nextTxnId || 1;
    }

    localData.nextId = newId + 1;
    localData.nextAccNo = newAccNo + 1;
    localData.nextTxnId = newTxnId + 1;

    const balStr = formatPaiseToRupees(openingPaise);
    const timeStr = new Date().toISOString().replace('T', ' ').substring(0, 19);

    const newCust = {
        id: newId,
        accountNo: newAccNo,
        name: name.trim(),
        phone: cleanPhone,
        email: (email || '').trim(),
        accountType,
        balanceRupees: balStr,
        balancePaise: openingPaise,
        createdAt: timeStr
    };

    const newTxn = {
        txn_id: newTxnId,
        account_no: newAccNo,
        txn_type: 'OPENING',
        amount: balStr,
        balance_after: balStr,
        txn_time: timeStr
    };

    if (mongoose.connection.readyState === 1) {
        try {
            await CustomerModel.create(newCust);
            await TransactionModel.create(newTxn);
        } catch (err) {
            console.error("MongoDB Insert Error:", err.message);
            return res.status(500).json({ error: 'Database record creation failed: ' + err.message });
        }
    }

    localData.customers.push(newCust);
    localData.transactions.push(newTxn);
    saveLocalData(localData);

    res.json({ success: true, accountNo: newAccNo, message: `Account created! ID: #${newId}, Account No: #${newAccNo}` });
});

// Deposit Funds
app.post('/api/deposit', async (req, res) => {
    const { accountNo, amount } = req.body;
    const accNo = parseInt(accountNo, 10);
    const amtPaise = parseMoneyToPaise(amount);

    if (!accNo || !amtPaise || amtPaise <= 0) {
        return res.status(400).json({ error: 'Invalid account number or deposit amount.' });
    }

    localData = loadLocalData();
    const target = localData.customers.find(c => c.accountNo === accNo);
    if (!target) return res.status(404).json({ error: `Account number ${accNo} not found.` });

    target.balancePaise += amtPaise;
    target.balanceRupees = formatPaiseToRupees(target.balancePaise);
    const amtStr = formatPaiseToRupees(amtPaise);
    const timeStr = new Date().toISOString().replace('T', ' ').substring(0, 19);

    const newTxn = {
        txn_id: localData.nextTxnId++,
        account_no: accNo,
        txn_type: 'DEPOSIT',
        amount: amtStr,
        balance_after: target.balanceRupees,
        txn_time: timeStr
    };

    localData.transactions.push(newTxn);
    saveLocalData(localData);

    if (mongoose.connection.readyState === 1) {
        try {
            await CustomerModel.updateOne(
                { accountNo: accNo },
                { balanceRupees: target.balanceRupees, balancePaise: target.balancePaise }
            );
            await TransactionModel.create(newTxn);
        } catch (err) {
            console.error("MongoDB Sync Error on Deposit:", err.message);
        }
    }

    res.json({ success: true, newBalance: target.balanceRupees, message: `Deposit saved! New Balance: Rs. ${target.balanceRupees}` });
});

// Withdraw Funds
app.post('/api/withdraw', async (req, res) => {
    const { accountNo, amount } = req.body;
    const accNo = parseInt(accountNo, 10);
    const amtPaise = parseMoneyToPaise(amount);

    if (!accNo || !amtPaise || amtPaise <= 0) {
        return res.status(400).json({ error: 'Invalid account number or withdrawal amount.' });
    }

    localData = loadLocalData();
    const target = localData.customers.find(c => c.accountNo === accNo);
    if (!target) return res.status(404).json({ error: `Account number ${accNo} not found.` });

    if (amtPaise > target.balancePaise) {
        return res.status(400).json({ error: 'Withdrawal rejected: Insufficient balance.' });
    }

    const newBalPaise = target.balancePaise - amtPaise;
    if (target.accountType === 'SAVINGS' && newBalPaise < 50000) {
        return res.status(400).json({ error: 'Withdrawal rejected: SAVINGS balance cannot drop below Rs. 500.00.' });
    }

    target.balancePaise = newBalPaise;
    target.balanceRupees = formatPaiseToRupees(newBalPaise);
    const amtStr = formatPaiseToRupees(amtPaise);
    const timeStr = new Date().toISOString().replace('T', ' ').substring(0, 19);

    const newTxn = {
        txn_id: localData.nextTxnId++,
        account_no: accNo,
        txn_type: 'WITHDRAW',
        amount: amtStr,
        balance_after: target.balanceRupees,
        txn_time: timeStr
    };

    localData.transactions.push(newTxn);
    saveLocalData(localData);

    if (mongoose.connection.readyState === 1) {
        try {
            await CustomerModel.updateOne(
                { accountNo: accNo },
                { balanceRupees: target.balanceRupees, balancePaise: target.balancePaise }
            );
            await TransactionModel.create(newTxn);
        } catch (err) {
            console.error("MongoDB Sync Error on Withdraw:", err.message);
        }
    }

    res.json({ success: true, newBalance: target.balanceRupees, message: `Withdrawal saved! New Balance: Rs. ${target.balanceRupees}` });
});

// Delete Account
app.delete('/api/customers/:accountNo', async (req, res) => {
    const accNo = parseInt(req.params.accountNo, 10);

    localData = loadLocalData();
    const idx = localData.customers.findIndex(c => c.accountNo === accNo);
    if (idx === -1) return res.status(404).json({ error: 'Account not found.' });

    localData.customers.splice(idx, 1);
    localData.transactions = localData.transactions.filter(t => t.account_no !== accNo);
    saveLocalData(localData);

    if (mongoose.connection.readyState === 1) {
        try {
            await CustomerModel.deleteOne({ accountNo: accNo });
            await TransactionModel.deleteMany({ account_no: accNo });
        } catch (err) {
            console.error("MongoDB Sync Error on Delete:", err.message);
        }
    }

    res.json({ success: true, message: `Account ${accNo} deleted!` });
});

// View Transactions
app.get('/api/transactions/:accountNo', async (req, res) => {
    const accNo = parseInt(req.params.accountNo, 10);
    if (mongoose.connection.readyState === 1) {
        try {
            const dbTxns = await TransactionModel.find({ account_no: accNo }).sort({ txn_id: 1 }).lean();
            if (dbTxns.length > 0) return res.json(dbTxns);
        } catch (err) {}
    }
    localData = loadLocalData();
    const txns = localData.transactions.filter(t => t.account_no === accNo);
    res.json(txns);
});

const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
    console.log(`Node.js Bank Server running on port ${PORT} with MongoDB Atlas & Localhost Persistence.`);
});
