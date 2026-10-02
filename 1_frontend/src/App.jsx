import React, { useState, useEffect } from 'react';
import { 
  Building2, Users, CreditCard, ArrowUpRight, ArrowDownLeft, 
  Search, Plus, RefreshCw, Trash2, Activity, 
  TrendingUp, CheckCircle2, AlertTriangle, 
  Sparkles, DollarSign, Wallet, ArrowRight, Award
} from 'lucide-react';
import confetti from 'canvas-confetti';


let base = (rawApiBase && !rawApiBase.includes('<'))
  ? rawApiBase
  : (isLocal ? 'http://localhost:5000/api' : 'https://bank-management-system-17my.onrender.com/api');

if (base.endsWith('/')) base = base.slice(0, -1);
if (!base.endsWith('/api')) base += '/api';
const API_BASE = base;

export default function App() {
  const [activeTab, setActiveTab] = useState('accounts');
  const [customers, setCustomers] = useState([]);
  const [loading, setLoading] = useState(false);
  const [dbStatus, setDbStatus] = useState({ connected: false, message: 'Verifying Database Connection...' });
  const [searchTerm, setSearchTerm] = useState('');
  const [sortOrder, setSortOrder] = useState('none'); // 'none', 'asc', 'desc'
  const [highestOnly, setHighestOnly] = useState(false);


  
  // Modals state
  const [showAddModal, setShowAddModal] = useState(false);
  const [addForm, setAddForm] = useState({ name: '', phone: '', email: '', accountType: 'SAVINGS', openingDeposit: '' });
  
  // Operations state
  const [depositForm, setDepositForm] = useState({ accountNo: '', amount: '' });
  const [withdrawForm, setWithdrawForm] = useState({ accountNo: '', amount: '' });
  const [historyAccNo, setHistoryAccNo] = useState('');
  const [transactions, setTransactions] = useState([]);

  // Notification Toast state
  const [toast, setToast] = useState({ show: false, message: '', type: 'success' });


  const showToast = (message, type = 'success') => {
    setToast({ show: true, message, type });
    setTimeout(() => setToast({ show: false, message: '', type: 'success' }), 4500);
  };

  const checkHealth = async () => {
    try {
      const res = await fetch(`${API_BASE}/health`);
      const data = await res.json();
      setDbStatus(data);
    } catch (err) {
      setDbStatus({ connected: false, message: 'Node backend server offline' });
    }
  };

  const fetchCustomers = async () => {
    setLoading(true);
    try {
      const res = await fetch(`${API_BASE}/customers`);
      if (!res.ok) throw new Error('Failed to load customers from server');
      const data = await res.json();
      setCustomers(data);
    } catch (err) {
      showToast(err.message, 'error');
    } finally {
      setTimeout(() => setLoading(false), 400);
    }
  };

  const handleRefresh = () => {
    setSortOrder('none');
    setHighestOnly(false);
    setSearchTerm('');
    checkHealth();
    fetchCustomers();
    showToast('Portfolio reloaded from server');
  };

  useEffect(() => {
    checkHealth();
    fetchCustomers();
  }, []);


  // Submit Handlers
  const handleAddSubmit = async (e) => {
    e.preventDefault();
    try {
      const res = await fetch(`${API_BASE}/customers`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(addForm)
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.error);

      confetti({ particleCount: 100, spread: 70, origin: { y: 0.6 } });
      showToast(data.message);
      setShowAddModal(false);
      setAddForm({ name: '', phone: '', email: '', accountType: 'SAVINGS', openingDeposit: '' });
      fetchCustomers();
    } catch (err) {
      showToast(err.message, 'error');
    }
  };

  const handleDepositSubmit = async (e) => {
    e.preventDefault();
    try {
      const res = await fetch(`${API_BASE}/deposit`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(depositForm)
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.error);

      confetti({ particleCount: 80, spread: 80 });
      showToast(data.message);
      setDepositForm({ accountNo: '', amount: '' });
      fetchCustomers();
    } catch (err) {
      showToast(err.message, 'error');
    }
  };

  const handleWithdrawSubmit = async (e) => {
    e.preventDefault();
    try {
      const res = await fetch(`${API_BASE}/withdraw`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(withdrawForm)
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.error);

      showToast(data.message);
      setWithdrawForm({ accountNo: '', amount: '' });
      fetchCustomers();
    } catch (err) {
      showToast(err.message, 'error');
    }
  };

  const handleDelete = async (accNo) => {
    if (!window.confirm(`Permanently close account ${accNo}? All transaction logs will be cascaded.`)) return;
    try {
      const res = await fetch(`${API_BASE}/customers/${accNo}`, { method: 'DELETE' });
      const data = await res.json();
      if (!res.ok) throw new Error(data.error);
      showToast(data.message);
      fetchCustomers();
    } catch (err) {
      showToast(err.message, 'error');
    }
  };

  const fetchTransactions = async (accNo) => {
    if (!accNo) return;
    try {
      const res = await fetch(`${API_BASE}/transactions/${accNo}`);
      const data = await res.json();
      if (!res.ok) throw new Error(data.error);
      setTransactions(data);
      if (data.length === 0) showToast('No transaction logs found for this account');
    } catch (err) {
      showToast(err.message, 'error');
    }
  };

  // Metrics Calculations
  const totalBalancePaise = customers.reduce((acc, c) => acc + (c.balancePaise || 0), 0);
  const totalSavings = customers.filter(c => c.accountType === 'SAVINGS').length;
  const totalCurrent = customers.filter(c => c.accountType === 'CURRENT').length;

  // Processed customer list for display
  let displayedCustomers = [...customers];

  if (highestOnly) {
    let maxBal = -1;
    customers.forEach(c => { if (c.balancePaise > maxBal) maxBal = c.balancePaise; });
    displayedCustomers = customers.filter(c => c.balancePaise === maxBal);
  } else if (sortOrder === 'asc') {
    // Hand-rolled Insertion Sort Ascending
    for (let i = 1; i < displayedCustomers.length; i++) {
      let key = displayedCustomers[i];
      let j = i - 1;
      while (j >= 0 && displayedCustomers[j].balancePaise > key.balancePaise) {
        displayedCustomers[j + 1] = displayedCustomers[j];
        j--;
      }
      displayedCustomers[j + 1] = key;
    }
  } else if (sortOrder === 'desc') {
    // Hand-rolled Insertion Sort Descending
    for (let i = 1; i < displayedCustomers.length; i++) {
      let key = displayedCustomers[i];
      let j = i - 1;
      while (j >= 0 && displayedCustomers[j].balancePaise < key.balancePaise) {
        displayedCustomers[j + 1] = displayedCustomers[j];
        j--;
      }
      displayedCustomers[j + 1] = key;
    }
  }

  const filteredCustomers = displayedCustomers.filter(c => 
    c.name.toLowerCase().includes(searchTerm.toLowerCase()) ||
    c.accountNo.toString().includes(searchTerm) ||
    (c.id && c.id.toString().includes(searchTerm)) ||
    c.phone.includes(searchTerm)
  );


  return (
    <div className="dashboard-layout">
      {/* Toast Alert */}
      {toast.show && (
        <div style={{
          position: 'fixed', top: '28px', right: '28px', zIndex: 1000,
          background: toast.type === 'error' ? 'linear-gradient(135deg, #f43f5e, #e11d48)' : 'linear-gradient(135deg, #10b981, #059669)',
          color: 'white', padding: '16px 26px', borderRadius: '14px',
          boxShadow: '0 12px 35px rgba(0,0,0,0.4)', display: 'flex', alignItems: 'center', gap: '12px',
          fontFamily: 'var(--font-heading)', fontWeight: 600
        }}>
          {toast.type === 'error' ? <AlertTriangle size={22} /> : <CheckCircle2 size={22} />}
          <span>{toast.message}</span>
        </div>
      )}

      {/* Top Header Navigation */}
      <header className="top-nav premium-card">
        <div className="brand-section">
          <div className="brand-logo">
            <Building2 size={26} />
          </div>
          <div className="brand-info">
            <h1>Apex Bank Suite</h1>
            <div style={{ display: 'flex', alignItems: 'center', gap: '10px', marginTop: '2px' }}>
              <span className="status-indicator">
                <span className={dbStatus.connected ? 'dot-green' : 'dot-red'}></span>
                {dbStatus.connected ? 'MongoDB Atlas Connected' : dbStatus.message}
              </span>
              <span style={{ fontSize: '0.78rem', color: 'var(--text-muted)' }}>• MongoDB Dual-Sync</span>
            </div>
          </div>
        </div>

        <nav className="tab-switcher">
          <button className={`tab-btn ${activeTab === 'accounts' ? 'active' : ''}`} onClick={() => setActiveTab('accounts')}>
            <Users size={18} /> Accounts Portfolio
          </button>
          <button className={`tab-btn ${activeTab === 'operations' ? 'active' : ''}`} onClick={() => setActiveTab('operations')}>
            <Wallet size={18} /> Money Studio
          </button>
          <button className={`tab-btn ${activeTab === 'history' ? 'active' : ''}`} onClick={() => setActiveTab('history')}>
            <Activity size={18} /> Audit History
          </button>
        </nav>

        {/* System Manager Header Badge */}
        <div style={{ display: 'flex', alignItems: 'center', gap: '12px' }}>
          <div style={{
            display: 'flex', alignItems: 'center', gap: '10px',
            background: 'rgba(255, 255, 255, 0.05)', padding: '8px 16px', borderRadius: '14px',
            border: '1px solid rgba(255, 255, 255, 0.08)'
          }}>
            <div className="avatar-circle" style={{ width: '32px', height: '32px', fontSize: '0.8rem' }}>
              M
            </div>
            <div style={{ display: 'flex', flexDirection: 'column' }}>
              <span style={{ fontSize: '0.85rem', fontWeight: 700, color: 'white' }}>System Bank Manager</span>
              <span style={{ fontSize: '0.7rem', color: '#34d399' }}>● Active Portal Session</span>
            </div>
          </div>
        </div>
      </header>

      {/* Analytics Dashboard Grid */}
      <div className="analytics-grid">
        <div className="stat-card premium-card">
          <div>
            <div className="stat-title">Total Vault Balance</div>
            <div className="stat-value">Rs. {(totalBalancePaise / 100).toLocaleString('en-IN', { minimumFractionDigits: 2 })}</div>
            <div className="stat-sub">
              <Sparkles size={14} /> Encrypted & Audited Integer Paise
            </div>
          </div>
          <div className="icon-glow-box" style={{ background: 'var(--primary-gradient)', boxShadow: '0 0 24px rgba(99, 102, 241, 0.4)' }}>
            <TrendingUp size={28} />
          </div>
        </div>

        <div className="stat-card premium-card">
          <div>
            <div className="stat-title">Active Accounts</div>
            <div className="stat-value">{customers.length} Accounts</div>
            <div className="stat-sub" style={{ color: '#38bdf8' }}>
              <Users size={14} /> Live Localhost Persistent Data
            </div>
          </div>
          <div className="icon-glow-box" style={{ background: 'var(--cyan-gradient)', boxShadow: '0 0 24px rgba(6, 182, 212, 0.4)' }}>
            <Users size={28} />
          </div>
        </div>

        <div className="stat-card premium-card">
          <div>
            <div className="stat-title">Account Distribution</div>
            <div className="stat-value" style={{ fontSize: '1.3rem', marginTop: '6px' }}>
              <span style={{ color: '#a5b4fc' }}>{totalSavings} Savings</span>
              <span style={{ color: 'var(--text-muted)', margin: '0 8px' }}>|</span>
              <span style={{ color: '#fcd34d' }}>{totalCurrent} Current</span>
            </div>
            <div style={{ marginTop: '10px', height: '6px', background: 'rgba(255,255,255,0.08)', borderRadius: '10px', overflow: 'hidden', display: 'flex' }}>
              <div style={{ width: `${customers.length ? (totalSavings / customers.length) * 100 : 0}%`, background: '#6366f1' }}></div>
              <div style={{ width: `${customers.length ? (totalCurrent / customers.length) * 100 : 0}%`, background: '#f59e0b' }}></div>
            </div>
          </div>
          <div className="icon-glow-box" style={{ background: 'var(--amber-gradient)', boxShadow: '0 0 24px rgba(245, 158, 11, 0.4)' }}>
            <CreditCard size={28} />
          </div>
        </div>
      </div>

      {/* TAB 1: ACCOUNTS PORTFOLIO */}
      {activeTab === 'accounts' && (
        <div className="premium-card" style={{ padding: '28px' }}>
          <div className="controls-header">
            <div className="search-input-group">
              <Search className="search-icon-inside" size={20} />
              <input 
                type="text" 
                placeholder="Search by ID, Account No, Name, or Phone..." 
                value={searchTerm}
                onChange={(e) => setSearchTerm(e.target.value)}
              />
            </div>

            <div style={{ display: 'flex', gap: '12px', flexWrap: 'wrap' }}>
              <button 
                className={`action-btn ${sortOrder === 'asc' ? 'btn-gradient-primary' : 'btn-glass'}`}
                onClick={() => { setHighestOnly(false); setSortOrder(prev => prev === 'asc' ? 'none' : 'asc'); }}
              >
                Sort Ascending
              </button>
              <button 
                className={`action-btn ${sortOrder === 'desc' ? 'btn-gradient-primary' : 'btn-glass'}`}
                onClick={() => { setHighestOnly(false); setSortOrder(prev => prev === 'desc' ? 'none' : 'desc'); }}
              >
                Sort Descending
              </button>
              <button 
                className={`action-btn ${highestOnly ? 'btn-gradient-emerald' : 'btn-glass'}`}
                onClick={() => { setHighestOnly(prev => !prev); setSortOrder('none'); }}
              >
                <Award size={16} /> Highest Balance
              </button>
              <button 
                className="action-btn btn-glass" 
                onClick={handleRefresh}
                title="Reload Portfolio"
              >
                <RefreshCw size={18} className={loading ? 'spin' : ''} />
              </button>
              <button className="action-btn btn-gradient-primary" onClick={() => setShowAddModal(true)}>
                <Plus size={20} /> Open New Account
              </button>
            </div>
          </div>

          <div className="table-wrapper">
            <table className="modern-table">
              <thead>
                <tr>
                  <th>ID</th>
                  <th>Account No</th>
                  <th>Customer Name</th>
                  <th>Phone Number</th>
                  <th>Account Type</th>
                  <th>Balance (Rs.)</th>
                  <th>Created Timestamp</th>
                  <th>Actions</th>
                </tr>
              </thead>
              <tbody>
                {filteredCustomers.map(c => (
                  <tr key={c.accountNo}>
                    <td>
                      <span className="id-tag">#{c.id}</span>
                    </td>
                    <td>
                      <span className="account-tag">#{c.accountNo}</span>
                    </td>
                    <td>
                      <div className="customer-profile">
                        <div className="avatar-circle">
                          {c.name.charAt(0).toUpperCase()}
                        </div>
                        <div>
                          <div style={{ fontWeight: 700, color: 'var(--text-primary)' }}>{c.name}</div>
                          <div style={{ fontSize: '0.78rem', color: 'var(--text-muted)' }}>{c.email || 'No email registered'}</div>
                        </div>
                      </div>
                    </td>
                    <td style={{ fontFamily: 'var(--font-mono)' }}>{c.phone}</td>
                    <td>
                      <span className={`type-badge ${c.accountType === 'SAVINGS' ? 'type-savings' : 'type-current'}`}>
                        {c.accountType}
                      </span>
                    </td>
                    <td className="amount-high">
                      Rs. {(c.balancePaise / 100).toFixed(2)}
                    </td>
                    <td style={{ color: 'var(--text-muted)', fontSize: '0.85rem' }}>{c.createdAt}</td>
                    <td>
                      <div style={{ display: 'flex', gap: '8px' }}>
                        <button 
                          className="action-btn btn-glass"
                          style={{ padding: '6px 12px', fontSize: '0.8rem' }}
                          onClick={() => { setHistoryAccNo(c.accountNo); fetchTransactions(c.accountNo); setActiveTab('audit'); }}
                        >
                          History
                        </button>
                        <button 
                          className="action-btn btn-gradient-rose"
                          style={{ padding: '6px 12px', fontSize: '0.8rem' }}
                          onClick={() => handleDelete(c.accountNo)}
                        >
                          <Trash2 size={14} /> Close
                        </button>
                      </div>
                    </td>
                  </tr>
                ))}
                {filteredCustomers.length === 0 && (
                  <tr>
                    <td colSpan="8" style={{ textAlign: 'center', padding: '40px', color: 'var(--text-muted)' }}>
                      No matching customer accounts found in portfolio.
                    </td>
                  </tr>
                )}
              </tbody>
            </table>
          </div>
        </div>
      )}

      {/* TAB 2: MONEY STUDIO */}
      {activeTab === 'operations' && (
        <div className="studio-grid">
          {/* Deposit Card */}
          <div className="premium-card studio-card">
            <div className="studio-header">
              <div className="studio-icon-box" style={{ background: 'var(--emerald-gradient)', boxShadow: '0 0 24px rgba(16, 185, 129, 0.4)' }}>
                <ArrowDownLeft size={28} />
              </div>
              <div>
                <h2 style={{ fontSize: '1.3rem', fontWeight: 800 }}>Deposit Funds</h2>
                <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>Atomic PostgreSQL row-locking deposit (SELECT ... FOR UPDATE)</p>
              </div>
            </div>

            <form onSubmit={handleDepositSubmit}>
              <div className="form-group">
                <label>Account Number</label>
                <input 
                  type="number" 
                  placeholder="e.g. 1001"
                  value={depositForm.accountNo}
                  onChange={(e) => setDepositForm({ ...depositForm, accountNo: e.target.value })}
                  required
                />
              </div>

              <div className="form-group">
                <label>Deposit Amount (Rs.)</label>
                <input 
                  type="text" 
                  placeholder="e.g. 1500.50"
                  value={depositForm.amount}
                  onChange={(e) => setDepositForm({ ...depositForm, amount: e.target.value })}
                  required
                />
                <div className="preset-pills-row">
                  {['500', '1000', '5000', '10000'].map(val => (
                    <button type="button" key={val} className="preset-pill-btn" onClick={() => setDepositForm({ ...depositForm, amount: val })}>
                      + Rs. {val}
                    </button>
                  ))}
                </div>
              </div>

              <button type="submit" className="action-btn btn-gradient-emerald" style={{ width: '100%', marginTop: '16px', padding: '16px' }}>
                Confirm Deposit
              </button>
            </form>
          </div>

          {/* Withdraw Card */}
          <div className="premium-card studio-card">
            <div className="studio-header">
              <div className="studio-icon-box" style={{ background: 'var(--rose-gradient)', boxShadow: '0 0 24px rgba(244, 63, 94, 0.4)' }}>
                <ArrowUpRight size={28} />
              </div>
              <div>
                <h2 style={{ fontSize: '1.3rem', fontWeight: 800 }}>Withdraw Funds</h2>
                <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>Enforces ₹500.00 minimum balance for SAVINGS accounts</p>
              </div>
            </div>

            <form onSubmit={handleWithdrawSubmit}>
              <div className="form-group">
                <label>Account Number</label>
                <input 
                  type="number" 
                  placeholder="e.g. 1001"
                  value={withdrawForm.accountNo}
                  onChange={(e) => setWithdrawForm({ ...withdrawForm, accountNo: e.target.value })}
                  required
                />
              </div>

              <div className="form-group">
                <label>Withdrawal Amount (Rs.)</label>
                <input 
                  type="text" 
                  placeholder="e.g. 500.00"
                  value={withdrawForm.amount}
                  onChange={(e) => setWithdrawForm({ ...withdrawForm, amount: e.target.value })}
                  required
                />
                <div className="preset-pills-row">
                  {['500', '1000', '2000', '5000'].map(val => (
                    <button type="button" key={val} className="preset-pill-btn" onClick={() => setWithdrawForm({ ...withdrawForm, amount: val })}>
                      - Rs. {val}
                    </button>
                  ))}
                </div>
              </div>

              <button type="submit" className="action-btn btn-gradient-rose" style={{ width: '100%', marginTop: '16px', padding: '16px' }}>
                Confirm Withdrawal
              </button>
            </form>
          </div>
        </div>
      )}

      {/* TAB 3: AUDIT TRAIL */}
      {activeTab === 'history' && (
        <div className="premium-card" style={{ padding: '32px' }}>
          <div style={{ display: 'flex', alignItems: 'center', gap: '14px', marginBottom: '24px' }}>
            <div className="brand-logo" style={{ background: 'var(--cyan-gradient)' }}>
              <Activity size={26} />
            </div>
            <div>
              <h2 style={{ fontSize: '1.3rem', fontWeight: 800 }}>Audit History Ledger</h2>
              <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>
                Immutable transaction ledger for account deposits, withdrawals, and opening records.
              </p>
            </div>
          </div>

          <div style={{ display: 'flex', gap: '14px', marginBottom: '28px' }}>
            <input 
              type="number" 
              placeholder="Enter Account Number (e.g. 1001)"
              value={historyAccNo}
              onChange={(e) => setHistoryAccNo(e.target.value)}
              style={{ padding: '12px 18px', borderRadius: '12px', background: 'rgba(13, 19, 34, 0.8)', border: '1px solid var(--border-subtle)', color: 'white', width: '300px', fontFamily: 'var(--font-mono)' }}
            />
            <button className="action-btn btn-gradient-primary" onClick={() => fetchTransactions(historyAccNo)}>
              Load Audit History
            </button>
          </div>

          <div className="table-wrapper">
            <table className="modern-table">
              <thead>
                <tr>
                  <th>Txn ID</th>
                  <th>Account No</th>
                  <th>Transaction Type</th>
                  <th>Amount (Rs.)</th>
                  <th>Balance After (Rs.)</th>
                  <th>Txn Timestamp</th>
                </tr>
              </thead>
              <tbody>
                {transactions.map(t => (
                  <tr key={t.txn_id}>
                    <td style={{ fontFamily: 'var(--font-mono)' }}>#{t.txn_id}</td>
                    <td><span className="account-tag">#{t.account_no}</span></td>
                    <td>
                      <span className={`type-badge ${t.txn_type === 'DEPOSIT' ? 'type-savings' : (t.txn_type === 'WITHDRAW' ? 'type-current' : 'type-savings')}`}>
                        {t.txn_type}
                      </span>
                    </td>
                    <td className="amount-high">Rs. {t.amount}</td>
                    <td style={{ fontWeight: 700, fontFamily: 'var(--font-mono)' }}>Rs. {t.balance_after}</td>
                    <td style={{ color: 'var(--text-muted)' }}>{t.txn_time}</td>
                  </tr>
                ))}
                {transactions.length === 0 && (
                  <tr>
                    <td colSpan="6" style={{ textAlign: 'center', padding: '40px', color: 'var(--text-muted)' }}>
                      Enter an account number to view historical transaction logs.
                    </td>
                  </tr>
                )}
              </tbody>
            </table>
          </div>
        </div>
      )}

      {/* Add Account Modal */}
      {showAddModal && (
        <div className="modal-backdrop">
          <div className="modal-box premium-card">
            <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', marginBottom: '24px' }}>
              <h2 style={{ fontSize: '1.3rem', fontWeight: 800 }}>Open New Bank Account</h2>
              <button style={{ background: 'none', border: 'none', color: 'white', cursor: 'pointer', fontSize: '1.4rem' }} onClick={() => setShowAddModal(false)}>✕</button>
            </div>

            <form onSubmit={handleAddSubmit}>
              <div className="form-group">
                <label>Full Customer Name</label>
                <input type="text" placeholder="e.g. Ramesh Kumar" value={addForm.name} onChange={e => setAddForm({ ...addForm, name: e.target.value })} required />
              </div>
              <div className="form-group">
                <label>10-Digit Phone Number</label>
                <input type="text" placeholder="e.g. 9988776655" value={addForm.phone} onChange={e => setAddForm({ ...addForm, phone: e.target.value })} required />
              </div>
              <div className="form-group">
                <label>Email Address (Optional)</label>
                <input type="email" placeholder="e.g. ramesh@example.com" value={addForm.email} onChange={e => setAddForm({ ...addForm, email: e.target.value })} />
              </div>
              <div className="form-group">
                <label>Account Type</label>
                <select value={addForm.accountType} onChange={e => setAddForm({ ...addForm, accountType: e.target.value })}>
                  <option value="SAVINGS">SAVINGS (Minimum deposit Rs. 500.00)</option>
                  <option value="CURRENT">CURRENT</option>
                </select>
              </div>
              <div className="form-group">
                <label>Initial Opening Deposit (Rs.)</label>
                <input type="text" placeholder="e.g. 1500.50" value={addForm.openingDeposit} onChange={e => setAddForm({ ...addForm, openingDeposit: e.target.value })} required />
              </div>
              <div style={{ display: 'flex', gap: '14px', marginTop: '28px' }}>
                <button type="button" className="action-btn btn-glass" style={{ flex: 1 }} onClick={() => setShowAddModal(false)}>Cancel</button>
                <button type="submit" className="action-btn btn-gradient-primary" style={{ flex: 1, justifyContent: 'center' }}>Create Account</button>
              </div>
            </form>
          </div>
        </div>
      )}
    </div>
  );
}
