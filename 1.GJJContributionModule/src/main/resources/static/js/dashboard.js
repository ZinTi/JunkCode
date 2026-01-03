// js/dashboard.js
document.addEventListener('DOMContentLoaded', function() {
    // 设置欢迎昵称
    const nickname = sessionStorage.getItem('nickname') || '超级管理员';
    document.getElementById('welcomeNickname').textContent = nickname;

    // 获取系统健康配置
    fetch('/api/health/config')
        .then(response => response.json())
        .then(data => {
            // 更新系统信息
            document.querySelector('.project-name').textContent = data.project;
            document.querySelector('.version').textContent = data.version;
            document.getElementById('copyrightYear').textContent = data.copyleft;
            document.getElementById('lastUpdated').textContent = `最后更新: ${data.now}`;
            document.getElementById('systemTime').textContent = data.now;
        })
        .catch(error => {
            console.error('获取系统配置失败:', error);
            document.getElementById('systemTime').textContent = '2025-07-02 07:50:44';
        });

    // 实时更新时间
    setInterval(() => {
        const now = new Date();
        const timeStr = now.toLocaleString('zh-CN', {
            year: 'numeric',
            month: '2-digit',
            day: '2-digit',
            hour: '2-digit',
            minute: '2-digit',
            second: '2-digit',
            hour12: false
        }).replace(/\//g, '-');

        document.getElementById('systemTime').textContent = timeStr;
    }, 1000);

    // 卡片悬停动画
    const cards = document.querySelectorAll('.card');
    cards.forEach(card => {
        card.addEventListener('mouseenter', () => {
            card.style.transform = 'translateY(-5px)';
        });

        card.addEventListener('mouseleave', () => {
            card.style.transform = 'translateY(0)';
        });
    });
});