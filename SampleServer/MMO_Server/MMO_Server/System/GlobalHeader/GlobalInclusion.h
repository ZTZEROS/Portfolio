#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "./System/MySQL/lib/libmysql.lib")
#pragma comment(lib, "./System/MySQL/lib/vs14/mysqlclient.lib")

#pragma comment(lib, "pdh.lib")

#include <malloc.h>
#include <memory.h>
#include <process.h>
#include <io.h>
#include <conio.h>

#include <stdio.h>
#include <stdlib.h>

//#include <assert.h>
//#include <ctype.h>
#include <direct.h>
//#include <errno.h>
//#include <fenv.h>
//#include <float.h>
//#include <inttypes.h>
//#include <iso646.h>
//#include <limits.h>
//#include <locale.h>
//#include <math.h>
//#include <setjmp.h>
//#include <signal.h>
//#include <stdarg.h>
//#include <stdbool.h>
//#include <stddef.h>
//#include <stdint.h>
//#include <string.h>
#include <strsafe.h>
//#include <tgmath.h>
#include <time.h>
#include <tchar.h>
//#include <uchar.h>
//#include <wchar.h>
//#include <wctype.h>
//#include <mmsystem.h>

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <ws2def.h>

#include <iostream>
//#include <array>
//#include <bitset>
//#include <queue>
//#include <deque>
//#include <forward_list>
#include <list>
#include <map>
#include <vector>
//#include <set>
//#include <stack>
//#include <hash_map>
#include <unordered_map>
#include <unordered_set>

//#include <atomic>
//#include <condition_variable>
//#include <future>
//#include <mutex>
//#include <thread>

//#include <algorithm>
//#include <chrono>
//#include <codecvt>
//#include <complex>
//#include <exception>
//#include <functional>
//#include <initializer_list>
//#include <iterator>
//#include <limits>
//#include <locale>
//#include <memory>
//#include <new>
//#include <numeric>
//#include <random>
//#include <ratio>
//#include <regex>
//#include <stdexcept>
#include <string>
//#include <system_error>
//#include <tuple>
//#include <typeindex>
#include <typeinfo>
//#include <type_traits>
//#include <utility>
//#include <valarray>

#include <sql.h>
#include <sqlext.h>

#include <Pdh.h>
#include <PdhMsg.h>

#include "GlobalTypeDefinition.h"
#include "GlobalDefinition.h"
#include "GlobalEnumeration.h"
#include "GlobalUnion.h"
#include "GlobalStructure.h"
//#include "GlobalClass.h"
//#include "GlobalVariable.h"
//#include "GlobalConstant.h"

using namespace std;
//using namespace GlobalConstant;
//using namespace GlobalVariable;

#include "System\MySQL\include\mysql.h"
#include "System\MySQL\include\errmsg.h"

#include "System\Timer\Timer.h"
#include "System\Astar\Astar.h"
#include "System\JumpPointSearch\JumpPointSearch.h"
#include "System\Log\Log.h"
#include "System\MemoryDumper\MemoryDumper.h"
#include "System\MemoryAllocator\MemoryAllocator.h"
#include "System\Profiler\Profiler.h"
#include "System\Profiler\CPU_Profiler.h"
#include "System\Profiler\PDH_Profiler.h"

//#include "System\Parser\Parser.h"
#include "System\Exception\Exception.h"
#include "System\Pool\Pool.h"
#include "System\Pool\LockFreePool.h"
#include "System\Pool\ThreadLocalChunk.h"
#include "System\Pool\ThreadLocalPool.h"
#include "System\Queue\Queue.h"
#include "System\Queue\ByteQueue.h"
//#include "System\Queue\CircularByteQueue.h"
#include "System\Queue\SerialByteQueue.h"
#include "System\Queue\SerialPacketQueue.h"
//#include "System\Queue\DoubleByteQueue.h"
#include "System\Queue\LockFreeQueue.h"
#include "System\Stack\LockFreeStack.h"

#include "System\DatabaseHandler\DatabaseHandler.h"
#include "System\DatabaseHandler\ThreadLocalDatabaseHandler.h"

#include "System\Network\IOCP\IOCP_Server\IOCP_Server.h"
#include "System\Network\IOCP\IOCP_Client\IOCP_Client.h"
#include "System\Network\IOCP\IOCP_InnerServer\IOCP_InnerServer.h"
#include "System\Network\IOCP\IOCP_OuterServer\IOCP_OuterServer.h"

#include "System\rapidjson\document.h"

using namespace rapidjson;