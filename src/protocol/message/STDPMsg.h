#pragma once
#include "core/utils/tools.h"
#include "word/HeaderWord.h"
#include "word/InitialWord.h"
#include "word/ExtendWord.h"
#include "word/ContinueWord.h"

using namespace link16::utils;

class STDPMsg {
private:
	string		m_raw_msg;
	string		m_bit_msg;

	HeaderWord* m_headerword;
	InitialWord* m_iword;
	ExtendWord* m_eword;
	ContinueWord* m_cword;

public:
	STDPMsg() : m_raw_msg("0"), m_bit_msg("0") {
		//TODO: 记得改回随机数
		m_headerword = new HeaderWord(bitset<15>(Tools::generateRandomBinary(15)));
		m_iword = new InitialWord();
		m_eword = new ExtendWord();
		m_cword = new ContinueWord();
	}

	~STDPMsg() {
		delete m_headerword;
		m_headerword = nullptr;
		delete m_iword;
		m_iword = nullptr;
		delete m_eword;
		m_eword = nullptr;
		delete m_cword;
		m_cword = nullptr;
	}

	HeaderWord* getHeaderWord();
	InitialWord* getInitialWord();
	ExtendWord* getExtendWord();
	ContinueWord* getContinueWord();
	void setRawMsg(string& raw_msg);
	void setBitMsg(string& bit_msg);
	void setHeaderWord(string& bit_data);
	void setInitialWord(string& bit_data);
	void setExtendWord(string& bit_data);
	void setContinueWord(string& bit_data);
	void clear();
};
