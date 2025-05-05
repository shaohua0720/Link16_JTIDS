#pragma once
#include "Word.hpp"

//35bit报头
class HeaderWord : public Word<RS_Length::code_16_7, RS_Length::data_16_7> {
private:
	bitset<3>	m_type;	        //时隙类型(具体规定查表)
	bitset<1>	m_PR;		    //传输自由文本时，用于标识传输波形是双脉冲字符还是单脉冲字符
	bitset<15>	m_STN;	        //终端源航迹号，本时限消息的发送源编号
	bitset<16>	m_SDU;	        //保密数据单元，标识加密方式(目前暂时约定：前8位表示加密方式，后8位置位11001010)
	//AES:00001010  DES:00001011    RSA:00001111

public:
	explicit HeaderWord(bitset<15> STN) : Word(), m_type(bitset<3>("100")), m_PR(bitset<1>(0)),
		m_SDU(bitset<16>("0000101011001010")) {
		m_STN = STN;
	}

	HeaderWord(bitset<3> type, bitset<1> PR, bitset<15> STN) : Word(), m_SDU(bitset<16>("0000101011001010")) {
		m_type = type;
		m_PR = PR;
		m_STN = STN;
	}

	~HeaderWord() {}

	void rewrite(string& bit_str);
	void show();
	string toString();
	string toString_15B();
	void to_symbol();
	void setType(bitset<3> type);
	void setPR(bitset<1> PR);
	bitset<15> getSTN();
	bitset<16> getSDU();
	void setSTN(bitset<15> STN);
	void setSDU(bitset<16> SDU);
};
