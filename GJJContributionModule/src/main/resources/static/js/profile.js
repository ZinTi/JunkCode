document.addEventListener('DOMContentLoaded', () => {
    const profileForm = document.getElementById('profileForm');
    const sessionId = sessionStorage.getItem('sessionId');

    // 检查会话ID是否存在
    if (!sessionId) {
        notifySessionExpired();
        return;
    }

    // 获取用户资料
    fetchUserProfile(sessionId);

    // 表单提交事件
    profileForm.addEventListener('submit', (e) => {
        e.preventDefault();
        updateUserProfile(sessionId);
    });
});

function fetchUserProfile(sessionId) {
    fetch('/api/users/me', {
        method: 'GET',
        headers: {
            'sessionId': sessionId
        }
    })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            if (data.result === true) {
                populateForm(data.data);
            } else {
                handleApiError(data.message);
            }
        })
        .catch(error => {
            console.error('获取用户资料失败:', error);
            handleApiError(error.message);
        });
}

function populateForm(userData) {
    document.getElementById('userId').value = userData.userId || '';
    document.getElementById('username').value = userData.username || '';
    document.getElementById('nickname').value = userData.nickname || '';
    document.getElementById('email').value = userData.email || '';
    document.getElementById('telephone').value = userData.telephone || '';
    document.getElementById('gender').value = userData.gender || 'Unknown';
    document.getElementById('userType').value = userData.userType || '';
    document.getElementById('createdAt').value = userData.createdAt || '';
    document.getElementById('updatedAt').value = userData.updatedAt || '';
}

function updateUserProfile(sessionId) {
    const formData = {
        username: document.getElementById('username').value,
        nickname: document.getElementById('nickname').value,
        email: document.getElementById('email').value,
        telephone: document.getElementById('telephone').value,
        gender: document.getElementById('gender').value
    };

    fetch('/api/users', {
        method: 'PUT',
        headers: {
            'Content-Type': 'application/json',
            'sessionId': sessionId
        },
        body: JSON.stringify(formData)
    })
        .then(response => response.json())
        .then(data => {
            if (data.result === true) {
                alert('个人资料更新成功！');
                fetchUserProfile(sessionId); // 刷新数据
            } else {
                handleApiError(data.message);
            }
        })
        .catch(error => {
            console.error('更新用户资料失败:', error);
            handleApiError(error.message);
        });
}

function handleApiError(message) {
    if (message.includes('会话过期或无效') || message.includes('请重新登录')) {
        notifySessionExpired();
    } else {
        alert(`操作失败: ${message}`);
    }
}

function notifySessionExpired() {
    window.parent.postMessage({ type: 'sessionExpired' }, '*');
    alert('会话已过期，即将跳转到登录页面');
}