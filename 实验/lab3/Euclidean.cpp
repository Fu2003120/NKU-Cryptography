#include <iostream>
using namespace std;

typedef long long ll;

// 计算 ax + by = gcd(a, b) 的整数解，并返回gcd(a, b)
ll extendedGCD(ll num1, ll num2, ll& inverse, ll& coefficient) {
    // 初始化x1, x2作为x的前两个值，y1, y2作为y的前两个值
    // 在开始时，x1为1，x2为0，y1为0，y2为1
    ll x1 = 1, x2 = 0, y1 = 0, y2 = 1;
    while (num2 != 0) {
        // 计算num1除以num2的商和余数
        ll quotient = num1 / num2;
        ll remainder = num1 % num2;

        // 更新num1和num2为下一轮迭代
        num1 = num2;
        num2 = remainder;

        // 更新x和y的值
        ll tempX = x1 - quotient * x2;
        x1 = x2;
        x2 = tempX;

        ll tempY = y1 - quotient * y2;
        y1 = y2;
        y2 = tempY;
    }
    // 当num2变为0时，退出循环，此时num1存储着gcd(num1, num2)
    // x1和y1存储着满足ax + by = gcd(a, b)的解
    inverse = x1;
    coefficient = y1;
    return num1;
}

int main() {
    ll num1, num2, inverse, coefficient;
    cin >> num1 >> num2;
    ll gcd = extendedGCD(num1, num2, inverse, coefficient);

    if (gcd != 1) {
        // 如果gcd不为1，则逆元不存在
        cout << "逆元不存在（num1和num2的最大公约数不为1）" << endl;
    }
    else {
        // 如果gcd为1，则计算逆元
        inverse = (inverse % num2 + num2) % num2; // 保证inverse为正数
        cout << inverse << endl;
    }
    return 0;
}