#include "User.h"
#include<iostream>
#include"FtpApp.h"
#include<io.h>
#include<direct.h>
using namespace std;


CUser::CUser()
{
	m_szCWD = '/';
}


CUser::~CUser()
{
}


int CUser::Work()
{
	char s[2048];
	int n = 0;
	while (true)
	{
		n = m_socka.Receive(s, sizeof(s));
		if (n <= 0)
			break;
		s[n] = 0;
		cout << s << endl;
		char* sKey = strtok(s, " \r");//拆分
		if (sKey)
		{
			_strupr(sKey);//转大写
		}
		cout << "sKey" << sKey << endl;
		char* sData = strtok(NULL, " \r\n");
		Protocal(sKey, sData); //进行协议解析
	}

	return 0;
}


int CUser::Protocal(char* sKey, const char* sData)
{
	if (!strcmp("USER", sKey))
		return User(sData);  //账户协议
	if (!strcmp("PASS", sKey))
		return Pass(sData);
	if (!strcmp("OPTS", sKey))
		return Opts(sData);
	if (!strcmp("PWD", sKey))
		return Pwd();
	if (!strcmp("TYPE", sKey))
		return Type(sData);
	if (!strcmp("CWD", sKey))
		return Cwd(sData);
	if (!strcmp("PASV", sKey))
		return Pasv(sData);
	if (!strcmp("LIST", sKey))
		return List();
	if (!strcmp("SYST", sKey))
		return Syst(sData);
	if (!strcmp("NOOP", sKey))
		return Noop();
	if (!strcmp("DELE", sKey))
		return Dele(sData);
	if (!strcmp("MKD", sKey))
		return Mkd(sData);//新建文件夹
	if (!strcmp("RNFR", sKey))
		return Rnfr(sData);//
	if (!strcmp("RNTO", sKey))
		return Rnto(sData);//
	if (!strcmp("RMD", sKey))
		return Rmd(sData);//删除含文件和文件夹的目录
	if (!strcmp("STOR", sKey))
		return Stro(sData);
	if (!strcmp("SIZE", sKey))
		return Size(sData);
	if (!strcmp("RETR", sKey))
		return Retr(sData);
	return UnKnown();
}


int CUser::UnKnown()
{
	LPCSTR sRet = "502 未知协议处理\r\n";
	m_socka.Send(sRet, sizeof(sRet));
	return TRUE;
}


// 处理用户登陆协议
int CUser::User(const char* sData)
{
	string str;
	if (theApp.FindUser(sData))
	{
		m_szUser = sData;
		str = "331 " + m_szUser + "登陆成功！\r\n";
	}
	else
		str = "530 密码不匹配！\r\n";

	return m_socka.Send(str.c_str(), str.length())>0;

}

int CUser::Pass(const char * sData)
{
	string str;
	if (theApp.Login(m_szUser.c_str(),sData, m_szPath))
	{
		str = "230 用户登陆成功:" + m_szUser + "\r\n";
	}
	else
	{
		str = "530 用户不存在！\r\n";
	}
	return m_socka.Send(str.c_str(), str.length())>0;

}

int CUser::Opts(const char * sData)
{
	const char* sSucc = "502 编码已经设置成功\r\n";  //502 编码已经设置成功
	m_socka.Send(sSucc, strlen(sSucc));
	return TRUE;
}

int CUser::Pwd()
{
	string str = "257 \"" + m_szCWD + "\"是当前的目录\r\n";  //
	return m_socka.Send(str.c_str(), str.length())>0;
}

int CUser::Type(const char * sData)
{
	if (!sData)
		return -1;
	m_eType = (EType)sData[0];//'i'.'a'

	string str;
	if (m_eType == A)//fopen("","rt")
		str = "200 文本模式已经设置\r\n";
	else
		str= "200 二进制模式已经设置\r\n";

	return m_socka.Send(str.c_str(), str.length())>0;
	 
}

int CUser::Cwd(const char * sData)
{
	//m_szCWD = "";//清除内容
	if (!sData)
		return -1;
	string str(sData);
	if(str[0]=='/')
	m_szCWD = str;
	else
	{
		int n = m_szCWD.length();
		if (m_szCWD[n - 1] != '/')
			m_szCWD += '/';
		m_szCWD += str;
	}
	str = "250 \"/" + m_szCWD + "\"的路径已经设置好\r\n";
	m_socka.Send(str.c_str(), str.length());

	return TRUE;
}

int CUser::Pasv(const char * sData)
{
	m_sockp.Close();
	if (!m_sockp.Create(16669, 1,"0.0.0.0"))//  10.34.9.79
	{
		cout <<"PASV失败："<< GetLastError() << endl;
		const char* sSucc = "425 数据通道创建失败！\r\n";
		return m_socka.Send(sSucc, strlen(sSucc))>0;
	}
	char szIP[20];
	UINT nPort;
	m_socka.GetsockName(szIP, nPort);//获取本机IP
	strcpy_s(szIP, "103.46.128.51");								 //strcpy_s(szIP, "103.46.128.47");//...

	char* p = szIP;
	while (p = strchr(p, '.'))
		*p = ',';
	string str = szIP;
	str += ',';
	m_sockp.GetsockName(szIP, nPort);//获取本机IP
	nPort = 56302;							 //nPort = 43036;//....
	sprintf(szIP, "%d", HIBYTE(nPort));
	str += szIP;
	sprintf(szIP, "%d", LOBYTE(nPort));
	str += ',';
	str += szIP;
	str = "227 Entering Passive Mode(" + str + ")\r\n";
	//str = "227 Entering Passive Mode (103,46,128,47,43036,50)\r\n";
	m_socka.Send(str.c_str(), str.length());
	m_sockp.Listen();
	m_sockp.Accept(m_sockat);
	return TRUE;
}

int CUser::List()
{
	string str;
	_finddata_t fd;
	str = m_szPath + m_szCWD + "*.*";//该用户的根+相对路径+*.*
	Replace(str);//正/换成'\'
	long lf = _findfirst(str.c_str(), &fd);
	str = "150 File status okay\r\n";//我要发送了，请开始接受
	m_socka.Send(str.c_str(), str.length());
	if (-1 != lf)
	{
		do
		{
			str = (fd.attrib& _A_SUBDIR) ? 'd' : '-';
			str += "rwxrwxrwx 1 ";
			str += "test ";
			str += " nogroup ";
			char szSize[15];
			sprintf(szSize, "%d ", fd.size);
			str += szSize;
			str += "Sep 2 02:20 ";
			str += fd.name;
			str += "\r\n";
			m_sockat.Send(str.c_str(), str.length());
			cout << "send: " << str << endl;
		} while (!_findnext(lf, &fd));
		_findclose(lf);

	}
	m_sockat.Close();
	m_sockp.Close();
	str = "226 Transfer finished successfully. Data connection closed\r\n";
	return m_socka.Send(str.c_str(), str.length())>0;

	
}

int CUser::Syst(const char * sData)
{
#ifdef _WIN32
	LPCSTR str = "215 Windows\r\n";
#else
	LPCSTR str = "215 Linux\r\n";
#endif
	m_socka.Send(str, strlen(str));
	return TRUE;
}

int CUser::Noop()
{
	//LPCSTR str = "200 Command Ok, NOOP\r\n";
	//m_socka.Send(str, sizeof(str));
	string str = "200 Command Ok, NOOP\r\n";
	m_socka.Send(str.c_str(), str.length());
	return TRUE;
}

int CUser::Dele(const char * sData)
{
	string str = m_szPath;
	str += sData;
	if (remove(str.c_str()) < 0)//-1表示删除失败
		str += "553 删除失败！\r\n";
	else
		str += "250 删除成功! \r\n";
	return m_socka.Send(str.c_str(), sizeof(str))>0;
}

int CUser::Rmd(const char * sData)
{
	if (!sData)
		return -1;
	string str = m_szPath + m_szCWD + sData;
	Replace(str);
	if (!_rmdir(str.c_str()))
	{
		str = "250 文件删除成功！\r\n";
	}
	else
		str = "553 文件删除失败！("+m_szCWD+sData+")\r\n";
	return m_socka.Send(str.c_str(), str.length())>0;
}

int CUser::Mkd(const char * sData)
{
	if (!sData)
		return -1;
	string str = m_szPath + m_szCWD + sData;
	Replace(str);
	if (_mkdir(str.c_str()) < 0)
		str = "553 Create directory failed.\r\n";
	else
		str = "250 Create directory success.\r\n";
	return m_socka.Send(str.c_str(), str.length()) > 0;
}

int CUser::Rnfr(const char * sData)
{
	m_szFrom = sData;
	string str = "250 已经准备好改名！\r\n";
	return m_socka.Send(str.c_str(), str.length())>0;
}

int CUser::Rnto(const char * sData)
{
	string szOld = m_szPath + m_szCWD + m_szFrom;
	string szNew = m_szPath + m_szCWD + sData;
	string str;
	if (!rename(szOld.c_str(), szNew.c_str()))
		str = "250 改名成功！\r\n";
	else
		str = "553 改名失败！\r\n";
	return m_socka.Send(str.c_str(),str.length())>0;
}

int CUser::Stro(const char * sData)
{
	string str = m_szPath + m_szCWD + sData;
	Replace(str);
	FILE* fp = fopen(str.c_str(), m_eType == 'I' ? "wb" : "w");
	const char* sRet = NULL;
	if (!fp)
	{
		sRet = "426 打开文件失败!\r\n";
		return m_socka.Send(sRet, sizeof(sRet))>0;
	}
	
	sRet = "150 准备就绪开始发送!\r\n";
	m_socka.Send(sRet, strlen(sRet));

	int n = 0;
	char s[2048];	
 	while ((n = m_sockat.Receive(s, sizeof(s))) > 0)
	{	
		fwrite(s, 1, n, fp);
	}

	fclose(fp);
	m_sockat.Close();
	m_sockp.Close();

	sRet = "226 文件接收成功!\r\n";
	return m_socka.Send(sRet, sizeof(sRet))>0;
}
/*
SIZE 获取文件长度
如果文件存在发213协议，后跟数字长度【描述】
如果文件不存在也发213协议，后面跟数字0[描述]*/
int CUser::Size(const char * sData)
{
	string str = m_szPath + m_szCWD + sData;
	Replace(str);
	_finddata_t fd;
	DWORD size = 0; 
	int nRet = _findfirst(str.c_str(), &fd);
	if (nRet >= 0)
	{
		size = fd.size;
		_findclose(nRet);
	}
	char s[256];
	sprintf(s, "213 %u是这个文件夹的长度\r\n", size);

	return m_socka.Send(s,strlen(s))>0;
}

int CUser::Retr(const char * sData)
{
	string str = m_szPath + m_szCWD + sData;
	Replace(str);
	FILE* fp = fopen(str.c_str(), m_eType == 'I' ? "rb" : "r");
	const char* sRet = NULL;
	if (!fp)
	{
		sRet = "426 打开文件失败!\r\n";
		return m_socka.Send(sRet, strlen(sRet))>0;
	}

	sRet = "150 准备就绪开始发送!\r\n";
	m_socka.Send(sRet, strlen(sRet));
	int n = 0;
	char s[2048];
	while ((n = fread(s, 1, sizeof(s), fp)) > 0)
	{
		m_sockat.Send(s, n);
	}
	fclose(fp);
	m_sockat.Close();
	m_sockp.Close();
	sRet = "226 文件发送成功!\r\n";
	return m_socka.Send(sRet, strlen(sRet))>0;
}

void CUser::Replace(std::string & str)
{
	int i = 0;
	while (true)
	{
		i = str.find('/', 0);
		if (i < 0)
			return;
		str[i] = '\\';
	}
}
