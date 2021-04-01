# Portfolio
  SampleGame, SampleLibrary, SampleServer.

* SampleGame  
  
  1. Legend of zelda(imitation)  
  ![Legend of zelda](./SampleGame/LegendOfZelda/PlayScreenshot.jpg)  
  
  <젤다의 전설 모작>
  
  Windows API(GDI) 팀 포트폴리오  
  
  - 제작기간 : 1주일  
  - 담당 파트 : UI, 인벤토리  
  
  선택에 따라 인벤토리, 미니맵, 저장UI를 따로 출력.  
  UI 키 입력을 기다리고 있다가 키가 입력되면 본 게임 업데이트를 중지하고 UI만 업데이트하여 렌더.  
  UI화면으로 자연스럽게 전환되도록 화면비율에 따라 조금씩 스크롤 시킴.  
  입력받은 UI키가 월드맵이었다면 월드맵만 업데이트하도록 구현.  
  부분확대 및 방향키로 표시지역 변경 가능.  
  월드맵 이미지를 128층으로 나누어 각 층마다 조금씩 확대해 출력하는 기법으로 오목거울에 비친 것처럼 표현.  
  
- 녹화영상 : https://youtu.be/FyDCROxhJNg  
  
  2. Disgaea2(imitation)  
  ![Disgaea2](./SampleGame/Disgaea2/PlayScreenshot.jpg)  
  
  <디스가이아2 모작>
  
  Windows API(GDI) 팀 포트폴리오  
  
  - 제작기간 : 1주일  
  - 담당파트 : A*, 적 캐릭터, 아군 캐릭터  
  
  에너미, 플레이어 모두 파일명과 크기가 일정 양식에 맞춰 준비된 그림 파일을 읽어 종류에 따라 자동으로 분류 로드되게 구현.  
  외부에서 설정시킨 캐릭터 액션 상태값에 따라 캐릭터별로 업데이트 함수 실행결과가 달라지도록 구현.  
  캐릭터별로 설정된 이동가능거리에 따라 목적지가 될 수 있는 타일을 재귀적으로 수집.  
  수집된 타일 목록 내에서 A* 를 이용하여 이동가능 타일 경로 검색.  
  
- 녹화영상 : https://youtu.be/FDuLjguT34E  
  
  3. Mabinogi Heroes(imitation)  
  ![Mabinogi Heroes](./SampleGame/MabinogiHeroes/PlayScreenshot.jpg)  
  
  <마비노기 영웅전 모작>  
  
  DirectX 9 팀 포트폴리오  
  
  - 제작기간 : 4주일  
  - 담당파트 : 리소스 탐색 및 추출, 월드맵, NPC, 셰이더  
  
  구체에 텍스쳐를 입혀 스카이맵 구현.  
  DirectX9 기본 기능 이용.  
  Height map 기법으로 지형정보를 생성하고 건물 모델을 배치해 맵 완성.  
  그림자 매핑 방식으로 그림자 표현.  
  깊이버퍼의 데이터를 가져와 그림자 영역인지를 판정하고 하이트맵의 텍스쳐 rgb값을 조정.  
  
- 녹화영상 : https://youtu.be/sx2sdj-TO7w  
  
* SampleLibrary  
  Custom library. Excluded incomplete library from ./SampleServer/SilentNetworkLibrary.
  
* SampleServer  
  Custom Windows IOCP server. Client and database are not included.

  1. IntegratedApplicationProject  
  Project for practice and study. It contains LoginServer, ChatServer, MonitorServer, etc.

  2. MMO_Server  
  Game server for MMO. Lock-free linked stack for IOCP applied.  
  ![AllServer](./SampleServer/TestingEXE/AllServer.png)
  ![GameServer&Dummy](./SampleServer/TestingEXE/GameServer&Dummy.png)
  ![GameServer](./SampleServer/TestingEXE/GameServer.png) ![MonitorServer](./SampleServer/TestingEXE/MonitorServer.png)
  
  3. SilentNetworkLibrary  
  Static library used in the project above.
