# Mec4bars

Es un programa que traza la ruta que dibuja mecanismos de 4 barras.

Se puede configurar:

* La longitud de cada eslabón
* El número de mecanismos
* La luz que incide en los mecanismos
* Transparencias para mecanismos no seleccionados

## Instalar

El programa no necesita instalación, solo ejecuta el "UTM_Mec4bars.exe" de la carpeta "Bin". Solo Windows.

## Compilar

El programa corre sobre Windows. Está escrito en DevC++ con las bibliotecas OpenGLUT y OpenGLUI. Se deben instalar en el siguiente orden:

* Instalar el IDE [DevC++ 4.9.9.2](http://www.bloodshed.net/devcpp.html).
* Instalar el DevPak GLUT de su [página](http://www.nigels.com/glt/devpak/) o de la carpeta "DevPak".
* Instalar el DevPak GLUI 2.36 de su [página](http://www.nigels.com/glt/devpak/) o de la carpeta "DevPak".
* Abrir el projecto, es el archivo "UTM_Mec4bars.dev" de la carpeta "Scr".
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
