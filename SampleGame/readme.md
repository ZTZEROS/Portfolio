# SampleGame

* LegendOfZelda
![Legend of zelda](./SampleGame/LegendOfZelda/PlayScreenshot.jpg)  
  
  This game is an imitation of 'Legend of zelda'. It is the result of a team work for a week.  
  We used Windows API(GDI) for implement, and my part was UI and inventory.  
  
  Following the input, Inventory, minimap, save menu are shown separately.  
  It checks UI key input every frame. When UI key input, pause main game update, then update only UI.  
  UI will be shown by scrolling for more natural game state switching.  
  If you input minimap(world map) UI key, main game update is stopped also and update only minimap.  
  You can zoom in and out the minimap, and explore other part of the world.  
  I used some trick to imitate original game. I sliced minimap view 128 pieces horizontally, then decreased the ratio of upper 64 pieces gradually, and increased the ratio of under 64 pieces. The minimap view is simillar to an image on concave mirror.  
  
  https://youtu.be/FyDCROxhJNg  
  
* Disgaea2  
 ![Disgaea2](./SampleGame/Disgaea2/PlayScreenshot.jpg)  
 
 This game is an imitation of 'Disgaea2'. It is the result of a team work for a week, too.  
 We used Windows API(GDI), and my part was A*(path finding algorithm), enemy character, player character.  
 
 All character resources of enemy and player are loaded automatically by sorted result. For this, we standardizated file name and size of resources.  
 By character action state value, each character is updated differently.  
 
 Path finding works following step.  
 First, check movable distance of the character.  
 Second, collect the data of the movable tile recursivly.  
 Third, in the list of collected tile, search movable path using A*.  
  
 https://youtu.be/FDuLjguT34E  
 
* Mabinogi Heroes  
 ![Mabinogi Heroes](./SampleGame/MabinogiHeroes/PlayScreenshot.jpg)  
   https://youtu.be/sx2sdj-TO7w
   
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
