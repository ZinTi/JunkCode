@echo off

:: multiline_comment
if 0 equ 1 (
    为第三方库目录创建文件夹链接（相当于Linux中的软链接）

    路径分隔符：
    使用双引号包裹目录路径时，可以使用 "/" 正斜杠和 "\" 反斜杠
    不使用双引号包裹目录路径时，只允许使用 "\" 反斜杠
)

REM 将下列命令中的库路径更改为自己的系统上的库安装路径
HELP MKLINK
MKLINK /D asio "A:/applib/asio"
MKLINK /D boost "A:/applib/boost-v1.89.0-20251021-windows-x86_64-msys2"
MKLINK /D crow "A:/applib/Crow"
MKLINK /D openssl "A:/applib/openssl-v3.5.4-20251021-windows-x86_64-msys2"
MKLINK /D sqlite "A:/applib/sqlite-v3.50.4-20251021-windows-x86_64-mingw"
