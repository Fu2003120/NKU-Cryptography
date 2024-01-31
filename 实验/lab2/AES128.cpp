#include <iostream>  
#include <vector>    
#include <string>    
#include <algorithm> 
#include <cmath>     
#include <fstream>  
#include <sstream>   
#include <stdexcept>

using namespace std;

// AES-128加密类的定义
class AES128 {
private:
    static const int S_BOX[16][16];                                         // S_BOX
    static const long long ROUND_CONST[10];                                 // 密钥扩展轮常数
    static const int MOD = 256;                                             // MOD用于MixColumns步骤中的数学运算

    int CharToInt(char ch);                                                 // 将字符转换为整数（用于16进制转换）
    long long StrToLong(const string& str);                                 // 将16进制字符串转换为长整数
    string LongToStr(long long number);                                     // 将长整数转换为16进制字符串
    string RotateWord(const string& input);                                 // 将一个32-bit字循环左移8-bit
    string SubstituteWord(const string& inputWord);                         // 使用S_BOX进行字节替换
    string XorWithRoundConst(const string& input, int round);               // 与轮常数进行异或操作
    string TransformationFunction(const string& prevWord, int round);       // 密钥扩展中的转换函数
    string XorStrings(const string& s1, const string& s2);                  // 对两个字符串进行异或操作
    string IntToTwoChars(int num);                                          // 将整数转换为两个字符的16进制字符串
    vector<string> SplitKeyIntoGroups(const string& key);                   // 将128-bit的密钥分成四个32-bit的词
    vector<string> ExtendKey(const string& key);                            // 执行密钥扩展并生成所有轮密钥
    vector<string> ShiftRows(const vector<string>& state);                  // 执行ShiftRows操作
    vector<string> SplitState(const string& state);                         // 将128-bit的状态分成16个8-bit的字节
    vector<string> MixColumns(const vector<string>& state);                 // 执行MixColumns操作
    int MultiplyBy2(int num);                                               // 在GF(2^8)上将数字乘以2

public:
    void Encrypt(string& plaintext, string& key);                           // 主加密函数
};
const int AES128::S_BOX[16][16] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

const long long AES128::ROUND_CONST[10] = {
    0x01000000, 0x02000000,
    0x04000000, 0x08000000,
    0x10000000, 0x20000000,
    0x40000000, 0x80000000,
    0x1b000000, 0x36000000
};

// 将字符转换为整数：用于将16进制字符('0'-'9', 'a'-'f', 'A'-'F')转换为其对应的整数值。
int AES128::CharToInt(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';      // 如果是数字字符
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10; // 如果是小写字母
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10; // 如果是大写字母
    return 0;                                         // 其他情况
}

// 将16进制字符串转换为长整数。例如："1a" => 26
long long AES128::StrToLong(const string& str) {
    long long ans = 0;
    for (char ch : str)
        ans = (ans << 4) + CharToInt(ch); // 左移4位并加上当前字符对应的整数
    return ans;
}

// 将长整数转换为16进制字符串。例如：26 => "1a"
string AES128::LongToStr(long long number) {
    if (number == 0) return "0";
    static const char hexChars[] = "0123456789ABCDEF"; // 16进制字符映射
    string result; // 用于存储结果的字符串

    while (number) {
        char hexChar = hexChars[number & 0xf]; // 直接使用映射数组获取字符
        result = hexChar + result; // 在字符串前面插入字符
        number >>= 4; // 右移4位
    }

    return result;
}

// 将128-bit的密钥分成四个32-bit的词，密钥扩展步骤的开始部分。
vector<string> AES128::SplitKeyIntoGroups(const string& key) {
    const size_t groupSize = 8;
    const size_t groupCount = 4;
    vector<string> groups(groupCount);

    // 预分配内存以避免不必要的内存重分配
    for (size_t i = 0; i < groupCount; ++i)
        groups[i].reserve(groupSize);

    size_t index = 0;
    for (size_t i = 0; i < groupCount; ++i) {
        for (size_t j = 0; j < groupSize; ++j) {
            groups[i] += key[index];
            index++;
        }
    }
    return groups;
}

// 将一个32-bit字循环左移8-bit。例如："abcdef12" => "cdef12ab"
string AES128::RotateWord(const string& input) {
    return input.substr(2) + input.substr(0, 2);
}

// 使用S_BOX进行字节替换。
string AES128::SubstituteWord(const string& inputWord) {
    int length = inputWord.length();
    string result = "";
    for (int i = 0; i < length; i += 2) {
        int rowIndex = CharToInt(inputWord[i]);
        int colIndex = CharToInt(inputWord[i + 1]);
        int numValue = S_BOX[rowIndex][colIndex];     // 使用S_BOX查找替换值
        string numStr = IntToTwoChars(numValue);
        result += numStr;
    }
    return result;
}

// 与轮常数进行异或操作。主要用于密钥扩展中的轮常数加步骤。
string AES128::XorWithRoundConst(const string& input, int round) {
    long long number = StrToLong(input); // 将输入字符串转换为长整数
    number ^= ROUND_CONST[round];       // 与轮常数进行异或
    string result = LongToStr(number);
    while (result.length() < 8)
        result.insert(result.begin(), '0'); // 保证长度为8
    return result;
}

// 将整数转换为两个字符的16进制字符串。例如：26 => "1a"
string AES128::IntToTwoChars(int num) {
    string result = LongToStr(num);
    while (result.length() < 2)
        result = "0" + result;                       // 保证长度为2
    return result;
}

// 对两个字符串进行异或操作。
string AES128::XorStrings(const string& s1, const string& s2) {
    long long num1 = StrToLong(s1), num2 = StrToLong(s2);
    long long resultNum = num1 ^ num2;               // 对两个数字进行异或操作
    string result = LongToStr(resultNum);
    while (result.length() < 8)
        result = "0" + result;                       // 保证长度为8
    return result;
}

// 执行密钥扩展并生成所有轮密钥。
vector<string> AES128::ExtendKey(const string& key) {
    vector<string> extendedKey;
    extendedKey.reserve(44); // 预先分配足够的空间

    // 初始化扩展密钥列表，前4个词等于初始密钥
    for (int i = 0; i < 4; ++i)
        extendedKey.push_back(key.substr(i * 8, 8));

    for (int i = 4; i < 44; ++i) {
        string temp = extendedKey[i - 1];
        if (i % 4 == 0) {
            temp = TransformationFunction(temp, i / 4 - 1);
        }
        string newWord = XorStrings(temp, extendedKey[i - 4]);
        extendedKey.push_back(newWord);
    }

    return extendedKey;
}


// 执行ShiftRows操作，目的是对状态中的字节进行特定的置换。
vector<string> AES128::ShiftRows(const vector<string>& state) {
    vector<string> shiftedState = state;
    for (int i = 0; i < 4; ++i) {
        int offset = i * 2;
        for (int j = 0; j < 4; ++j) {
            shiftedState[j][offset] = state[(j + i) % 4][offset];
            shiftedState[j][offset + 1] = state[(j + i) % 4][offset + 1];
        }
    }
    return shiftedState;
}

// 将128-bit的状态分成16个8-bit的字节。
vector<string> AES128::SplitState(const string& state) {
    vector<string> result(state.length() / 2); // 预分配结果向量的大小

    for (size_t i = 0; i < result.size(); ++i) {
        result[i] = state.substr(i * 2, 2); // 每次提取2个字符
    }

    return result;
}

// 在GF(2^8)上将数字乘以2，GF(2^8)是一个有限域，这里使用了一个特定的多项式进行模运算。
int AES128::MultiplyBy2(int num) {
    int result = (num << 1) % MOD; // 将数字左移（乘以2）
    if (num & 0x80)                // 如果原数字的最高位是1
        result ^= 0x1b;            // 进行特定的异或操作
    return result;
}

// 密钥扩展中的转换函数。
string AES128::TransformationFunction(const string& prevWord, int round) {
    string result = RotateWord(prevWord);
    result = SubstituteWord(result);
    result = XorWithRoundConst(result, round);
    return result;
}

// 执行MixColumns操作，使用有限域上的乘法来混合状态中的数据。
vector<string> AES128::MixColumns(const vector<string>& state) {
    vector<string> mixedState = state;
    for (int i = 0; i < 4; ++i) {
        auto column = SplitState(state[i]); // 获取当前列的字节
        int s0 = StrToLong(column[0]), s1 = StrToLong(column[1]),
            s2 = StrToLong(column[2]), s3 = StrToLong(column[3]);

        // 进行有限域上的乘法和加法操作
        int t0 = MultiplyBy2(s0) ^ MultiplyBy2(s1) ^ s1 ^ s2 ^ s3;
        int t1 = s0 ^ MultiplyBy2(s1) ^ MultiplyBy2(s2) ^ s2 ^ s3;
        int t2 = s0 ^ s1 ^ MultiplyBy2(s2) ^ s3 ^ MultiplyBy2(s3);
        int t3 = s0 ^ MultiplyBy2(s0) ^ s1 ^ s2 ^ MultiplyBy2(s3);

        // 合并结果
        mixedState[i] = IntToTwoChars(t0) + IntToTwoChars(t1) +IntToTwoChars(t2) + IntToTwoChars(t3);
    }
    return mixedState;
}

// 主加密函数
void AES128::Encrypt(string& plaintext, string& key) {
    // 使用提供的密钥执行密钥扩展
    vector<string> keys = ExtendKey(key);
    int keyIndex = 0;

    // 将明文分为4个组（每组8个字符，即32位）
    vector<string> state = SplitKeyIntoGroups(plaintext);

    // 初始轮密钥加步骤
    for (int i = 0; i < 4; ++i)
        state[i] = XorStrings(state[i], keys[i]);

    keyIndex += 4;

    // 执行10轮AES加密
    for (int round = 0; round < 10; ++round) {
        // 执行字节替换（SubBytes）步骤
        for (int j = 0; j < 4; ++j)
            state[j] = SubstituteWord(state[j]);

        // 执行行移位（ShiftRows）步骤
        state = ShiftRows(state);

        // 在最后一轮之前执行列混淆（MixColumns）步骤
        if (round < 9)
            state = MixColumns(state);

        // 轮密钥加步骤
        for (int i = 0; i < 4; ++i)
            state[i] = XorStrings(state[i], keys[i + keyIndex]);

        keyIndex += 4; // 移动到下一组轮密钥
    }

    // 组合加密后的状态为最终的密文
    string ciphertext = "";
    for (int i = 0; i < 4; ++i)
        ciphertext += state[i];

    // 输出密文
    cout << ciphertext;
}

int main() {
    AES128 aes; 
    string key, text;
    cin >> key >> text;
    aes.Encrypt(text, key);
    return 0;
}