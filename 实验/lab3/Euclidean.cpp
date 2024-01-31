#include <iostream>
using namespace std;

typedef long long ll;

// ���� ax + by = gcd(a, b) �������⣬������gcd(a, b)
ll extendedGCD(ll num1, ll num2, ll& inverse, ll& coefficient) {
    // ��ʼ��x1, x2��Ϊx��ǰ����ֵ��y1, y2��Ϊy��ǰ����ֵ
    // �ڿ�ʼʱ��x1Ϊ1��x2Ϊ0��y1Ϊ0��y2Ϊ1
    ll x1 = 1, x2 = 0, y1 = 0, y2 = 1;
    while (num2 != 0) {
        // ����num1����num2���̺�����
        ll quotient = num1 / num2;
        ll remainder = num1 % num2;

        // ����num1��num2Ϊ��һ�ֵ���
        num1 = num2;
        num2 = remainder;

        // ����x��y��ֵ
        ll tempX = x1 - quotient * x2;
        x1 = x2;
        x2 = tempX;

        ll tempY = y1 - quotient * y2;
        y1 = y2;
        y2 = tempY;
    }
    // ��num2��Ϊ0ʱ���˳�ѭ������ʱnum1�洢��gcd(num1, num2)
    // x1��y1�洢������ax + by = gcd(a, b)�Ľ�
    inverse = x1;
    coefficient = y1;
    return num1;
}

int main() {
    ll num1, num2, inverse, coefficient;
    cin >> num1 >> num2;
    ll gcd = extendedGCD(num1, num2, inverse, coefficient);

    if (gcd != 1) {
        // ���gcd��Ϊ1������Ԫ������
        cout << "��Ԫ�����ڣ�num1��num2�����Լ����Ϊ1��" << endl;
    }
    else {
        // ���gcdΪ1���������Ԫ
        inverse = (inverse % num2 + num2) % num2; // ��֤inverseΪ����
        cout << inverse << endl;
    }
    return 0;
}