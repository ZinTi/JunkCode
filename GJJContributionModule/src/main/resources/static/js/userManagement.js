// 全局变量
let currentPage = 1;
const pageSize = 10;
let totalItems = 0;
let currentUserId = '';

// DOM元素
const usersTableBody = document.getElementById('usersTableBody');
const pagination = document.getElementById('pagination');
const searchInput = document.getElementById('searchInput');
const searchButton = document.getElementById('searchButton');
const addUserBtn = document.getElementById('addUserBtn');
const refreshBtn = document.getElementById('refreshBtn');
const userModal = document.getElementById('userModal');
const modalTitle = document.getElementById('modalTitle');
const closeModalBtn = document.getElementById('closeModalBtn');
const cancelBtn = document.getElementById('cancelBtn');
const saveUserBtn = document.getElementById('saveUserBtn');
const userForm = document.getElementById('userForm');
const userIdInput = document.getElementById('userId');
const usernameInput = document.getElementById('usernameInput');
const passwordInput = document.getElementById('passwordInput');
const nicknameInput = document.getElementById('nicknameInput');
const emailInput = document.getElementById('emailInput');
const telephoneInput = document.getElementById('telephoneInput');
const genderSelect = document.getElementById('genderSelect');
const userTypeSelect = document.getElementById('userTypeSelect');
const loadingOverlay = document.getElementById('loadingOverlay');
const toast = document.getElementById('toast');

// 页面加载时初始化
document.addEventListener('DOMContentLoaded', function() {
    // 加载用户列表
    loadUsers();

    // 添加事件监听器
    searchButton.addEventListener('click', handleSearch);
    searchInput.addEventListener('keypress', function(e) {
        if (e.key === 'Enter') handleSearch();
    });

    addUserBtn.addEventListener('click', openAddUserModal);
    refreshBtn.addEventListener('click', function() {
        searchInput.value = '';
        loadUsers();
    });

    closeModalBtn.addEventListener('click', closeModal);
    cancelBtn.addEventListener('click', closeModal);
    saveUserBtn.addEventListener('click', saveUser);
});

// 加载用户列表
function loadUsers() {
    showLoading();

    const sessionId = sessionStorage.getItem('sessionId');
    const userId = sessionStorage.getItem('userId');

    if (!sessionId || !userId) {
        showToast('会话已过期，请重新登录', 'error');
        // 通知父页面会话过期
        window.parent.postMessage({ type: 'sessionExpired' }, window.location.origin);
        return;
    }

    fetch(`/api/users?page=${currentPage}&pageSize=${pageSize}`, {
        method: 'GET',
        headers: {
            'sessionId': sessionId
        }
    })
        .then(response => response.json())
        .then(data => {
            hideLoading();

            if (data.result) {
                totalItems = data.data.totalItems;
                renderUsersTable(data.data.users);
                renderPagination();
            } else {
                showToast(data.message || '加载用户列表失败', 'error');
            }
        })
        .catch(error => {
            hideLoading();
            showToast('网络错误，请稍后重试', 'error');
            console.error('加载用户列表失败:', error);
        });
}

// 渲染用户表格
function renderUsersTable(users) {
    usersTableBody.innerHTML = '';

    if (users.length === 0) {
        usersTableBody.innerHTML = `
            <tr>
                <td colspan="9" style="text-align: center; padding: 40px;">
                    没有找到用户数据
                </td>
            </tr>
        `;
        return;
    }

    users.forEach(user => {
        const row = document.createElement('tr');

        // 格式化创建时间
        const createdAt = new Date(user.createdAt);
        const formattedDate = `${createdAt.getFullYear()}-${(createdAt.getMonth() + 1).toString().padStart(2, '0')}-${createdAt.getDate().toString().padStart(2, '0')}`;

        // 用户类型标签
        let userTypeClass = '';
        let userTypeText = '';

        switch (user.userType.toLowerCase()) {
            case 'admin':
                userTypeClass = 'user-type-admin';
                userTypeText = '管理员';
                break;
            case 'teller':
                userTypeClass = 'user-type-teller';
                userTypeText = '柜员';
                break;
            case 'suspended':
                userTypeClass = 'user-type-suspended';
                userTypeText = '已停用';
                break;
            default:
                userTypeClass = 'user-type-admin';
                userTypeText = user.userType;
        }

        row.innerHTML = `
            <td>${user.userId}</td>
            <td>${user.username}</td>
            <td>${user.nickname}</td>
            <td>${user.email}</td>
            <td>${user.telephone}</td>
            <td>${user.gender === 'Male' ? '男' : user.gender === 'Female' ? '女' : '未知'}</td>
            <td><span class="user-type ${userTypeClass}">${userTypeText}</span></td>
            <td>${formattedDate}</td>
            <td class="action-cell">
                <button class="action-btn edit-btn" data-id="${user.userId}" data-username="${user.username}">编辑</button>
                <button class="action-btn delete-btn" data-id="${user.userId}" data-username="${user.username}">删除</button>
            </td>
        `;

        usersTableBody.appendChild(row);
    });

    // 添加编辑和删除事件监听器
    document.querySelectorAll('.edit-btn').forEach(btn => {
        btn.addEventListener('click', function() {
            const userId = this.getAttribute('data-id');
            const username = this.getAttribute('data-username');
            openEditUserModal(userId, username);
        });
    });

    document.querySelectorAll('.delete-btn').forEach(btn => {
        btn.addEventListener('click', function() {
            const userId = this.getAttribute('data-id');
            const username = this.getAttribute('data-username');
            deleteUser(userId, username);
        });
    });
}

// 渲染分页
function renderPagination() {
    const totalPages = Math.ceil(totalItems / pageSize);
    pagination.innerHTML = '';

    // 上一页按钮
    const prevButton = document.createElement('button');
    prevButton.className = 'page-btn';
    prevButton.innerHTML = '&laquo;';
    prevButton.disabled = currentPage === 1;
    prevButton.addEventListener('click', () => {
        if (currentPage > 1) {
            currentPage--;
            loadUsers();
        }
    });
    pagination.appendChild(prevButton);

    // 页码按钮
    for (let i = 1; i <= totalPages; i++) {
        const pageButton = document.createElement('button');
        pageButton.className = 'page-btn';
        if (i === currentPage) {
            pageButton.classList.add('active');
        }
        pageButton.textContent = i;
        pageButton.addEventListener('click', () => {
            currentPage = i;
            loadUsers();
        });
        pagination.appendChild(pageButton);
    }

    // 下一页按钮
    const nextButton = document.createElement('button');
    nextButton.className = 'page-btn';
    nextButton.innerHTML = '&raquo;';
    nextButton.disabled = currentPage === totalPages;
    nextButton.addEventListener('click', () => {
        if (currentPage < totalPages) {
            currentPage++;
            loadUsers();
        }
    });
    pagination.appendChild(nextButton);
}

// 处理搜索
function handleSearch() {
    const query = searchInput.value.trim();
    if (!query) {
        showToast('请输入用户ID或用户名', 'error');
        return;
    }

    showLoading();

    const sessionId = sessionStorage.getItem('sessionId');
    const userId = sessionStorage.getItem('userId');

    if (!sessionId || !userId) {
        showToast('会话已过期，请重新登录', 'error');
        // 通知父页面会话过期
        window.parent.postMessage({ type: 'sessionExpired' }, window.location.origin);
        return;
    }

    fetch(`/api/users/${query}`, {
        method: 'GET',
        headers: {
            'sessionId': sessionId
        }
    })
        .then(response => response.json())
        .then(data => {
            hideLoading();

            if (data.result) {
                // 清空表格并显示单个用户
                usersTableBody.innerHTML = '';
                pagination.innerHTML = '';

                const user = data.data;
                const createdAt = new Date(user.createdAt);
                const formattedDate = `${createdAt.getFullYear()}-${(createdAt.getMonth() + 1).toString().padStart(2, '0')}-${createdAt.getDate().toString().padStart(2, '0')}`;

                let userTypeClass = '';
                let userTypeText = '';

                switch (user.userType.toLowerCase()) {
                    case 'admin':
                        userTypeClass = 'user-type-admin';
                        userTypeText = '管理员';
                        break;
                    case 'teller':
                        userTypeClass = 'user-type-teller';
                        userTypeText = '柜员';
                        break;
                    case 'suspended':
                        userTypeClass = 'user-type-suspended';
                        userTypeText = '已停用';
                        break;
                    default:
                        userTypeClass = 'user-type-admin';
                        userTypeText = user.userType;
                }

                const row = document.createElement('tr');
                row.innerHTML = `
                    <td>${user.userId}</td>
                    <td>${user.username}</td>
                    <td>${user.nickname}</td>
                    <td>${user.email}</td>
                    <td>${user.telephone}</td>
                    <td>${user.gender === 'Male' ? '男' : user.gender === 'Female' ? '女' : '未知'}</td>
                    <td><span class="user-type ${userTypeClass}">${userTypeText}</span></td>
                    <td>${formattedDate}</td>
                    <td class="action-cell">
                        <button class="action-btn edit-btn" data-id="${user.userId}" data-username="${user.username}">编辑</button>
                        <button class="action-btn delete-btn" data-id="${user.userId}" data-username="${user.username}">删除</button>
                    </td>
                `;

                usersTableBody.appendChild(row);

                // 添加事件监听器
                row.querySelector('.edit-btn').addEventListener('click', function() {
                    openEditUserModal(user.userId, user.username);
                });

                row.querySelector('.delete-btn').addEventListener('click', function() {
                    deleteUser(user.userId, user.username);
                });
            } else {
                showToast(data.message || '未找到用户', 'error');
            }
        })
        .catch(error => {
            hideLoading();
            showToast('网络错误，请稍后重试', 'error');
            console.error('搜索用户失败:', error);
        });
}

// 打开新增用户模态框
function openAddUserModal() {
    modalTitle.textContent = '新增用户';
    userIdInput.value = '';
    usernameInput.value = '';
    passwordInput.value = '';
    nicknameInput.value = '';
    emailInput.value = '';
    telephoneInput.value = '';
    genderSelect.value = '';
    userTypeSelect.value = '';

    // 显示密码字段
    passwordInput.parentElement.style.display = 'block';

    userModal.classList.add('active');
}

// 打开编辑用户模态框
function openEditUserModal(userId, username) {
    showLoading();
    currentUserId = userId;

    const sessionId = sessionStorage.getItem('sessionId');
    const userIdVal = sessionStorage.getItem('userId');

    if (!sessionId || !userIdVal) {
        showToast('会话已过期，请重新登录', 'error');
        // 通知父页面会话过期
        window.parent.postMessage({ type: 'sessionExpired' }, window.location.origin);
        return;
    }

    fetch(`/api/users/${userId}`, {
        method: 'GET',
        headers: {
            'sessionId': sessionId
        }
    })
        .then(response => response.json())
        .then(data => {
            hideLoading();

            if (data.result) {
                const user = data.data;
                modalTitle.textContent = '编辑用户';
                userIdInput.value = user.userId;
                usernameInput.value = user.username;
                nicknameInput.value = user.nickname;
                emailInput.value = user.email;
                telephoneInput.value = user.telephone;
                genderSelect.value = user.gender;
                userTypeSelect.value = user.userType;

                // 隐藏密码字段
                passwordInput.parentElement.style.display = 'none';

                userModal.classList.add('active');
            } else {
                showToast(data.message || '加载用户信息失败', 'error');
            }
        })
        .catch(error => {
            hideLoading();
            showToast('网络错误，请稍后重试', 'error');
            console.error('加载用户信息失败:', error);
        });
}

// 关闭模态框
function closeModal() {
    userModal.classList.remove('active');
    userForm.reset();
}

// 保存用户（新增或更新）
function saveUser() {
    const sessionId = sessionStorage.getItem('sessionId');
    const userIdVal = sessionStorage.getItem('userId');

    if (!sessionId || !userIdVal) {
        showToast('会话已过期，请重新登录', 'error');
        // 通知父页面会话过期
        window.parent.postMessage({ type: 'sessionExpired' }, window.location.origin);
        return;
    }

    const isNewUser = !userIdInput.value;

    const userData = {
        username: usernameInput.value,
        nickname: nicknameInput.value,
        email: emailInput.value,
        telephone: telephoneInput.value,
        gender: genderSelect.value,
        userType: userTypeSelect.value
    };

    // 如果是新增用户，需要密码
    if (isNewUser) {
        userData.password = passwordInput.value;
    }

    showLoading();

    const url = isNewUser
        ? '/api/users'
        : `/api/users/${currentUserId}`;

    const method = isNewUser ? 'POST' : 'PUT';

    fetch(url, {
        method: method,
        headers: {
            'sessionId': sessionId,
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(userData)
    })
        .then(response => response.json())
        .then(data => {
            hideLoading();

            if (data.result) {
                showToast(isNewUser ? '用户添加成功' : '用户信息更新成功', 'success');
                closeModal();
                loadUsers();
            } else {
                showToast(data.message || (isNewUser ? '添加用户失败' : '更新用户失败'), 'error');
            }
        })
        .catch(error => {
            hideLoading();
            showToast('网络错误，请稍后重试', 'error');
            console.error('保存用户失败:', error);
        });
}

// 删除用户
function deleteUser(userId, username) {
    if (!confirm(`确定要删除用户 ${username} (${userId}) 吗？此操作不可撤销。`)) {
        return;
    }

    showLoading();

    const sessionId = sessionStorage.getItem('sessionId');
    const userIdVal = sessionStorage.getItem('userId');

    if (!sessionId || !userIdVal) {
        showToast('会话已过期，请重新登录', 'error');
        // 通知父页面会话过期
        window.parent.postMessage({ type: 'sessionExpired' }, window.location.origin);
        return;
    }

    fetch(`/api/users/${userId}`, {
        method: 'DELETE',
        headers: {
            'sessionId': sessionId
        }
    })
        .then(response => response.json())
        .then(data => {
            hideLoading();

            if (data.result) {
                showToast('用户删除成功', 'success');
                loadUsers();
            } else {
                showToast(data.message || '删除用户失败', 'error');
            }
        })
        .catch(error => {
            hideLoading();
            showToast('网络错误，请稍后重试', 'error');
            console.error('删除用户失败:', error);
        });
}

// 显示加载指示器
function showLoading() {
    loadingOverlay.classList.add('active');
}

// 隐藏加载指示器
function hideLoading() {
    loadingOverlay.classList.remove('active');
}

// 显示提示信息
function showToast(message, type) {
    toast.textContent = message;
    toast.className = 'toast';
    toast.classList.add(`toast-${type}`);
    toast.classList.add('show');

    setTimeout(() => {
        toast.classList.remove('show');
    }, 3000);
}