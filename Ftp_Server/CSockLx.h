#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
class CSockLx
{
protected:
	SOCKET  m_hSocket;//���ĳ�Ա����
public:
	BOOL Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM, const char* lpszScoketAddress = NULL);//�˿ڣ���UDP��TCP�����ͣ�IP��ַ
	int SendTo(const void *lpBuf, int nBufLen, UINT nHostPort, LPCSTR lpszScocketAddress = NULL, int Flags = 0);//����
	int ReceiveFrom(void *lpBuf, int nBufLen, char *rScoketAddress, UINT &rScoketPort, int Flags = 0);//������Ϣ�ͶԷ�IP���˿�
	int Receive(void *lpBuf, int nBufLen, int nFlags = 0)//������Ϣ
	{
		return recv(m_hSocket, (char *)lpBuf, nBufLen, nFlags);
	}
	BOOL GetsockName(char *SocketAddress, UINT &SocketPort);//���ձ���IP�Ͷ˿���Ϣ
	BOOL GetpeerName(char *SocketAddress, UINT &SocketPort);//���ձ���IP�Ͷ˿���Ϣ��������TCP��
	BOOL Accept(CSockLx &rconnetedSocket, LPSTR szIP = NULL, UINT *nPort = NULL);
	BOOL Connect(LPCSTR lpszHostPort, UINT nHostPort);
	BOOL Listen(int nConnectionBacklog = 5)//����
	{
		return!listen(m_hSocket, nConnectionBacklog);
	}
	int Send(const void*lpBuf, int BufLen, int nFlags = 0)//����
	{
		return send(m_hSocket, (LPCSTR)lpBuf, BufLen, nFlags);
	}
	void Close()//�ر�
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
	SOCKET GetSock()
	{
		return m_hSocket;
	}
	CSockLx();
	~CSockLx();
};

