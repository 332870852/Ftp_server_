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
		char* sKey = strtok(s, " \r");//���
		if (sKey)
		{
			_strupr(sKey);//ת��д
		}
		cout << "sKey" << sKey << endl;
		char* sData = strtok(NULL, " \r\n");
		Protocal(sKey, sData); //����Э�����
	}

	return 0;
}


int CUser::Protocal(char* sKey, const char* sData)
{
	if (!strcmp("USER", sKey))
		return User(sData);  //�˻�Э��
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
		return Mkd(sData);//�½��ļ���
	if (!strcmp("RNFR", sKey))
		return Rnfr(sData);//
	if (!strcmp("RNTO", sKey))
		return Rnto(sData);//
	if (!strcmp("RMD", sKey))
		return Rmd(sData);//ɾ�����ļ����ļ��е�Ŀ¼
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
	LPCSTR sRet = "502 δ֪Э�鴦��\r\n";
	m_socka.Send(sRet, sizeof(sRet));
	return TRUE;
}


// �����û���½Э��
int CUser::User(const char* sData)
{
	string str;
	if (theApp.FindUser(sData))
	{
		m_szUser = sData;
		str = "331 " + m_szUser + "��½�ɹ���\r\n";
	}
	else
		str = "530 ���벻ƥ�䣡\r\n";

	return m_socka.Send(str.c_str(), str.length())>0;

}

int CUser::Pass(const char * sData)
{
	string str;
	if (theApp.Login(m_szUser.c_str(),sData, m_szPath))
	{
		str = "230 �û���½�ɹ�:" + m_szUser + "\r\n";
	}
	else
	{
		str = "530 �û������ڣ�\r\n";
	}
	return m_socka.Send(str.c_str(), str.length())>0;

}

int CUser::Opts(const char * sData)
{
	const char* sSucc = "502 �����Ѿ����óɹ�\r\n";  //502 �����Ѿ����óɹ�
	m_socka.Send(sSucc, strlen(sSucc));
	return TRUE;
}

int CUser::Pwd()
{
	string str = "257 \"" + m_szCWD + "\"�ǵ�ǰ��Ŀ¼\r\n";  //
	return m_socka.Send(str.c_str(), str.length())>0;
}

int CUser::Type(const char * sData)
{
	if (!sData)
		return -1;
	m_eType = (EType)sData[0];//'i'.'a'

	string str;
	if (m_eType == A)//fopen("","rt")
		str = "200 �ı�ģʽ�Ѿ�����\r\n";
	else
		str= "200 ������ģʽ�Ѿ�����\r\n";

	return m_socka.Send(str.c_str(), str.length())>0;
	 
}

int CUser::Cwd(const char * sData)
{
	//m_szCWD = "";//�������
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
	str = "250 \"/" + m_szCWD + "\"��·���Ѿ����ú�\r\n";
	m_socka.Send(str.c_str(), str.length());

	return TRUE;
}

int CUser::Pasv(const char * sData)
{
	m_sockp.Close();
	if (!m_sockp.Create(16669, 1,"0.0.0.0"))//  10.34.9.79
	{
		cout <<"PASVʧ�ܣ�"<< GetLastError() << endl;
		const char* sSucc = "425 ����ͨ������ʧ�ܣ�\r\n";
		return m_socka.Send(sSucc, strlen(sSucc))>0;
	}
	char szIP[20];
	UINT nPort;
	m_socka.GetsockName(szIP, nPort);//��ȡ����IP
	strcpy_s(szIP, "103.46.128.51");								 //strcpy_s(szIP, "103.46.128.47");//...

	char* p = szIP;
	while (p = strchr(p, '.'))
		*p = ',';
	string str = szIP;
	str += ',';
	m_sockp.GetsockName(szIP, nPort);//��ȡ����IP
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
	str = m_szPath + m_szCWD + "*.*";//���û��ĸ�+���·��+*.*
	Replace(str);//��/����'\'
	long lf = _findfirst(str.c_str(), &fd);
	str = "150 File status okay\r\n";//��Ҫ�����ˣ��뿪ʼ����
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
	if (remove(str.c_str()) < 0)//-1��ʾɾ��ʧ��
		str += "553 ɾ��ʧ�ܣ�\r\n";
	else
		str += "250 ɾ���ɹ�! \r\n";
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
		str = "250 �ļ�ɾ���ɹ���\r\n";
	}
	else
		str = "553 �ļ�ɾ��ʧ�ܣ�("+m_szCWD+sData+")\r\n";
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
	string str = "250 �Ѿ�׼���ø�����\r\n";
	return m_socka.Send(str.c_str(), str.length())>0;
}

int CUser::Rnto(const char * sData)
{
	string szOld = m_szPath + m_szCWD + m_szFrom;
	string szNew = m_szPath + m_szCWD + sData;
	string str;
	if (!rename(szOld.c_str(), szNew.c_str()))
		str = "250 �����ɹ���\r\n";
	else
		str = "553 ����ʧ�ܣ�\r\n";
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
		sRet = "426 ���ļ�ʧ��!\r\n";
		return m_socka.Send(sRet, sizeof(sRet))>0;
	}
	
	sRet = "150 ׼��������ʼ����!\r\n";
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

	sRet = "226 �ļ����ճɹ�!\r\n";
	return m_socka.Send(sRet, sizeof(sRet))>0;
}
/*
SIZE ��ȡ�ļ�����
����ļ����ڷ�213Э�飬������ֳ��ȡ�������
����ļ�������Ҳ��213Э�飬���������0[����]*/
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
	sprintf(s, "213 %u������ļ��еĳ���\r\n", size);

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
		sRet = "426 ���ļ�ʧ��!\r\n";
		return m_socka.Send(sRet, strlen(sRet))>0;
	}

	sRet = "150 ׼��������ʼ����!\r\n";
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
	sRet = "226 �ļ����ͳɹ�!\r\n";
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
