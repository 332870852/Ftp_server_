#pragma once
#include "CSockLx.h"
#include<string>

class CUser
{
	std::string m_szUser;
	std::string m_szPass;
	std::string m_szPath;
	std::string m_szCWD;
	std::string m_szFrom;
	int UnKnown();
	// 处理用户登陆协议
	int User(const char* sData);//处理用户登陆协议
	int Pass(const char* sData);//用户密码协议
	int Opts(const char* sData);//设置字符集编码格式
	int Pwd();//获取当前位置
	int Type(const char* sData);//设置与客户端相同的文件格式 
	int Cwd(const char* sData);
	int Pasv(const char* sData);//创建数据通道协议
	int List();//获取列表文件
	int Syst(const char* sData);
	int Noop();
	int Dele(const char* sData);
	int Rmd(const char* sData);
	int Mkd(const char* sData);
	int Rnfr(const char* sData);
	int Rnto(const char* sData);
	int Stro(const char* sData);//上传协议
	int Size(const char* sData);//要求下载文件的大小
	int Retr(const char* sData);//下载协议
	static void Replace(std::string& str);
	enum EType { A = 'A', I = 'I' };
	EType m_eType;//记录对方发送的是a还是i

	CSockLx m_sockp,m_sockat;//命令通道的收发套接字()
public:
	CUser();
	~CUser();
	CSockLx m_socka;// 命令通道的收发套接字
	int Work();
	int Protocal(char* sKey, const char* sData);
	
	
};

