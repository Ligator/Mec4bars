# Mec4bars

Es un programa que traza la ruta que dibuja mecanismos de 4 barras.

Se puede configurar:

* La longitud de cada eslabón
* El número de mecanismos
* La luz que incide en los mecanismos
* Transparencias para mecanismos no seleccionados

## Instalar

El programa no necesita instalación.
Para Windows solo ejecuta el "UTM_Mec4bars.exe" de la carpeta "Bin".
Para Mac OS X solo ejecuta el archivo "mec4bars" de la carpeta "mec4bars Bin".

## Compilar en Windows

Está escrito en DevC++ con las bibliotecas OpenGLUT y OpenGLUI. Se deben instalar en el siguiente orden:

* Instalar el IDE [DevC++ 4.9.9.2](http://www.bloodshed.net/devcpp.html).
* Instalar el DevPak GLUT de su [página](http://www.nigels.com/glt/devpak/) o de la carpeta "DevPak".
* Instalar el DevPak GLUI 2.36 de su [página](http://www.nigels.com/glt/devpak/) o de la carpeta "DevPak".
* Abrir el projecto, es el archivo "UTM_Mec4bars.dev" de la carpeta "scr Windows".
* Compilar y correr.

Con el programa puedes abrir los archivos de la carpeta "Examples", son ejemplos de mecanismos.

## Compilar en Mac OS X

Está escrito en DevC++ con los frameworks OpenGL y GLUT. Se deben instalar en el siguiente orden:

* Instalar el Xcode si no se tiene aún.
* Instalar la biblioteca "glui" usando Homebrew `> brew install glui`.
* Abrir el projecto, es el archivo "mec4bars.xcodeproj" de la carpeta "scr Mac OS X".
* Compilar y correr.

Con el programa puedes abrir los archivos de la carpeta "Examples", son ejemplos de mecanismos.

## Uso

Teclado:

| Tecla 				| Acción 							|
| ---------------------	| --------------------------------- |
| [Barra espaciadora] 	| Animar el mecanismo 				|
| [x] 					| Seleccionar otro mecanismo 		|
| [Supr]				| Borrar el mecanismo seleccionado 	|


Mouse:

| Botón 		| Acción 					|
| ------------- | ------------------------- |
| [Izquierdo] 	| Traslación de la escena 	|
| [Derecho] 	| Rotación de la escena 	|
