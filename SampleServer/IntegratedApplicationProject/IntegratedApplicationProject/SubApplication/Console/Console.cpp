#include "stdafx.h"
#include "Console.h"

int main(int argumentCount, char* argumentVector[])
{
	//ChatServer(argumentCount, argumentVector);
	//FM_Server(argumentCount, argumentVector);
	//FM_Client(argumentCount, argumentVector);
	//FighterServer(argumentCount, argumentVector);
	//FighterClient(argumentCount, argumentVector);
	//ExecuteIOCP_ChatServer(argumentCount, argumentVector);
	//ExecuteIOCP_ChatClient(argumentCount, argumentVector);
	//ExecuteHTTP_Client(argumentCount, argumentVector);
	//ExecuteODBC_Client(argumentCount, argumentVector);
	//ExecuteLanServer(argumentCount, argumentVector);
	//ExecuteAdvancedLAN_Server(argumentCount, argumentVector);
	//ExecuteAdvancedWAN_Server(argumentCount, argumentVector);
	//ExecuteAnotherLAN_Server(argumentCount, argumentVector);
	//ExecuteLockFreeMemoryPool(argumentCount, argumentVector);
	//ExecuteThreadLocalMemoryPool(argumentCount, argumentVector);
	
	//ExecuteAdvancedLoginServer(argumentCount, argumentVector);
	//ExecuteAdvancedChatServer(argumentCount, argumentVector);

	ExecuteMonitorServer(argumentCount, argumentVector);
	//ExecutePerformanceDataHelper(argumentCount, argumentVector);

	//MemoryCopyExperiment(argumentCount, argumentVector);
	//QueueExperiment(argumentCount, argumentVector);
	//ExecuteIOCP_Experiment(argumentCount, argumentVector);
	//ExecuteClassExperiment(argumentCount, argumentVector);
	
	//ExecuteJurassicWarServer(argumentCount, argumentVector);

	//ExecuteMySQL_Example(argumentCount, argumentVector);
	//ExecuteMySQL_Client(argumentCount, argumentVector);

	system("pause");

	return 0;
}

/*
1.����ť, ���ú�ť �߰����� ���� ���� �̿�
2.���ӻ��� üũ �ڵ�, ť ������ üũ �ڵ�
3.����ȭ���۷� ����

4.���ú� ��ƾ ����
5.classify Astar
6.classify JumpPointSearch
7.classify bresenhamLine
8.redblacktree
9.ChatClient
10.FriendManagementClient
11.StressTester
12.45000
13.no async 2000 connection
14.iocp
15.PHP with http RAW POST JSON, profilinglog, systemlog, errorlog
16.http��Ŷ �񵿱� ó��
17.SerialByteQueue�� �����͸� ��� SendQ�� �̿��� ������� ����������(���� �����Ҵ��ؼ� �޸� ����)
18.�ϳ��� ����ȭ���۷� ���� �������� ���� �� �ֵ��� ���۷���ī��Ʈ�� ���Ե� ��Ȱ�밡����,
 ����Ʈ������ó�� �ڵ�ȭ�� ����ȭ���۷� ����
19.������ �޸�Ǯ �ϼ�
20.������ ���� ����
21.������ ť
22. ������ �ϼ�
23. ������ ���� �������Ϸ�
23. ������ ���� �����ͺ��̽� �Ŵ���

24. WSARecv transferredSize 0 ó��
*/