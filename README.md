# Labor 9

## OpenGL
A mai labororán az OpenGL-t fogjuk használni s ezzel fogunk elemenseket megjeleníteni, mozgatni. Az OpenGL (Open Graphics Library) egy részletesen kidolgozott szabvány. Olyan API-t takar, amely segítségével egy egyszerű, szabványos felületen keresztül megvalósítható a grafikus kártya kezelése és a háromdimenziós grafika programozása. Az interfész több ezer különböző függvényhívásból áll, melynek segítségével a programozók szinte közvetlenül vezérelhetik a grafikus kártyát, segítségükkel 3 dimenziós alakzatokat rajzolhatnak ki, és a kirajzolás módját szabályozhatják. Széleskörűen használják  a virtuális valóság létrehozatalában, a tudományos élet különböző területein, és szimulátorok esetében is.

### OpenGL összekötése QT-vel

A projektfájlt (```.pro```) egészítsük ki a következőkkel: 
```QT += opengl```
, illetve ```LIBS += -lOpengl32```
A headerbe írjuk be az alábbi könyvtárakat:
```
#include <QOpenGLWindow>
#include <QtOpenGL>
#include <GL/glu.h>
```


## Feladatok
1. Írjunk egy programot, mely OpenGL segítségével megjelenít és kiszínez egy téglalapot és egy háromszöget.
![1](https://user-images.githubusercontent.com/78269344/114312903-08cd2e80-9afd-11eb-8450-a37bcb4f04fd.png)

2. Írjunk egy programot, mely OpenGL segítségével megjelenít és kiszínez egy kockát.
   - Készítsük el mind a 6 oldalát, s lásssuk el más-más színnel.
   - Ezek után forgassuk meg az alakzatunkat.

![2](https://i.ibb.co/jRJWn9X/kocka.png)

A perspektíva beállításához segédfüggvény: 
``` cpp
void glPerspective( GLdouble fov, GLdouble aspectRatio, GLdouble zNear, GLdouble zFar )
{
        GLdouble rFov = fov * 3.14159265 / 180.0;
        glFrustum( -zNear * tan( rFov / 2.0 ) * aspectRatio,
                  zNear * tan( rFov / 2.0 ) * aspectRatio,
                  -zNear * tan( rFov / 2.0 ),
                  zNear * tan( rFov / 2.0 ),
                  zNear, zFar );
}
```

3. Írjunk egy programot, mely OpenGL segítségével megjelenít egy megvilágított színes [tetraédert](https://hu.wikipedia.org/wiki/Tetraéder), melyet az „wasdqe” és „rt” billentyűk segítségével mozgatni és forgatni tudunk:
   - előre-hátra mozgatás (w-s)
   - jobbra-balra mozgatás (a-d)
   - fel-le mozgatás (q-e)
   - forgatás balra (r)
   - forgatás jobbra (t)

