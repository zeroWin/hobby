#pragma once

const unsigned char BUFFERSIZE = 128;
class RLE
{
public:
	//构造析构
	RLE(CString path);
	~RLE();

	//压缩
	void RLE_ya();
	//解压
	void RLE_jie();


private:
	CString path;	//文件路径	


	//RLE 关键函数，返回值是有几个一样或不一样，flag = 1表示连续相同，flag为0连续不同
	//num_start表示从第几个开始
	unsigned char _RLE_main(unsigned char &flag, unsigned char ReadBuffer[BUFFERSIZE - 1], unsigned char num_start, unsigned char readNum);
};

