// js/main.js
document.addEventListener('DOMContentLoaded', function() {
    // 获取用户信息
    const username = sessionStorage.getItem('username') || 'admin';
    const nickname = sessionStorage.getItem('nickname') || '超级管理员';

    // 更新页面上的用户信息
    document.getElementById('username').textContent = username;
    document.getElementById('nickname').textContent = nickname;

    // 设置用户头像首字母
    const userAvatar = document.getElementById('userAvatar');
    userAvatar.textContent = nickname.charAt(0);

    // 用户菜单显示/隐藏
    const userProfile = document.getElementById('userProfile');
    const userDropdown = document.getElementById('userDropdown');

    userProfile.addEventListener('click', function(e) {
        e.stopPropagation();
        userDropdown.classList.toggle('active');
    });

    // 点击页面其他区域关闭下拉菜单
    document.addEventListener('click', function() {
        userDropdown.classList.remove('active');
    });

    // 阻止下拉菜单点击时关闭
    userDropdown.addEventListener('click', function(e) {
        e.stopPropagation();
    });

    // 菜单项点击事件
    const menuItems = document.querySelectorAll('.menu-item');
    const mainFrame = document.getElementById('mainFrame');

    menuItems.forEach(item => {
        item.addEventListener('click', function() {
            // 移除所有active类
            menuItems.forEach(i => i.classList.remove('active'));
            // 添加active类到当前项
            this.classList.add('active');

            // 加载对应页面
            const target = this.getAttribute('data-target');
            mainFrame.src = target;
        });
    });

    // 退出登录功能
    document.querySelectorAll('.dropdown-item')[2].addEventListener('click', function() {
        const sessionId = sessionStorage.getItem('sessionId');
        if (sessionId) {
            fetch('/api/auth/revoke', {
                method: 'POST',
                headers: {
                    'sessionId': sessionId
                }
            })
                .catch(error => {
                    // 即使请求失败也继续执行清理操作
                    console.error('退出请求失败:', error);
                })
                .finally(() => {
                    // 无论请求成功与否，都执行清理操作
                    sessionStorage.clear();
                    window.location.href = 'login.html';
                });
        } else {
            // 没有sessionId时直接清理并跳转
            sessionStorage.clear();
            window.location.href = 'login.html';
        }
    });

    // 初始化加载控制面板
    mainFrame.src = 'dashboard.html';

    // 添加消息监听，监听子页面内容的通知
    window.addEventListener('message', function(event) {
        if (event.data.type === 'sessionExpired') {
            // 清除sessionStorage
            sessionStorage.clear();
            // 跳转到登录页面
            window.location.href = 'login.html';
        }
    });
});
