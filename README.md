Cómo correr el Proyecto Final (Equipo)

Esta guía explica paso a paso cómo dejar el proyecto funcionando en tu computadora, sin importar que tengas Windows. Sigue los pasos en orden, no te saltes ninguno.

El proyecto tiene 2 partes:
Parte 1: Shell Script (gestor.sh) — gestor de películas y series
Parte 2: Programa en C (hangman) — juego del ahorcado

1. Preparar tu computadora (WSL + Ubuntu)
Si ya tienes WSL con Ubuntu instalado, sáltate al paso 2.

1.1 Instalar WSL
Abre PowerShell como Administrador (botón derecho sobre PowerShell → "Ejecutar como administrador") y escribe:
powershellwsl --install
Espera a que termine de descargar (puede tardar varios minutos) y reinicia tu computadora cuando te lo pida.

1.2 Configurar Ubuntu
Al reiniciar, se abrirá solita una ventana de Ubuntu pidiéndote:
Un nombre de usuario (minúsculas, sin espacios)
Una contraseña (no se ve mientras escribes, es normal)

1.3 Instalar las herramientas necesarias
Dentro de la terminal de Ubuntu, instala el compilador de C y las herramientas básicas:

bashsudo apt update
sudo apt install build-essential -y

Con esto ya tienes gcc, make y todo lo necesario para correr ambas partes del proyecto.

2. Descargar el proyecto (clonar desde GitHub)

El proyecto está en este repositorio:
https://github.com/Val-264/proyecto_final_unix.git

2.1 Instalar Git (si no lo tienes)
Desde tu terminal Ubuntu:
bashgit --version

Si no te muestra un número de versión, instálalo:
bashsudo apt install git -y

2.2 Configurar tu nombre y correo (solo la primera vez)

bashgit config --global user.name "Tu Nombre"
git config --global user.email "tu_correo@ejemplo.com"

Usa el mismo correo de tu cuenta de GitHub.

2.3 Clonar el repositorio
Importante: clónalo dentro del sistema de archivos de Linux (tu carpeta personal de Ubuntu), no en C:\ ni en OneDrive, para evitar errores de permisos y de saltos de línea.

bashcd ~
git clone https://github.com/Val-264/proyecto_final_unix.git

Cuando termine, vas a tener una carpeta nueva:

bashcd ~/proyecto_final_unix
ls -la

Ahí deberías ver las carpetas Parte1_ShellScript y Parte2_C_Hangman.

2.4 Si el equipo actualiza el repositorio después
Si alguien más sube cambios nuevos, para traerlos a tu copia local:

bashcd ~/proyecto_final_unix
git pull origin main

(Si tu repositorio usa master en vez de main, usa git pull origin master)


3. Ejecutar la Parte 1 (Shell Script)

bashcd ~/proyecto_final_unix/Parte1_ShellScript

3.1 Arreglar permisos y formato (hacer esto siempre antes de la primera ejecución)

bashsed -i 's/\r$//' gestor.sh
chmod +x gestor.sh

Esto casi nunca hace falta si clonaste con git clone, pero es un paso de 2 segundos que evita el error más común (Permission denied o que el menú no responda bien) si alguien subió el archivo desde Windows en algún momento.

3.2 Ejecutar

bash./gestor.sh

Te debe salir un menú con las opciones de Alta, Baja, Consulta, Modificación y Reportes.


4. Ejecutar la Parte 2 (Hangman en C)

bashcd ~/proyecto_final_unix/Parte2_C_Hangman

4.1 Arreglar formato del archivo de palabras (precaución rápida)

bashsed -i 's/\r$//' palabras.txt

Si no haces esto y el archivo llegó a tener saltos de línea estilo Windows, el juego puede volverse imposible de ganar (queda un espacio invisible al final de la palabra).

4.2 Compilar

bashmake

Esto debe terminar sin errores y crear un archivo llamado hangman.

4.3 Ejecutar

bash./hangman

O en un solo paso:

bashmake run


5. Si quieres modificar el código

5.1 Editar con VS Code (recomendado)

Instala Visual Studio Code en Windows normal (como cualquier programa), y desde la terminal Ubuntu, estando dentro de la carpeta del proyecto:

bashcode .

La primera vez te pedirá instalar la extensión "WSL" — acéptala. Esto abre VS Code editando directamente los archivos de Linux, sin causar problemas de permisos.

5.2 Modificar la Parte 1 (gestor.sh)

El script está dividido en funciones, una por cada opción del menú:

FunciónQué hacealta_registro()Agrega un nuevo registro, con validacionesbaja_registro()Elimina un registro por IDconsulta_registros()Busca registros (todos, por ID, título o género)modificar_registro()Edita un campo de un registro existentemenu_reportes() y las funciones reporte_*()Los 5 reportes

Si agregas un campo nuevo al CSV (por ejemplo "Director"), tienes que:


Agregarlo en alta_registro() (pedirlo y validarlo)
Agregarlo en modificar_registro() (como nueva opción de edición)
Agregarlo en mostrar_tabla() (para que se vea en las consultas)


Después de cualquier cambio, vuelve a correr:

bashsed -i 's/\r$//' gestor.sh
chmod +x gestor.sh
./gestor.sh

5.3 Modificar la Parte 2 (Hangman)

ArchivoQué contienemain.cEl menú principal (Jugar / Salir)ahorcado.cToda la lógica del juegoahorcado.hLas declaraciones de funciones (si agregas una función nueva en ahorcado.c, también hay que declararla aquí)palabras.txtEl banco de palabras, una por línea

Para agregar más palabras, solo edita palabras.txt y agrega una palabra nueva por línea (en mayúsculas, sin acentos ni espacios).

Para cambiar el número de intentos permitidos, edita en ahorcado.h:

c#define MAX_INTENTOS 6

Después de cualquier cambio en los archivos .c o .h, hay que recompilar:

bashmake clean
make
./hangman

make clean borra la compilación vieja, y make vuelve a compilar con tus cambios. Si no haces make de nuevo, seguirás viendo el comportamiento anterior aunque hayas editado el código.


6. Errores comunes y solución rápida

Error que vesCausaSoluciónPermission denied al ejecutar ./gestor.shEl archivo no tiene permiso de ejecución, o es propiedad de "root"chmod +x gestor.sh — si sigue sin funcionar, revisa el dueño con ls -la y usa sudo chown tu_usuario:tu_usuario gestor.shEl script se comporta raro, opciones que no hacen nadaEl archivo tiene saltos de línea estilo Windowssed -i 's/\r$//' gestor.shEn el Hangman, adivinas toda la palabra pero igual "pierdes"palabras.txt tiene saltos de línea estilo Windowssed -i 's/\r$//' palabras.txtmake: command not foundNo instalaste build-essentialsudo apt install build-essential -yLos archivos aparecen como propiedad de "root" en ls -laPasa si alguien copió archivos arrastrando desde el Explorador de Windows en vez de usar git clone/git pullUsa sudo chown tu_usuario:tu_usuario archivo para recuperar el archivo, y de ahí en adelante usa siempre git pull para actualizargit clone o git pull piden usuario y contraseña y la rechazanGitHub ya no acepta la contraseña normal, se necesita un "Personal Access Token"Pide ayuda al equipo para generar el token desde la configuración de GitHub


7. Resumen rápido (para copiar y pegar)

Si ya tienes todo instalado y copiado, esto es lo único que necesitas correr cada vez:

bash# Parte 1
cd ~/proyecto_final_unix/Parte1_ShellScript
chmod +x gestor.sh
./gestor.sh

# Parte 2
cd ~/proyecto_final_unix/Parte2_C_Hangman
make
./hangman
```
