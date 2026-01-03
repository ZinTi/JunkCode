package com.gjj.common.utils;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Enumeration;
import java.util.Map;
import java.util.Properties;

public class SystemInfoUtils {

    // 获取主机名
    public static String getHostname() {
        try {
            return InetAddress.getLocalHost().getHostName();
        } catch (UnknownHostException e) {
            e.printStackTrace();
            return "Unknown";
        }
    }

    // 获取主机的IP地址
    public static String getHostAddress() {
        try {
            return InetAddress.getLocalHost().getHostAddress();
        } catch (UnknownHostException e) {
            e.printStackTrace();
            return "Unknown";
        }
    }

    /*
    NetworkInterface.getNetworkInterfaces():获取所有网络接口。
    iface.isLoopback():忽略回环接口。
    iface.isUp():忽略未启用的接口。
    addr.isLoopbackAddress():忽略回环地址。
    addr.getHostAddress().indexOf(':') == -1:忽略 IPv6 地址（只返回 IPv4 地址）。*/
    // 获取主机的实际 IP 地址（非回环地址）
    public static String getHostAddressNew() {
        try {
            Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface iface = interfaces.nextElement();
                // 忽略回环接口和未启用的接口
                if (iface.isLoopback() || !iface.isUp()) {
                    continue;
                }
                Enumeration<InetAddress> addresses = iface.getInetAddresses();
                while (addresses.hasMoreElements()) {
                    InetAddress addr = addresses.nextElement();
                    // 忽略回环地址和 IPv6 地址
                    if (!addr.isLoopbackAddress() && addr.getHostAddress().indexOf(':') == -1) {
                        return addr.getHostAddress();
                    }
                }
            }
        } catch (SocketException e) {
            e.printStackTrace();
        }
        return "Unknown";
    }

    // 获取操作系统名称
    public static String getOSName() {
        return System.getProperty("os.name");
    }

    // 获取操作系统版本
    public static String getOSVersion() {
        return System.getProperty("os.version");
    }

    // 获取操作系统架构
    public static String getOSArch() {
        return System.getProperty("os.arch");
    }

    // 获取Java运行时版本
    public static String getJavaVersion() {
        return System.getProperty("java.version");
    }

    // 获取Java虚拟机名称
    public static String getJVMName() {
        return System.getProperty("java.vm.name");
    }

    // 获取Java虚拟机供应商
    public static String getJVMVendor() {
        return System.getProperty("java.vm.vendor");
    }

    // 获取Java虚拟机版本
    public static String getJVMVersion() {
        return System.getProperty("java.vm.version");
    }

    // 获取CPU核心数
    public static int getCPUCores() {
        return Runtime.getRuntime().availableProcessors();
    }

    // 获取总内存大小（字节）
    public static long getTotalMemory() {
        return Runtime.getRuntime().totalMemory();
    }

    // 获取空闲内存大小（字节）
    public static long getFreeMemory() {
        return Runtime.getRuntime().freeMemory();
    }

    // 获取已使用内存大小（字节）
    public static long getUsedMemory() {
        return getTotalMemory() - getFreeMemory();
    }

    // 获取所有系统属性
    public static void printAllSystemProperties() {
        Properties props = System.getProperties();
        props.forEach((key, value) -> System.out.println(key + ": " + value));
    }

    // 获取所有环境变量
    public static void printAllEnvironmentVariables() {
        Map<String, String> env = System.getenv();
        env.forEach((key, value) -> System.out.println(key + ": " + value));
    }

    public static void main(String[] args) {
        System.out.println("Hostname: " + getHostname());
        System.out.println("Host Address: " + getHostAddress());
        System.out.println("Host Address(非回环地址): " + getHostAddressNew());
        System.out.println("OS Name: " + getOSName());
        System.out.println("OS Version: " + getOSVersion());
        System.out.println("OS Architecture: " + getOSArch());
        System.out.println("Java Version: " + getJavaVersion());
        System.out.println("JVM Name: " + getJVMName());
        System.out.println("JVM Vendor: " + getJVMVendor());
        System.out.println("JVM Version: " + getJVMVersion());
        System.out.println("CPU Cores: " + getCPUCores());
        System.out.println("Total Memory: " + getTotalMemory() + " bytes");
        System.out.println("Free Memory: " + getFreeMemory() + " bytes");
        System.out.println("Used Memory: " + getUsedMemory() + " bytes");

        System.out.println("\nAll System Properties:");
        printAllSystemProperties();

        System.out.println("\nAll Environment Variables:");
        printAllEnvironmentVariables();
    }
}