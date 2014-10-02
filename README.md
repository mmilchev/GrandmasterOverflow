GrandmasterOverflow
===================

A simple puzzle game using the EngineBase engine  

Compile instructions:
--------------------
Since the project uses the EngineBase library as a GIT submodule (which is another repository on its own), you need to make sure to download/clone it as well. There are two methods of doing this:  

I. Downloading as zip from GitHub  
After you've downloaded GrandmasterOverflow.zip and extracted the files from the archive, you'll need to download the EngineBase library as well. You can find it here:  

https://github.com/mmilchev/EngineBase  

You need to place it inside GrandmasterOverflow/EngineBase. After that, you can safely compile and run the game.  
  
II. Clone the repository  
This requires only one step, as long as you already have git installed on your machine. The following command will do it (Git 1.6.5 or higher):

git clone --recursive https://github.com/mmilchev/GrandmasterOverflow.git  

Alternatively, if you are cloning using a GUI, make sure to tick the recursive checkbox.  

Launching the game:
------------------
  
Open the GrandmasterOverflow.exe file. You can change some settings from the MainConfig.ini file.  
  
Gameplay instructions:
---------------------
The game has a build-in tutorial. I recommend jumping straight into the game, rather than continue reading here. Nevertheless, I will explain what the game is about and how to play it here.  

Grandmaster Overflow is a puzzle game, in which you need to fill the board with tiles as much as you can. Mainly, that happens through navigating the path of 'flows'. A 'flow' is a tile, which spreads in 4 direction every turn and will destroy itself when there isn't anywhere to go. Furthermore, if 2 or more 'flows' meet, they will self destruct as well. You can place and remove blocks to control 'flow' movement. There are currently 23 levels to explore.  

