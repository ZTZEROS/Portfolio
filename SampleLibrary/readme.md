# SampleLibrary

* Astar  
  A* is a path-finding algorithm for tile map.  
  
  A* 는 타일맵에 사용하는 길찾기 알고리즘이다. 주변의 이동가능한 타일을 검색하고, 각 타일에 대해 시작점으로부터의 거리와 목적지까지의 거리를 저장한 뒤, 두 거리값을 합한 값이 가장 작은 타일에 대해서 직전 단계의 타일이 어느 타일인지 기록해 두고, 다시 맨 처음의 작업부터 반복한다. 목적지에 도달하면 작업을 종료하고 기록해 둔 이전 단계의 타일 정보들을 모아 경로를 구성하는 식으로 작동한다.
  
* Breseham  
  브레슨햄 알고리즘은 픽셀 단위로 된 디지털 화면상에서 선을 적당히 보기좋게 그리기 위한 알고리즘이다. 특별히 어려운 부분은 없으나, 선의 양 끝점 처리와 경사를 주는 지점의 계산을 꼼꼼하게 처리해 주어야 예외없이 일정하고 예쁜 선이 나온다.
  
* DatabaseHandler  
  MySQL에서 제공하는 함수들을 이용해 데이터베이스를 좀더 편하게 다루기 위해 만든 데이터베이스 조작기이다. 외부 설정파일로부터 데이터베이스의 IP Address와 UserName, Password를 읽어와 접속하고, 쿼리를 보내고, 원치 않은 상황에 대해 오류메세지를 출력하는 기본적인 기능이 구현되어 있다.  
  멀티스레드용 버전은 기능은 같으나 TLS를 이용하여 여러 스레드가 동시에 데이터베이스를 조작해도 문제가 없도록 만든 것이다.
  
* Exception
* GlobalHeader
* JumpPointSearch
* Log
* MemoryAllocator
* MemoryDumper
* Network
* Parser
* Profiler
* Queue
* Stack
* Timer
* Tree
