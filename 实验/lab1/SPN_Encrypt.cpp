#include <iostream>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

// S-Box: 用于分组替代操作的盒子，提供一个非线性的替代步骤。
vector<int> S_Box = {0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7};

// P-Box: 用于位混淆操作的盒子，它重新排列输入的位。
vector<int> P_Box = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

// 从32位的主密钥生成五个16位的子密钥。
vector<int> gen_K_list(unsigned int K) {
    vector<int> Ks(5, 0);
    for (int i = 4; i >= 0; --i) {
        Ks[i] = K & 0xFFFF;  // 获取K的最后16位
        K >>= 4;  // 右移4位
    }
    return Ks;
}

// 使用S-Box执行分组替代操作。
int pi_s(const vector<int>& s_box, int ur) {
    int vr = 0;
    for (int i = 0; i < 4; ++i) {
        int uri = ur & 0xF;  // 获取ur的最后4位
        int vri = s_box[uri];  // 从S-Box获取替代值
        vr += vri << (4 * i);  // 将结果存储到vr
        ur >>= 4;  // 右移4位
    }
    return vr;
}

// 使用P-Box执行位混淆操作。
int pi_p(const vector<int>& p_box, int vr) {
    int wr = 0;
    for (int i = 15; i >= 0; --i) {
        int vri = vr & 1;  // 获取vr的最后1位
        vr >>= 1;  // 右移1位
        wr |= vri << (16 - p_box[i]);  // 根据P-Box的值重新排列位
    }
    return wr;
}

// 实现SPN网络加密操作。
int do_SPN(int x, const vector<int>& s_box, const vector<int>& p_box, const vector<int>& Ks) {
    int wr = x;
    // 执行Nr-1轮的加密操作，每轮包括三个步骤：密钥加、分组替代、位混淆。
    for (int r = 0; r < 3; ++r) {
        int ur = wr ^ Ks[r];  // 密钥加
        int vr = pi_s(s_box, ur);  // 分组替代
        wr = pi_p(p_box, vr);  // 位混淆
    }
    // 最后一轮不包括位混淆。
    int ur = wr ^ Ks[3];  // 密钥加
    int vr = pi_s(s_box, ur);  // 分组替代
    return vr ^ Ks[4];  // 输出结果再次进行密钥加操作
}

// 根据给定的32位密钥对16位的明文进行加密。
int encrypt(unsigned int K, int x) {
    vector<int> Ks = gen_K_list(K);  // 生成子密钥
    return do_SPN(x, S_Box, P_Box, Ks);  // 执行SPN加密操作
}

int main() {
    int x;
    unsigned int K;
    string binStr;

    cin >> binStr;
    x = stoi(binStr, nullptr, 2);
    cin >> binStr;
    K = stoul(binStr, nullptr, 2);

    int encrypted = encrypt(K, x);

    cout << bitset<16>(encrypted) << endl;

    return 0;
}
