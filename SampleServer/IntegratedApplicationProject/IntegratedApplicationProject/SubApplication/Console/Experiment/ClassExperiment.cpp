#include "stdafx.h"
#include "ClassExperiment.h"

int ExecuteClassExperiment(int argumentCount, char* argumentVector[])
{
	BaseClass* baseClassAddress;
	DerivedClass derivedClass;

	derivedClass.variable2 = 5;

	baseClassAddress = &derivedClass;

	baseClassAddress->variable2 = 6;

	printf("%d" CRALF, sizeof(*baseClassAddress)); //==sizeof(BaseClass)
	printf("%d" CRALF, sizeof(*(DerivedClass*)baseClassAddress)); //==sizeof(DerivedClass)
	printf("%d" CRALF, sizeof(derivedClass)); //==sizeof(DerivedClass)
	printf("%d" CRALF, baseClassAddress->variable2); //==BaseClass.variable2
	printf("%d" CRALF, ((DerivedClass*)baseClassAddress)->variable2); //==DerivedClass.variable2
	printf("%d" CRALF, derivedClass.variable2); //==DerivedClass.variable2

	CommonQueryPacket CQP;

	printf("%d", sizeof(CQP.Trailer)); // !=0 && ==1

	//DerivedClass::variable3 = 30;
	//StaticDerivedClass::variable3 = 30;
	
	char Queue[32];

	char* QueueAddress;


	QueueAddress = Queue;

	if (&QueueAddress[0] == Queue)
	{
		int a;

		a = 0;
	}

	Log gameLog;

	gameLog.WriteGameLog(L"test", LOG_LEVEL_DEBUG, L"XXXX %d XXXX %d XXXX %d", 1, 2, 3);

	LOG.WriteHexadecimalLog(L"test", LOG_LEVEL_DEBUG, L"Hexadecimal : ", (BYTE*)L"Hexadecimal", wcslen(L"Hexadecimal"));

	return 0;
}