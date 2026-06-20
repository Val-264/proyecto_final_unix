# Proyecto Final

Gestor de películas/series en Shell Script + Juego del Ahorcado en C.

## Estructura del proyecto

- **Parte1_ShellScript/** — Script en Bash para gestionar un catálogo de películas y series (Alta, Baja, Consulta, Modificación, Reportes)
- **Parte2_C_Hangman/** — Juego del ahorcado en C, compilado con Makefile

## Cómo correrlo

### 1. Requisitos (Windows con WSL)

Si no tienes WSL instalado, en PowerShell como administrador:

```bash
wsl --install
```

Dentro de tu terminal Ubuntu, instala las herramientas necesarias:

```bash
sudo apt update
sudo apt install build-essential git -y
```

### 2. Clonar el repositorio

```bash
cd ~
git clone https://github.com/Val-264/proyecto_final_unix.git
cd proyecto_final_unix
```

### 3. Ejecutar la Parte 1 (Shell Script)

```bash
cd Parte1_ShellScript
sed -i 's/\r$//' gestor.sh
chmod +x gestor.sh
./gestor.sh
```

### 4. Ejecutar la Parte 2 (Hangman en C)

```bash
cd Parte2_C_Hangman
sed -i 's/\r$//' palabras.txt
make
./hangman
```

## Errores comunes

| Error | Solución |
|---|---|
| `Permission denied` al ejecutar `./gestor.sh` | `chmod +x gestor.sh` |
| El script no responde bien / comportamiento raro | `sed -i 's/\r$//' gestor.sh` |
| En el Hangman, adivinas todo pero "pierdes" | `sed -i 's/\r$//' palabras.txt` |
| `make: command not found` | `sudo apt install build-essential -y` |
