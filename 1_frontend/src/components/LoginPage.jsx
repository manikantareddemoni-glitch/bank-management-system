import React, { useState } from 'react';
import { 
  Building2, KeyRound, ShieldAlert, Eye, EyeOff, LogIn, RefreshCw, 
  User, ShieldCheck, Lock, Sparkles, UserCheck, CheckCircle2 
} from 'lucide-react';
import confetti from 'canvas-confetti';

export default function LoginPage({ onLoginSuccess, apiBase = '/api' }) {
  const [authMode, setAuthMode] = useState('login'); // 'login' or 'register'
  const [roleMode, setRoleMode] = useState('manager'); // 'manager' or 'customer'
  const [authForm, setAuthForm] = useState({ username: '', password: '', name: '', accountNo: '' });
  const [showPassword, setShowPassword] = useState(false);
  const [authError, setAuthError] = useState('');
  const [authLoading, setAuthLoading] = useState(false);

  const handleAuthSubmit = async (e, customCreds = null) => {
    if (e) e.preventDefault();
    setAuthError('');
    setAuthLoading(true);

    const isLogin = authMode === 'login' || customCreds !== null;
    const endpoint = isLogin ? `${apiBase}/auth/login` : `${apiBase}/auth/register`;
    const bodyData = customCreds || (isLogin 
      ? { username: authForm.username, password: authForm.password } 
      : { ...authForm, role: roleMode });

    try {
      const res = await fetch(endpoint, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(bodyData)
      });

      const contentType = res.headers.get('content-type');
      if (!contentType || !contentType.includes('application/json')) {
        throw new Error('Server is currently starting up on Render. Please wait ~15 seconds and try again.');
      }

      const data = await res.json();
      if (!res.ok) throw new Error(data.error || 'Authentication failed');

      localStorage.setItem('bank_user', JSON.stringify(data.user));
      confetti({ particleCount: 90, spread: 70, origin: { y: 0.6 } });
      
      if (onLoginSuccess) {
        onLoginSuccess(data.user);
      }
    } catch (err) {
      setAuthError(err.message);
    } finally {
      setAuthLoading(false);
    }
  };

  return (
    <div className="login-backdrop">
      <div className="login-glow-1"></div>
      <div className="login-glow-2"></div>
      
      {/* Floating particles background accent */}
      <div style={{
        position: 'absolute', top: '8%', right: '12%',
        background: 'rgba(99, 102, 241, 0.08)', padding: '8px 16px', borderRadius: '30px',
        border: '1px solid rgba(255, 255, 255, 0.1)', backdropFilter: 'blur(12px)',
        display: 'flex', alignItems: 'center', gap: '8px', fontSize: '0.78rem', color: '#a78bfa',
        zIndex: 5
      }}>
        <ShieldCheck size={14} color="#34d399" />
        <span>256-Bit Encrypted Session</span>
      </div>

      <div className="login-card">
        <div className="login-header">
          <div className="login-icon-box">
            <Building2 size={32} />
          </div>
          <h2 className="login-title">Apex Bank Suite</h2>
          <p className="login-subtitle">Enterprise Secure Banking Portal</p>
        </div>

        {/* Auth Mode Toggle Tabs */}
        <div className="auth-tabs">
          <button 
            type="button"
            className={`auth-tab-btn ${authMode === 'login' ? 'active' : ''}`}
            onClick={() => { setAuthMode('login'); setAuthError(''); }}
          >
            <LogIn size={15} style={{ verticalAlign: 'middle', marginRight: '6px' }} />
            Sign In
          </button>
          <button 
            type="button"
            className={`auth-tab-btn ${authMode === 'register' ? 'active' : ''}`}
            onClick={() => { setAuthMode('register'); setAuthError(''); }}
          >
            <UserCheck size={15} style={{ verticalAlign: 'middle', marginRight: '6px' }} />
            Register
          </button>
        </div>

        {/* Quick Demo Logins Box */}
        <div className="demo-credentials-box">
          <div className="demo-title">
            <KeyRound size={14} /> Quick Demo One-Click Access
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

        {/* Error Alert Box */}
        {authError && (
          <div style={{
            background: 'rgba(244, 63, 94, 0.15)', 
            border: '1px solid rgba(244, 63, 94, 0.3)',
            color: '#fda4af', 
            padding: '12px 16px', 
            borderRadius: '12px', 
            marginBottom: '20px',
            fontSize: '0.85rem', 
            display: 'flex', 
            alignItems: 'center', 
            gap: '8px'
          }}>
            <ShieldAlert size={18} />
            <span>{authError}</span>
          </div>
        )}

        <form onSubmit={handleAuthSubmit}>
          {authMode === 'register' && (
            <>
              <div className="input-group" style={{ marginBottom: '16px' }}>
                <label>Full Name</label>
                <div style={{ position: 'relative' }}>
                  <input 
                    type="text" 
                    placeholder="e.g. Aarav Sharma"
                    value={authForm.name}
                    onChange={e => setAuthForm({ ...authForm, name: e.target.value })}
                    required
                  />
                </div>
              </div>

              <div className="input-group" style={{ marginBottom: '16px' }}>
                <label>Account Role</label>
                <div style={{ display: 'flex', gap: '10px' }}>
                  <button
                    type="button"
                    style={{
                      flex: 1, padding: '10px', borderRadius: '12px',
                      border: roleMode === 'manager' ? '1px solid #818cf8' : '1px solid rgba(255,255,255,0.1)',
                      background: roleMode === 'manager' ? 'rgba(99, 102, 241, 0.2)' : 'rgba(255,255,255,0.03)',
                      color: roleMode === 'manager' ? '#a5b4fc' : '#94a3b8',
                      cursor: 'pointer', fontSize: '0.82rem', fontWeight: 600
                    }}
                    onClick={() => setRoleMode('manager')}
                  >
                    👨‍💼 Bank Manager
                  </button>
                  <button
                    type="button"
                    style={{
                      flex: 1, padding: '10px', borderRadius: '12px',
                      border: roleMode === 'customer' ? '1px solid #34d399' : '1px solid rgba(255,255,255,0.1)',
                      background: roleMode === 'customer' ? 'rgba(16, 185, 129, 0.2)' : 'rgba(255,255,255,0.03)',
                      color: roleMode === 'customer' ? '#6ee7b7' : '#94a3b8',
                      cursor: 'pointer', fontSize: '0.82rem', fontWeight: 600
                    }}
                    onClick={() => setRoleMode('customer')}
                  >
                    👤 Customer
                  </button>
                </div>
              </div>
            </>
          )}

          <div className="input-group" style={{ marginBottom: '16px' }}>
            <label>Username / Account No</label>
            <div style={{ position: 'relative' }}>
              <input 
                type="text" 
                placeholder={authMode === 'login' ? 'e.g. admin or 1001' : 'e.g. aarav123'}
                value={authForm.username}
                onChange={e => setAuthForm({ ...authForm, username: e.target.value })}
                required
              />
            </div>
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
            {authLoading ? <RefreshCw className="spin" size={18} /> : <LogIn size={18} />}
            <span>{authMode === 'login' ? 'Sign In to Portal' : 'Create Account'}</span>
          </button>
        </form>

        {/* Footer info */}
        <div style={{
          marginTop: '24px', paddingTop: '16px', borderTop: '1px solid rgba(255, 255, 255, 0.08)',
          display: 'flex', justifyContent: 'center', gap: '16px', fontSize: '0.75rem', color: '#64748b'
        }}>
          <span style={{ display: 'flex', alignItems: 'center', gap: '4px' }}>
            <CheckCircle2 size={12} color="#10b981" /> FDIC Insured
          </span>
          <span style={{ display: 'flex', alignItems: 'center', gap: '4px' }}>
            <Lock size={12} color="#6366f1" /> TLS 1.3
          </span>
          <span style={{ display: 'flex', alignItems: 'center', gap: '4px' }}>
            <Sparkles size={12} color="#a855f7" /> Multi-Tenant
          </span>
        </div>
      </div>
    </div>
  );
}
