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
1.센드큐, 리시브큐 중간버퍼 없이 직접 이용
2.접속상태 체크 코드, 큐 사이즈 체크 코드
3.직렬화버퍼로 변경

4.리시브 루틴 수정
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
16.http패킷 비동기 처리
17.SerialByteQueue의 포인터를 담는 SendQ를 이용한 복사없는 데이터전송(필히 동적할당해서 메모리 관리)
18.하나의 직렬화버퍼로 여러 유저에게 보낼 수 있도록 레퍼런스카운트가 포함된 재활용가능한,
 스마트포인터처럼 자동화된 직렬화버퍼로 개선
19.락프리 메모리풀 완성
20.락프리 스택 수정
21.락프리 큐
22. 랜서버 완성
23. 스레드 로컬 프로파일러
23. 스레드 로컬 데이터베이스 매니저

24. WSARecv transferredSize 0 처리
*/