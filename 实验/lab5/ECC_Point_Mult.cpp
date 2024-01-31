#include <iostream>
using namespace std;

// ������Բ�����ϵĵ�
struct Point {
    long long x;
    long long y;
};

// ����ģp�ļӷ�����֤�����0��p-1֮��
long long mod(long long x, long long p) {
    return (x % p + p) % p;
}

// ����ģp�ĳ˷�����֤�����0��p-1֮��
long long modMul(long long a, long long b, long long p) {
    return mod(a * b, p);
}

// ʹ�÷���С�������ģp����Ԫ��Ҫ��pΪ����
long long modInverse(long long a, long long p) {
    long long m = p - 2;
    long long ans = 1;
    // ʹ�ÿ������㷨����������
    while (m) {
        if (m & 1) ans = modMul(ans, a, p);
        a = modMul(a, a, p);
        m >>= 1;
    }
    return ans;
}

// ������㣬ʵ����Բ�����ϵĵ�P�͵�Q�ļӷ�
Point pointAdd(Point P, Point Q, long long a, long long p) {
    if (P.x == -1) return Q; // ���P������Զ�㣬����Q
    if (Q.x == -1) return P; // ���Q������Զ�㣬����P

    long long m;
    // ���P��Q��ͬ��ʹ�����߹�ʽ����б��
    if (P.x == Q.x) {
        if (mod(P.y + Q.y, p) == 0) return { -1, -1 }; // P��Q��Ϊ��Ԫ�����Ϊ����Զ��
        m = modMul(modMul(3, modMul(P.x, P.x, p), p) + a, modInverse(mod(2 * P.y, p), p), p);
    }
    else {
        // ���P��Q��ͬ��ʹ�ø��߹�ʽ����б��
        m = modMul(Q.y - P.y, modInverse(Q.x - P.x, p), p);
    }
    long long x = mod(modMul(m, m, p) - P.x - Q.x, p); // ����б�ʺ�P��������������x����
    long long y = mod(m * (P.x - x) - P.y, p); // ����б�ʺ�P��������������y����
    return { x, y };
}

// ������㣬ʵ����Բ�����ϵĵ�P��������k
Point pointMultiply(Point P, long long k, long long a, long long p) {
    Point result = { -1, -1 }; // ��ʼ�����Ϊ����Զ��
    while (k > 0) {
        if (k & 1) result = pointAdd(result, P, a, p); // �����ǰk�Ķ�����λΪ1���ۼӵ�P
        P = pointAdd(P, P, a, p); // ����P�ӱ�����P = 2 * P
        k >>= 1; // ������k����һλ��������һ�ֵ���
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