import requests

# API地址
url = "http://gjj.com:8080/api/internal/users/register"

# 请求数据
payload = {
    "secretCode": "123456",
    "username": "admin",
    "nickname": "Mr. Li Hua",
    "password": "666666",
    "email": "admin@email.com",
    "telephone": "17766660001",
    "gender": "Male",
    "userType": "admin"
}

try:
    # 发送POST请求
    response = requests.post(url, json=payload)

    # 打印完整的响应信息
    print("响应状态码:", response.status_code)
    print("响应头信息:", response.headers)
    print("响应内容:")
    print(response.text)

except requests.exceptions.RequestException as e:
    # 处理请求异常
    print("请求失败:", e)