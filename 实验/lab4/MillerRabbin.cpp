#include <iostream>
#include <omp.h> // 引入 OpenMP 库，用于并行处理
#include <cmath>
#include <limits>
#include <algorithm>
using namespace std;

typedef long long long_long;

// 快速幂算法，用于高效计算 (base^exponent) % modulus
// 使用 inline 关键字提高函数调用效率
inline long_long modular_pow(long_long base, long_long exponent, long_long modulus) {
    long_long result = 1; // 初始化结果
    base %= modulus; // 预先对 base 进行模运算，以防溢出
    while (exponent) {
        if (exponent & 1) { // 如果指数的当前位是 1
            result = (__int128)result * base % modulus; // 计算结果，使用 __int128 防止溢出,__int128 提供了 128 位整数的支持。
        }
        base = (__int128)base * base % modulus; // 更新 base
        exponent >>= 1; // 指数右移一位
    }
    return result;
}

// 判断一个数是否为素数的米勒-罗宾测试
bool is_prime(long_long number) {
    if (number < 3) // 对小于 3 的数进行特判
        return number == 2;
    if (number % 2 == 0) // 偶数不是素数
        return false;

    /*
    预定义一组测试基:
    在米勒-罗宾素数检测算法中，测试基（witnesses）用于帮助确定一个给定的数是否是素数。
    这种方法是一种概率性测试，用于检验一个数是否符合素数的特定数学特征。
    */
    long_long test_bases[] = { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 };

    // 计算将 number-1 分解为 2^s * d 的形式中的 d 和 s
    long_long d = number - 1; // number 减去 1

    /*
    __builtin_ctz 是一个内建函数，用于计算一个无符号整数在二进制表示中尾部连续 0 的个数。"ctz" 是 "Count Trailing Zeros"
    （计算尾部零）的缩写。例如，如果输入是 8（二进制为 1000），则 __builtin_ctz(8) 返回 3，因为 8 的二进制表示在末尾有 3 个零。
    */
    long_long s = __builtin_ctz(d); // 使用内建函数计算 d 中末尾零的个数，即因子 2 的幂
    d >>= s; // 将 d 右移 s 位，得到分解后的 d

    // 初始化素性测试的结果为 true，假设 number 是素数
    bool result = true;

    // 使用 OpenMP 并行处理每个测试基
#pragma omp parallel for
    for (int i = 0; i < 7; ++i) {
        if (!result) continue; // 如果已经确定 number 不是素数，则跳过当前测试

        // 取出当前测试基
        long_long base = test_bases[i];
        // 计算 base^d % number，这是米勒-罗宾测试的关键步骤
        long_long x = modular_pow(base, d, number);

        // 如果 x 为 1 或 number-1，则当前测试基不能证明 number 是合数，继续下一个测试基
        if (x <= 1 || x == number - 1)
            continue;

        // 进一步检验，看是否存在证据表明 number 是合数
        for (int j = 0; j < s; ++j) {
            x = (__int128)x * x % number; // 计算 x^2 % number，使用 __int128 防止溢出

            // 如果在非最后一轮得到 number-1，那么 x 在下一轮会变成 1，可以直接跳出循环
            if (x == number - 1 && j != s - 1) {
                x = 1;
                break;
            }

            // 如果在中间某步得到了 1，但之前没有得到 number-1，说明 number 是合数
            if (x == 1) {
                result = false;
                break;
            }
        }

        // 检查最终 x 的值，如果不是 1，说明 number 是合数
        if (x != 1)
            result = false;
    }


    return result;
}

int main() {
    long_long number_to_test;
    cin >> number_to_test; // 输入要测试的数字
    if (number_to_test < 2) { // 如果数字小于 2，则输出 "No"
        cout << "No";
        return 0;
    }

    // 调用 is_prime 函数检查数字是否为素数，并输出相应的结果
    if (is_prime(number_to_test))
        cout << "Yes";
    else
        cout << "No";
    return 0;
}
