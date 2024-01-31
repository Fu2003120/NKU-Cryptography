#include <iostream>
using namespace std;

// 定义椭圆曲线上的点
struct Point {
    long long x;
    long long y;
};

// 计算模p的加法，保证结果在0到p-1之间
long long mod(long long x, long long p) {
    return (x % p + p) % p;
}

// 计算模p的乘法，保证结果在0到p-1之间
long long modMul(long long a, long long b, long long p) {
    return mod(a * b, p);
}

// 使用费马小定理计算模p的逆元，要求p为素数
long long modInverse(long long a, long long p) {
    long long m = p - 2;
    long long ans = 1;
    // 使用快速幂算法加速幂运算
    while (m) {
        if (m & 1) ans = modMul(ans, a, p);
        a = modMul(a, a, p);
        m >>= 1;
    }
    return ans;
}

// 点加运算，实现椭圆曲线上的点P和点Q的加法
Point pointAdd(Point P, Point Q, long long a, long long p) {
    if (P.x == -1) return Q; // 如果P是无穷远点，返回Q
    if (Q.x == -1) return P; // 如果Q是无穷远点，返回P

    long long m;
    // 如果P和Q相同，使用切线公式计算斜率
    if (P.x == Q.x) {
        if (mod(P.y + Q.y, p) == 0) return { -1, -1 }; // P和Q互为逆元，结果为无穷远点
        m = modMul(modMul(3, modMul(P.x, P.x, p), p) + a, modInverse(mod(2 * P.y, p), p), p);
    }
    else {
        // 如果P和Q不同，使用割线公式计算斜率
        m = modMul(Q.y - P.y, modInverse(Q.x - P.x, p), p);
    }
    long long x = mod(modMul(m, m, p) - P.x - Q.x, p); // 根据斜率和P点坐标计算结果点的x坐标
    long long y = mod(m * (P.x - x) - P.y, p); // 根据斜率和P点坐标计算结果点的y坐标
    return { x, y };
}

// 点乘运算，实现椭圆曲线上的点P乘以整数k
Point pointMultiply(Point P, long long k, long long a, long long p) {
    Point result = { -1, -1 }; // 初始化结果为无穷远点
    while (k > 0) {
        if (k & 1) result = pointAdd(result, P, a, p); // 如果当前k的二进制位为1，累加点P
        P = pointAdd(P, P, a, p); // 将点P加倍，即P = 2 * P
        k >>= 1; // 将乘数k右移一位，进行下一轮迭代
    }
    return result;
}

int main() {
    long long a, b, p, x, y, k;
    cin >> a >> b >> p;
    cin >> x >> y >> k;
    Point P = { x, y };
    Point result = pointMultiply(P, k, a, p);
    if (k == 0) result = { -1, -1 };
    cout << result.x << " " << result.y << endl;
    system("pause");
    return 0;
}