import React, { useState, useEffect } from 'react';
import { 
  Building2, Users, CreditCard, ArrowUpRight, ArrowDownLeft, 
  Search, Plus, RefreshCw, Trash2, Activity, ShieldCheck, 
  TrendingUp, CheckCircle2, AlertTriangle, Cpu, Play, Pause, RotateCcw,
  Sparkles, DollarSign, Wallet, ArrowRight, Clock, Award,
  Lock, LogIn, LogOut, Eye, EyeOff, User, KeyRound, ShieldAlert
} from 'lucide-react';
import confetti from 'canvas-confetti';

const rawApiBase = import.meta.env.VITE_API_BASE_URL;
const isLocal = typeof window !== 'undefined' && (window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1');
const API_BASE = (rawApiBase && !rawApiBase.includes('<'))
  ? rawApiBase
  : (isLocal ? 'http://localhost:5000/api' : 'https://bank-management-system-17my.onrender.com/api');

export default function App() {
  const [activeTab, setActiveTab] = useState('accounts');
  const [customers, setCustomers] = useState([]);
  const [loading, setLoading] = useState(false);
  const [dbStatus, setDbStatus] = useState({ connected: false, message: 'Verifying Database Connection...' });
  const [searchTerm, setSearchTerm] = useState('');
  const [sortOrder, setSortOrder] = useState('none'); // 'none', 'asc', 'desc'
  const [highestOnly, setHighestOnly] = useState(false);

  // Authentication State
  const [currentUser, setCurrentUser] = useState(() => {
    const saved = localStorage.getItem('bank_user');
    return saved ? JSON.parse(saved) : null;
  });

  const [authMode, setAuthMode] = useState('login');
  const [authForm, setAuthForm] = useState({ username: '', password: '', name: '', role: 'manager', accountNo: '' });
  const [showPassword, setShowPassword] = useState(false);
  const [authError, setAuthError] = useState('');
  const [authLoading, setAuthLoading] = useState(false);

  const handleAuthSubmit = async (e, customCreds = null) => {
    if (e) e.preventDefault();
    setAuthError('');
    setAuthLoading(true);

    const isLogin = authMode === 'login' || customCreds !== null;
    const endpoint = isLogin ? `${API_BASE}/auth/login` : `${API_BASE}/auth/register`;
    const bodyData = customCreds || (isLogin 
      ? { username: authForm.username, password: authForm.password } 
      : authForm);

    try {
      const res = await fetch(endpoint, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(bodyData)
      });
      const data = await res.json();
      if (!res.ok) throw new Error(data.error || 'Authentication failed');

      localStorage.setItem('bank_user', JSON.stringify(data.user));
      setCurrentUser(data.user);
      confetti({ particleCount: 90, spread: 70, origin: { y: 0.6 } });
    } catch (err) {
      setAuthError(err.message);
    } finally {
      setAuthLoading(false);
    }
  };

  const handleLogout = () => {
    localStorage.removeItem('bank_user');
    setCurrentUser(null);
  };
  
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

  // DSA Visualizer State
  const [binarySearchAcc, setBinarySearchAcc] = useState('');
  const [binarySteps, setBinarySteps] = useState([]);
  const [currentBinaryIdx, setCurrentBinaryIdx] = useState(0);
  const [isAutoPlaying, setIsAutoPlaying] = useState(false);

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

  // Hand-rolled Binary Search Visualizer Simulation
  const runBinarySearchVisualizer = () => {
    const acc = parseInt(binarySearchAcc, 10);
    if (!acc) {
      showToast('Please enter a valid Account Number', 'error');
      return;
    }
    const sorted = [...customers].sort((a, b) => a.accountNo - b.accountNo);
    let low = 0;
    let high = sorted.length - 1;
    const steps = [];

    while (low <= high) {
      let mid = Math.floor(low + (high - low) / 2);
      const midAcc = sorted[mid].accountNo;
      const isMatch = midAcc === acc;
      steps.push({
        low, high, mid, midAcc,
        found: isMatch,
        explanation: isMatch 
          ? `MATCH FOUND! target (${acc}) equals account_no at mid index [${mid}].` 
          : (midAcc < acc 
              ? `${midAcc} < ${acc}: Target is in the RIGHT half. Set low = ${mid + 1}.` 
              : `${midAcc} > ${acc}: Target is in the LEFT half. Set high = ${mid - 1}.`)
      });
      if (isMatch) break;
      if (midAcc < acc) low = mid + 1;
      else high = mid - 1;
    }
    setBinarySteps(steps);
    setCurrentBinaryIdx(0);
    setIsAutoPlaying(false);
  };

  useEffect(() => {
    let timer;
    if (isAutoPlaying && binarySteps.length > 0) {
      timer = setInterval(() => {
        setCurrentBinaryIdx(prev => {
          if (prev >= binarySteps.length - 1) {
            setIsAutoPlaying(false);
            return prev;
          }
          return prev + 1;
        });
      }, 1500);
    }
    return () => clearInterval(timer);
  }, [isAutoPlaying, binarySteps]);

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

  if (!currentUser) {
    return (
      <div className="login-backdrop">
        <div className="login-glow-1"></div>
        <div className="login-glow-2"></div>

        <div className="login-card">
          <div className="login-header">
            <div className="login-icon-box">
              <Building2 size={32} />
            </div>
            <h2 className="login-title">Apex Bank Suite</h2>
            <p className="login-subtitle">Secure Financial Portal Login</p>
          </div>

          <div className="auth-tabs">
            <button 
              className={`auth-tab-btn ${authMode === 'login' ? 'active' : ''}`}
              onClick={() => { setAuthMode('login'); setAuthError(''); }}
            >
              Sign In
            </button>
            <button 
              className={`auth-tab-btn ${authMode === 'register' ? 'active' : ''}`}
              onClick={() => { setAuthMode('register'); setAuthError(''); }}
            >
              Register
            </button>
          </div>

          {/* Quick Demo Logins */}
          <div className="demo-credentials-box">
            <div className="demo-title">
              <KeyRound size={14} /> Quick Demo Logins
            </div>
            <div className="demo-buttons-grid">
              <button 
                type="button" 
                className="demo-btn"
                onClick={() => handleAuthSubmit(null, { username: 'admin', password: 'admin123' })}
              >
                <span>👨‍💼 Bank Manager</span>
                <span style={{ fontSize: '0.72rem', color: '#a78bfa' }}>admin / admin123</span>
              </button>
              <button 
                type="button" 
                className="demo-btn"
                onClick={() => handleAuthSubmit(null, { username: '1001', password: 'demo123' })}
              >
                <span>👤 Customer Portal</span>
                <span style={{ fontSize: '0.72rem', color: '#34d399' }}>1001 / demo123</span>
              </button>
            </div>
          </div>

          {authError && (
            <div style={{
              background: 'rgba(244, 63, 94, 0.15)', border: '1px solid rgba(244, 63, 94, 0.3)',
              color: '#fda4af', padding: '12px 16px', borderRadius: '12px', marginBottom: '20px',
              fontSize: '0.85rem', display: 'flex', alignItems: 'center', gap: '8px'
            }}>
              <ShieldAlert size={18} />
              <span>{authError}</span>
            </div>
          )}

          <form onSubmit={handleAuthSubmit}>
            {authMode === 'register' && (
              <div className="input-group" style={{ marginBottom: '16px' }}>
                <label>Full Name</label>
                <input 
                  type="text" 
                  placeholder="e.g. Aarav Sharma"
                  value={authForm.name}
                  onChange={e => setAuthForm({ ...authForm, name: e.target.value })}
                  required
                />
              </div>
            )}

            <div className="input-group" style={{ marginBottom: '16px' }}>
              <label>Username / Account No</label>
              <input 
                type="text" 
                placeholder={authMode === 'login' ? 'e.g. admin or 1001' : 'e.g. aarav123'}
                value={authForm.username}
                onChange={e => setAuthForm({ ...authForm, username: e.target.value })}
                required
              />
            </div>

            <div className="input-group" style={{ marginBottom: '24px' }}>
              <label>Password</label>
              <div style={{ position: 'relative' }}>
                <input 
                  type={showPassword ? 'text' : 'password'}
                  placeholder="••••••••"
                  value={authForm.password}
                  onChange={e => setAuthForm({ ...authForm, password: e.target.value })}
                  required
                  style={{ width: '100%', paddingRight: '42px' }}
                />
                <button
                  type="button"
                  onClick={() => setShowPassword(!showPassword)}
                  style={{
                    position: 'absolute', right: '12px', top: '50%', transform: 'translateY(-50%)',
                    background: 'none', border: 'none', color: 'var(--text-muted)', cursor: 'pointer'
                  }}
                >
                  {showPassword ? <EyeOff size={18} /> : <Eye size={18} />}
                </button>
              </div>
            </div>

            <button type="submit" className="action-btn btn-primary" style={{ width: '100%', padding: '14px' }} disabled={authLoading}>
              {authLoading ? <RefreshCw className="spin-icon" size={18} /> : <LogIn size={18} />}
              <span>{authMode === 'login' ? 'Sign In to Portal' : 'Create Account'}</span>
            </button>
          </form>
        </div>
      </div>
    );
  }

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
          <button className={`tab-btn ${activeTab === 'visualizer' ? 'active' : ''}`} onClick={() => setActiveTab('visualizer')}>
            <Cpu size={18} /> DSA Visualizer
          </button>
          <button className={`tab-btn ${activeTab === 'history' ? 'active' : ''}`} onClick={() => setActiveTab('history')}>
            <Activity size={18} /> Audit History
          </button>
        </nav>

        {/* User Profile & Logout */}
        <div style={{ display: 'flex', alignItems: 'center', gap: '12px' }}>
          <div style={{
            display: 'flex', alignItems: 'center', gap: '10px',
            background: 'rgba(255, 255, 255, 0.05)', padding: '6px 14px', borderRadius: '14px',
            border: '1px solid rgba(255, 255, 255, 0.08)'
          }}>
            <div className="avatar-circle" style={{ width: '32px', height: '32px', fontSize: '0.8rem' }}>
              {currentUser.name ? currentUser.name[0].toUpperCase() : 'U'}
            </div>
            <div style={{ display: 'flex', flexDirection: 'column' }}>
              <span style={{ fontSize: '0.85rem', fontWeight: 700, color: 'white' }}>{currentUser.name}</span>
              <span style={{ fontSize: '0.7rem', color: '#a78bfa', textTransform: 'capitalize' }}>
                {currentUser.role === 'manager' ? '👨‍💼 Bank Manager' : `👤 Account #${currentUser.accountNo}`}
              </span>
            </div>
          </div>
          <button 
            onClick={handleLogout}
            className="action-btn"
            style={{ padding: '8px 14px', background: 'rgba(244, 63, 94, 0.12)', color: '#fda4af', border: '1px solid rgba(244, 63, 94, 0.25)' }}
            title="Sign Out"
          >
            <LogOut size={16} />
            <span>Logout</span>
          </button>
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

      {/* TAB 3: DSA VISUALIZER */}
      {activeTab === 'dsa' && (
        <div className="premium-card" style={{ padding: '32px' }}>
          <div style={{ display: 'flex', alignItems: 'center', gap: '14px', marginBottom: '24px' }}>
            <div className="brand-logo" style={{ background: 'var(--purple-gradient)' }}>
              <Cpu size={26} />
            </div>
            <div>
              <h2 style={{ fontSize: '1.3rem', fontWeight: 800 }}>Interactive Binary Search Visualizer O(log n)</h2>
              <p style={{ fontSize: '0.85rem', color: 'var(--text-muted)' }}>
                Demonstrates hand-rolled iterative binary search halving intervals on vector sorted by account_no.
              </p>
            </div>
          </div>

          <div style={{ display: 'flex', gap: '14px', marginBottom: '24px', flexWrap: 'wrap' }}>
            <input 
              type="number" 
              placeholder="Target Account No (e.g. 1003)" 
              value={binarySearchAcc}
              onChange={(e) => setBinarySearchAcc(e.target.value)}
              style={{ padding: '12px 18px', borderRadius: '12px', background: 'rgba(13, 19, 34, 0.8)', border: '1px solid var(--border-subtle)', color: 'white', width: '280px', fontFamily: 'var(--font-mono)' }}
            />
            <button className="action-btn btn-gradient-primary" onClick={runBinarySearchVisualizer}>
              Run Binary Search
            </button>
          </div>

          {binarySteps.length > 0 && (
            <div>
              <div style={{ display: 'flex', gap: '12px', alignItems: 'center', marginBottom: '20px' }}>
                <button 
                  className="action-btn btn-glass" 
                  disabled={currentBinaryIdx === 0}
                  onClick={() => setCurrentBinaryIdx(prev => Math.max(0, prev - 1))}
                >
                  Step Back
                </button>
                <button 
                  className="action-btn btn-glass"
                  onClick={() => setIsAutoPlaying(!isAutoPlaying)}
                >
                  {isAutoPlaying ? <Pause size={16} /> : <Play size={16} />} {isAutoPlaying ? 'Pause' : 'Auto Play'}
                </button>
                <button 
                  className="action-btn btn-glass" 
                  disabled={currentBinaryIdx === binarySteps.length - 1}
                  onClick={() => setCurrentBinaryIdx(prev => Math.min(binarySteps.length - 1, prev + 1))}
                >
                  Next Step
                </button>
                <span style={{ fontWeight: 700, color: '#38bdf8', fontFamily: 'var(--font-mono)' }}>
                  Iteration {currentBinaryIdx + 1} of {binarySteps.length}
                </span>
              </div>

              <div style={{ padding: '16px 20px', background: 'rgba(13, 19, 34, 0.9)', borderRadius: '12px', marginBottom: '24px', borderLeft: '4px solid #6366f1', fontWeight: 600 }}>
                {binarySteps[currentBinaryIdx].explanation}
              </div>

              <div className="dsa-canvas">
                {customers.map((c, idx) => {
                  const step = binarySteps[currentBinaryIdx];
                  const isMid = idx === step.mid;
                  const isLow = idx === step.low;
                  const isHigh = idx === step.high;
                  const isMatch = step.found && isMid;

                  let nodeClass = 'node-card';
                  if (isMatch) nodeClass += ' match-node';
                  else if (isMid) nodeClass += ' mid-node';
                  else if (isLow || isHigh) nodeClass += ' low-node';

                  return (
                    <div key={c.accountNo} className={nodeClass}>
                      <div className="node-val">#{c.accountNo}</div>
                      <div className="node-sub">idx [{idx}]</div>

                      {isMid && <span className="pointer-badge badge-mid-p">MID</span>}
                      {isLow && !isMid && <span className="pointer-badge badge-low-p">LOW</span>}
                      {isHigh && !isMid && <span className="pointer-badge badge-high-p">HIGH</span>}
                    </div>
                  );
                })}
              </div>
            </div>
          )}
        </div>
      )}

      {/* TAB 4: AUDIT TRAIL */}
      {activeTab === 'audit' && (
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
