#pragma once

const unsigned char BUFFERSIZE = 128;
class RLE
{
public:
	//��������
	RLE(CString path);
	~RLE();

	//ѹ��
	void RLE_ya();
	//��ѹ
	void RLE_jie();


private:
	CString path;	//�ļ�·��	


	//RLE �ؼ�����������ֵ���м���һ����һ����flag = 1��ʾ������ͬ��flagΪ0������ͬ
	//num_start��ʾ�ӵڼ�����ʼ
	unsigned char _RLE_main(unsigned char &flag, unsigned char ReadBuffer[BUFFERSIZE - 1], unsigned char num_start, unsigned char readNum);
};

