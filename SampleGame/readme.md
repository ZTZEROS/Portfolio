# SampleGame

* LegendOfZelda  
  ![Legend of zelda](./LegendOfZelda/PlayScreenshot.jpg)  
    
  
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
  ![Disgaea2](./Disgaea2/PlayScreenshot.jpg)  
    
  
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
  ![Mabinogi Heroes](./MabinogiHeroes/PlayScreenshot.jpg)  
    
  
  This game is an imitation if 'Mabinogi Heroes'. It is the result of a team work for a month.  
  We used Windows API and DirectX9. My part was resource abstraction, world map, NPC, shader.  
  
  I implemented skymap by drawing texture on sphere. I helped by basic supporting of DirectX in many points.  
  I created the world map by generating geographical information by Height map technique, then locating building model.  
  For express shadow, I followed shadow mapping technique. Judge the area of shadow from the data of depth buffer, and adjust rgb value of the texture ofheight map.  
  
  https://youtu.be/sx2sdj-TO7w
