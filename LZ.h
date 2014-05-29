#pragma once
class LZ
{
public:
	LZ(CString path);
	~LZ();

	//压缩
	void LZ_ya();
	//解压
	void LZ_jie();

private:
	CString path;	//文件路径	
};

