#include <iostream>
#include <cassert>

using namespace std;

// 定义椭圆曲线上的点
struct Point {
    long long x; // 点的x坐标
    long long y; // 点的y坐标
};

// 椭圆曲线参数
const int p = 11; // 素数p，用作模运算的基数
const int a = 1;  // 椭圆曲线方程中x^3项的系数

// 计算模p的加法
long long mod(long long x, long long p) {
    // 在进行模运算前，先将负数转换为正数
    return (x % p + p) % p;
}

// 计算模p的乘法
long long modMul(long long a, long long b, long long p) {
    // 将乘法结果进行模运算
    return mod(a * b, p);
}

// 使用费马小定理计算模p的逆元
long long modInverse(long long a, long long p) {
    // p为素数，根据费马小定理，a^(p-1) ≡ 1 (mod p)，因此可以计算逆元
    long long m = p - 2; // 指数为p-2
    long long ans = 1;
    // 快速幂算法计算a的m次方模p
    while (m) {
        if (m & 1) ans = modMul(ans, a, p);
        a = modMul(a, a, p);
        m >>= 1;
    }
    return ans;
}

// 点加运算
Point pointAdd(Point P, Point Q, long long a, long long p) {
    // 如果其中一个点是无穷远点，则返回另一个点
    if (P.x == -1) return Q;
    if (Q.x == -1) return P;
    long long m;

    // 判断两点是否相同
    if (P.x == Q.x) {
        // 若两点y坐标之和为0，则返回无穷远点
        if (mod(P.y + Q.y, p) == 0) return { -1, -1 };
        // 计算斜率
        m = modMul(modMul(3, modMul(P.x, P.x, p), p) + a, modInverse(mod(2 * P.y, p), p), p);
    }
    else {
        // 计算不同点间的斜率
        m = modMul(Q.y - P.y, modInverse(Q.x - P.x, p), p);
    }
    // 根据椭圆曲线加法公式计算新点坐标
    long long x = mod(modMul(m, m, p) - P.x - Q.x, p);
    long long y = mod(m * (P.x - x) - P.y, p);
    return { x, y };
}

// 点乘运算
Point pointMultiply(Point P, long long k, long long a, long long p) {
    // 初始化结果为无穷远点
    Point result = { -1, -1 };
    // 使用倍加法计算kP
    while (k > 0) {
        if (k & 1) result = pointAdd(result, P, a, p); // 如果k的当前位为1，则累加P
        P = pointAdd(P, P, a, p); // 将P加倍
        k >>= 1; // 右移k，处理下一位
    }
    return result;
}

// 哈希函数
int hashFunc(int x, int q) {
    // 一个简单的哈希函数，将输入x重复乘以2并取模q
    int result = 1;
    for (int i = 0; i < x; i++) {
        result = (result * 2) % q;
    }
    return result;
}

// 椭圆曲线数字签名算法
pair<int, int> sign(int x0, int y0, int m, int x, int k, int q) {
    // A为椭圆曲线上的一个点
    Point A = { x0, y0 };

    // 计算 k * A
    Point kA = pointMultiply(A, k, a, p);
    // 计算签名的两部分
    int r = kA.x % q;
    int s = modMul(modInverse(k, q), (hashFunc(x, q) + modMul(m, r, q)), q);

    // 返回签名对(r, s)
    return make_pair(r, s);
}

int main() {
    // 读取输入参数
    int x0, y0, m, x, k;
    cin >> x0 >> y0 >> m >> x >> k;

    // 生成数字签名
    pair<int, int> signature = sign(x0, y0, m, x, k, 13);

    // 输出签名
    cout << signature.first << " " << signature.second << endl;

    return 0;
}
