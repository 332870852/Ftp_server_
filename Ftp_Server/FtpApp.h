#pragma once
#include"CSockLx.h"
#include"User.h"
#include<list>
struct SUser
{
	char sUser[20];
	char sPass[20];
	char sPath[256];
};
class CFtpApp
{
	CSockLx m_sock;
	static void theProc(void* p)
	{
		const char* sWelc = "220 ��ӭʹ��FTP��������\r\n";  //220Э�飬��ʾ��ӭ��Ϣ����ӭ֮����ܷ���
		CUser* pUser = (CUser*)p;
		pUser->m_socka.Send(sWelc, strlen(sWelc));
		pUser->Work();
	}
	std::list<SUser>m_list;
public:
	CFtpApp();
	~CFtpApp();
	int Start();

	void Load();
	// �����û����Ƿ����
	bool FindUser(const char* sUser);
	bool Login(const char * sUser,const char* sPass, std::string& szPath);
};
extern CFtpApp theApp;

