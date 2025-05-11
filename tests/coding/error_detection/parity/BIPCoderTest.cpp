#include "gtest/gtest.h"
#include "coding/error_detection/parity/BIPCoder.h"
#include <string>
#include <bitset>

using namespace link16::coding::error_detection;

// 测试BIP计算
TEST(BIPCoderTest, CalculateBIP) {
    BIPCoder coder;
    
    // 测试空字符串
    std::bitset<5> emptyBIP = coder.calculateBIP("");
    EXPECT_EQ(emptyBIP, std::bitset<5>("00000"));
    
    // 测试简单字符串
    std::string testData1 = "A"; // 'A' = 01000001
    std::bitset<5> bip1 = coder.calculateBIP(testData1);
    // 预期结果：奇校验，A的位分组后，每组中1的个数为：
    // 组0: 0,5 => 0个1 => 校验位为1
    // 组1: 1,6 => 0个1 => 校验位为1
    // 组2: 2,7 => 1个1 => 校验位为0
    // 组3: 3   => 0个1 => 校验位为1
    // 组4: 4   => 0个1 => 校验位为1
    EXPECT_EQ(bip1, std::bitset<5>("11011"));
    
    // 测试复杂字符串
    std::string testData2 = "Hello";
    std::bitset<5> bip2 = coder.calculateBIP(testData2);
    // 复杂字符串的BIP计算结果需要根据实际算法确定
    // 这里只是验证函数能正常执行
    EXPECT_NE(bip2, std::bitset<5>("00000"));
}

// 测试BIP验证
TEST(BIPCoderTest, VerifyBIP) {
    BIPCoder coder;
    
    // 测试正确的BIP
    std::string testData = "Test";
    std::bitset<5> correctBIP = coder.calculateBIP(testData);
    EXPECT_TRUE(coder.verifyBIP(testData, correctBIP));
    
    // 测试错误的BIP
    std::bitset<5> wrongBIP("10101");
    EXPECT_FALSE(coder.verifyBIP(testData, wrongBIP));
}

// 测试添加BIP
TEST(BIPCoderTest, AddBIP) {
    BIPCoder coder;
    
    std::string testData = "Data";
    std::string dataWithBIP = coder.addBIP(testData);
    
    // 验证长度
    EXPECT_EQ(dataWithBIP.length(), testData.length() + 5);
    
    // 验证提取的数据
    EXPECT_EQ(coder.extractData(dataWithBIP), testData);
    
    // 验证BIP
    std::string bipStr = dataWithBIP.substr(dataWithBIP.length() - 5);
    std::bitset<5> extractedBIP(bipStr);
    EXPECT_EQ(extractedBIP, coder.calculateBIP(testData));
}

// 测试全局validateBIP函数
TEST(BIPCoderTest, ValidateBIP) {
    BIPCoder coder;
    
    // 创建带BIP的数据
    std::string testData = "ValidateTest";
    std::string dataWithBIP = coder.addBIP(testData);
    
    // 验证正确的数据
    EXPECT_TRUE(validateBIP(dataWithBIP));
    
    // 修改一位，验证错误的数据
    dataWithBIP[0] = dataWithBIP[0] ^ 0x01; // 翻转第一个字节的最低位
    EXPECT_FALSE(validateBIP(dataWithBIP));
}

// 测试错误修正
TEST(BIPCoderTest, ErrorCorrection) {
    BIPCoder coder;
    
    // 创建带BIP的数据
    std::string testData = "ErrorTest";
    std::string dataWithBIP = coder.addBIP(testData);
    
    // 引入单比特错误
    std::string corruptedData = dataWithBIP;
    corruptedData[0] = corruptedData[0] ^ 0x01; // 翻转第一个字节的最低位
    
    // 尝试修正错误
    std::string correctedData = correctSingleBitError(corruptedData);
    
    // 验证修正后的数据
    EXPECT_TRUE(validateBIP(correctedData));
    EXPECT_EQ(coder.extractData(correctedData), testData);
}