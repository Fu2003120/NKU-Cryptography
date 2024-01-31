#include <iostream>
#include <cassert>

using namespace std;

// ������Բ�����ϵĵ�
struct Point {
    long long x; // ���x����
    long long y; // ���y����
};

// ��Բ���߲���
const int p = 11; // ����p������ģ����Ļ���
const int a = 1;  // ��Բ���߷�����x^3���ϵ��

// ����ģp�ļӷ�
long long mod(long long x, long long p) {
    // �ڽ���ģ����ǰ���Ƚ�����ת��Ϊ����
    return (x % p + p) % p;
}

// ����ģp�ĳ˷�
long long modMul(long long a, long long b, long long p) {
    // ���˷��������ģ����
    return mod(a * b, p);
}

// ʹ�÷���С�������ģp����Ԫ
long long modInverse(long long a, long long p) {
    // pΪ���������ݷ���С����a^(p-1) �� 1 (mod p)����˿��Լ�����Ԫ
    long long m = p - 2; // ָ��Ϊp-2
    long long ans = 1;
    // �������㷨����a��m�η�ģp
    while (m) {
        if (m & 1) ans = modMul(ans, a, p);
        a = modMul(a, a, p);
        m >>= 1;
    }
    return ans;
}

// �������
Point pointAdd(Point P, Point Q, long long a, long long p) {
    // �������һ����������Զ�㣬�򷵻���һ����
    if (P.x == -1) return Q;
    if (Q.x == -1) return P;
    long long m;

    // �ж������Ƿ���ͬ
    if (P.x == Q.x) {
        // ������y����֮��Ϊ0���򷵻�����Զ��
        if (mod(P.y + Q.y, p) == 0) return { -1, -1 };
        // ����б��
        m = modMul(modMul(3, modMul(P.x, P.x, p), p) + a, modInverse(mod(2 * P.y, p), p), p);
    }
    else {
        // ���㲻ͬ����б��
        m = modMul(Q.y - P.y, modInverse(Q.x - P.x, p), p);
    }
    // ������Բ���߼ӷ���ʽ�����µ�����
    long long x = mod(modMul(m, m, p) - P.x - Q.x, p);
    long long y = mod(m * (P.x - x) - P.y, p);
    return { x, y };
}

// �������
Point pointMultiply(Point P, long long k, long long a, long long p) {
    // ��ʼ�����Ϊ����Զ��
    Point result = { -1, -1 };
    // ʹ�ñ��ӷ�����kP
    while (k > 0) {
        if (k & 1) result = pointAdd(result, P, a, p); // ���k�ĵ�ǰλΪ1�����ۼ�P
        P = pointAdd(P, P, a, p); // ��P�ӱ�
        k >>= 1; // ����k��������һλ
    }
    return result;
}

// ��ϣ����
int hashFunc(int x, int q) {
    // һ���򵥵Ĺ�ϣ������������x�ظ�����2��ȡģq
    int result = 1;
    for (int i = 0; i < x; i++) {
        result = (result * 2) % q;
    }
    return result;
}

// ��Բ��������ǩ���㷨
pair<int, int> sign(int x0, int y0, int m, int x, int k, int q) {
    // AΪ��Բ�����ϵ�һ����
    Point A = { x0, y0 };

    // ���� k * A
    Point kA = pointMultiply(A, k, a, p);
    // ����ǩ����������
    int r = kA.x % q;
    int s = modMul(modInverse(k, q), (hashFunc(x, q) + modMul(m, r, q)), q);

    // ����ǩ����(r, s)
    return make_pair(r, s);
}

int main() {
    // ��ȡ�������
    int x0, y0, m, x, k;
    cin >> x0 >> y0 >> m >> x >> k;

    // ��������ǩ��
    pair<int, int> signature = sign(x0, y0, m, x, k, 13);

    // ���ǩ��
    cout << signature.first << " " << signature.second << endl;

    return 0;
}
