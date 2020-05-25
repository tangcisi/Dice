#pragma once
#include <vector>
#include <string>

void show(void* t, int len);

class Unpack final
{
	std::vector<unsigned char> buff;
public:
	Unpack();
	explicit Unpack(const char*);
	explicit Unpack(std::vector<unsigned char>);
	explicit Unpack(const std::string&);

	Unpack& setData(const char* i, int len);
	Unpack& clear();
	int len() const;

	Unpack& add(int i); //���һ������
	int getInt(); //����һ������

	Unpack& add(long long i); //���һ��������
	long long getLong(); //����һ��������

	Unpack& add(short i); //���һ��������
	short getshort(); //����һ��������

	Unpack& add(const unsigned char* i, short len); //���һ���ֽڼ�(����add(std::string i);)
	std::vector<unsigned char> getchars(); //����һ���ֽڼ�(����getstring();)

	Unpack& add(std::string i); //���һ���ַ���
	std::string getstring(); //����һ���ַ���

	Unpack& add(Unpack& i); //���һ��Unpack
	Unpack getUnpack(); //����һ��Unpack

	std::string getAll(); //���ر�������

	void show();
};
