
#include "stdafx.h"
#include "RLE.h"
#include "stdlib.h";
#include <string>
using namespace std;



RLE::RLE(CString path)
{
	this->path = path;
}


RLE::~RLE()
{
}

void RLE::RLE_ya()
{
	unsigned char ReadBuffer[BUFFERSIZE-1] = {0};
	unsigned char WriteBuffer[2*(BUFFERSIZE)] = {0};

	//获取并合成文件名
	CString filePath = path.Left(path.ReverseFind('\\')+1);
	CString fileType = path.Mid(path.ReverseFind('.') + 1);
	WriteBuffer[0] = fileType.GetLength();	//拓展名长度
	for (int i = 1; i < WriteBuffer[0]+1; i++)
	{
		WriteBuffer[i] = fileType[i - 1];
	}

	CString fileName = path.Left(path.ReverseFind('.'));
	fileName = fileName.Mid(fileName.ReverseFind('\\') + 1);
	CString pathFileOut = filePath + fileName + ".RLE";

	//打开文件
	//文件
	CFile fileIn(path, CFile::modeRead);
	CFile fileOut;
	fileOut.Open(pathFileOut, CFile::modeCreate | CFile::modeReadWrite);
	fileOut.Write(WriteBuffer, WriteBuffer[0] + 1);		//写入拓展名
	


	//开始处理
	int readNum = 0;
	unsigned char flag;
	while ((readNum = fileIn.Read(ReadBuffer, BUFFERSIZE-1)) != 0)	
	{
		int num_all = 0, num_start = 0, num = 0;
		int writeNum = 0;		//记录写数据到第几个
		for (;;)
		{
			num = _RLE_main(flag, ReadBuffer, num_start,readNum);	//num返回有几个相同或不同
			if (flag == 0)	//不同,首位用1表示
			{
				WriteBuffer[writeNum] = num | 0x80;			//首位置1
				for (int i = writeNum + 1; i < writeNum + num + 1; i++)
				{
					WriteBuffer[i] = ReadBuffer[num_start + i - writeNum - 1];
				}
				writeNum = writeNum + num + 1;
			}
			else			//相同
			{
				WriteBuffer[writeNum] = num & 0x7F;			//首位置0
				WriteBuffer[writeNum + 1] = ReadBuffer[num_start];
				writeNum = writeNum + 2;
			}
			//更改num_start
			num_start += num;
			if (num_start == readNum)
				break;	//跳出循环
		}
		//输出数据
		fileOut.Write(WriteBuffer, writeNum);
	}



	fileOut.Close();
	fileIn.Close();
	return;
}

void RLE::RLE_jie()
{
	unsigned char ReadBuffer[BUFFERSIZE] = { 0 };
	unsigned char WriteBuffer[BUFFERSIZE] = { 0 };

	//获取并合成文件名
	CString filePath = path.Left(path.ReverseFind('\\') + 1);	//路径
	CString fileName = path.Left(path.ReverseFind('.'));
	fileName = fileName.Mid(fileName.ReverseFind('\\') + 1);	//文件名

	//打开文件
	//文件
	CFile fileIn(path, CFile::modeRead);
	//读入拓展名
	fileIn.Read(ReadBuffer, 1);

	int num = ReadBuffer[0];
	char *fileType = new char[num+1];
	for (int i = 0; i < num; i++)
	{
		fileIn.Read(ReadBuffer, 1);
		fileType[i] = ReadBuffer[0];
	}
	fileType[num] = '\0';

	CString strFileType;
	strFileType.Format(_T("%s"), CStringW(fileType));
	CString pathFileOut = filePath + fileName + _T("_解压.") + strFileType;
	CFile fileOut;
	fileOut.Open(pathFileOut, CFile::modeCreate | CFile::modeReadWrite);

	//开始解压
	int readNum = 0;
	int IfData = 0;
	int DataNum = 0;
	int dataWrite = 0;
	while ((readNum = fileIn.Read(ReadBuffer, BUFFERSIZE)) != 0)
	{
		int numRead = 0;
		for (int i = 0; i < readNum; i++)
		{
			if (IfData == 0)
			{
				DataNum = ReadBuffer[i];
				IfData = 1;
				dataWrite = 0;
			}
			else
			{
				if ((DataNum & 0x80) == 0x80)	//数据不同
				{
					WriteBuffer[numRead] = ReadBuffer[i];
					numRead++;
					if (numRead == BUFFERSIZE)
					{
						fileOut.Write(WriteBuffer, numRead);
						numRead = 0;
					}
					dataWrite++;
					if (dataWrite == (DataNum & 0x7F))
						IfData = 0;
				}
				else							//数据相同
				{
					for (int j = 0; j < (DataNum & 0x7F); j++)
					{
						WriteBuffer[numRead] = ReadBuffer[i];
						numRead++;
						if (numRead == BUFFERSIZE)
						{
							fileOut.Write(WriteBuffer, numRead);
							numRead = 0;
						}
					}
					IfData = 0;
				}
			}

		}
		if (numRead != 0)
			fileOut.Write(WriteBuffer, numRead);
	}
	fileOut.Close();
	fileIn.Close();
	return;
}
unsigned char RLE::_RLE_main(unsigned char &flag, unsigned char ReadBuffer[BUFFERSIZE - 1], unsigned char num_start, unsigned char readNum)
{
	int num = 1;
	//判断是相同还是不用
	if (num_start == (BUFFERSIZE - 1))		//只剩一个数
	{
		flag = 0;
		return num;
	}
	else
	{
		if (ReadBuffer[num_start] != ReadBuffer[num_start + 1])
			flag = 0;	//不同
		else
			flag = 1;	//相同
	}
	for (int i = 0 + num_start + 1; i < readNum;i++)
	{
		if (flag == 0)
		{
			if (ReadBuffer[i] != ReadBuffer[i - 1])
				num++;
			else
				return num - 1;
		}
		else
		{
			if (ReadBuffer[i] == ReadBuffer[i - 1])
				num++;
			else
				return num;
		}
	}

	return  num;
}


