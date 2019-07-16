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
	// �����û���½Э��
	int User(const char* sData);//�����û���½Э��
	int Pass(const char* sData);//�û�����Э��
	int Opts(const char* sData);//�����ַ��������ʽ
	int Pwd();//��ȡ��ǰλ��
	int Type(const char* sData);//������ͻ�����ͬ���ļ���ʽ 
	int Cwd(const char* sData);
	int Pasv(const char* sData);//��������ͨ��Э��
	int List();//��ȡ�б��ļ�
	int Syst(const char* sData);
	int Noop();
	int Dele(const char* sData);
	int Rmd(const char* sData);
	int Mkd(const char* sData);
	int Rnfr(const char* sData);
	int Rnto(const char* sData);
	int Stro(const char* sData);//�ϴ�Э��
	int Size(const char* sData);//Ҫ�������ļ��Ĵ�С
	int Retr(const char* sData);//����Э��
	static void Replace(std::string& str);
	enum EType { A = 'A', I = 'I' };
	EType m_eType;//��¼�Է����͵���a����i

	CSockLx m_sockp,m_sockat;//����ͨ�����շ��׽���()
public:
	CUser();
	~CUser();
	CSockLx m_socka;// ����ͨ�����շ��׽���
	int Work();
	int Protocal(char* sKey, const char* sData);
	
	
};

