#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "./System/MySQL/lib/libmysql.lib")
#pragma comment(lib, "./System/MySQL/lib/vs14/mysqlclient.lib")

#pragma comment(lib, "pdh.lib")

#include <memory.h>
#include <process.h>
#include <io.h>
#include <conio.h>

//#include <stdio.h>
//#include <stdlib.h>

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
//#include <uchar.h>
//#include <wchar.h>
//#include <wctype.h>
#include <mmsystem.h>

#include <Windows.h>
#include <WinSock2.h>
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

#include "..\MySQL\include\mysql.h"
#include "..\MySQL\include\errmsg.h"

#include "..\Timer\Timer.h"
#include "..\Astar\Astar.h"
#include "..\JumpPointSearch\JumpPointSearch.h"
#include "..\Log\Log.h"
#include "..\MemoryDumper\MemoryDumper.h"
#include "..\MemoryAllocator\MemoryAllocator.h"
#include "..\Profiler\Profiler.h"

#include "..\BitmapBackBuffer\BitmapBackBuffer.h"
#include "..\BitmapImage\BitmapImage.h"
#include "..\Animation\Animation.h"
//#include "..\Parser\Parser.h"
#include "..\Exception\Exception.h"
#include "..\Pool\Pool.h"
#include "..\Pool\LockFreePool.h"
#include "..\Pool\ThreadLocalChunk.h"
#include "..\Pool\ThreadLocalPool.h"
#include "..\Queue\Queue.h"
#include "..\Queue\ByteQueue.h"
//#include "..\Queue\CircularByteQueue.h"
#include "..\Queue\SerialByteQueue.h"
#include "..\Queue\SerialPacketQueue.h"
//#include "..\Queue\DoubleByteQueue.h"
#include "..\Queue\LockFreeQueue.h"
#include "..\Stack\LockFreeStack.h"

#include "..\DatabaseHandler\DatabaseHandler.h"
#include "..\DatabaseHandler\ThreadLocalDatabaseHandler.h"

#include "..\Network\IOCP\IOCP_Server\IOCP_Server.h"
#include "..\Network\IOCP\IOCP_InnerServer\IOCP_InnerServer.h"
#include "..\Network\IOCP\IOCP_OuterServer\IOCP_OuterServer.h"
#include "..\Network\IOCP\IOCP_Client\IOCP_Client.h"

#include "..\rapidjson\document.h"

using namespace rapidjson;