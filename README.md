Magic duels jest grą osadzoną w świecie fantasy gdzie gracze wcielają się w zwaśnionych magów.
Celem gry jest za pomocą zaklęć pokonać oponenta.
W prototypie może grać dwóch graczy na jednym ekranie w trybie deathmatch. 
W przyszłości można dodać więcej graczy oraz trybów gry. 
W grze zostały zaimplementowane takie mechaniki jak generowanie losowego terenu, dynamiczne niszczenie terenu,
zaklęcia oparte na systemach cząsteczkowych, własny system kolizji jak również obsługa kontrolerów.

////////////////////////////////

Aby skompilować projekt należy w pliku scons.ini wpisać jedną z poniższych linijek

;dla windows:
freetype=C:\\MinGW\\x86_64-w64-mingw32\\include\\freetype2 

;dla linuxa:
freetype=/usr/include/freetype2 

Należy również mieć zainstalowane następujące biblioteki:
-openGL
-GLEW
-SDL2
-SDL2-MIXER
-OpenGL Utility (GLU)
-freeimage
-assimp
-freetype
-GNU Offloading and Multi-Processing Project (GOMP)

trzeba mieć również zainstalowany SCons 

projekt kompileje się za pomocą komedy scons w terminalu 