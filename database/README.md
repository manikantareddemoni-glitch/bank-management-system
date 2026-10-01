# Database Configuration (MongoDB Atlas)

This directory manages the MongoDB Atlas database schemas, data persistence, and initial seed records for the Bank Management System.

## Database Engine: MongoDB Atlas

The application uses **MongoDB Atlas** as the single primary database engine.

### Collections Structure

#### 1. `customers`
```json
{
  "id": 1,
  "accountNo": 1001,
  "name": "Aarav Sharma",
  "phone": "9876543210",
  "email": "aarav.sharma@example.com",
  "accountType": "SAVINGS",
  "balanceRupees": "25000.50",
  "balancePaise": 2500050,
  "createdAt": "2026-01-01 10:00:00"
}
```

#### 2. `transactions`
```json
{
  "txn_id": 1,
  "account_no": 1001,
  "txn_type": "OPENING",
  "amount": "25000.50",
  "balance_after": "25000.50",
  "txn_time": "2026-01-01 10:00:00"
}
```

## Files in this Directory

- `data.json`: Initial JSON seed data imported into MongoDB Atlas upon server startup.
- `config.txt.example`: Example MongoDB Atlas connection string template.
