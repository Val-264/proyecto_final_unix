#!/bin/bash

# Gestor de peliculas y series
# Proyecto final - Parte 1 (Shell Script)
# El archivo de datos es un CSV separado por pipes con esta estructura:
# ID|Titulo|Tipo|Genero|Anio|Temporadas|Estado|Calificacion

ARCHIVO="datos.csv"
SEPARADOR="|"

# si el archivo no existe lo crea vacio
inicializar_archivo() {
    if [ ! -f "$ARCHIVO" ]; then
        touch "$ARCHIVO"
        echo "Archivo de datos '$ARCHIVO' creado correctamente"
    fi
}

# saca el id mas alto y le suma 1, asi nunca se repiten los ids
obtener_siguiente_id() {
    if [ ! -s "$ARCHIVO" ]; then
        echo 1
    else
        ultimo_id=$(awk -F"$SEPARADOR" '{print $1}' "$ARCHIVO" | sort -n | tail -1)
        echo $((ultimo_id + 1))
    fi
}

# checa que el valor sean puros numeros (para anio, temporadas, ids)
es_numero() {
    local valor="$1"
    if [[ "$valor" =~ ^[0-9]+$ ]]; then
        return 0
    else
        return 1
    fi
}

# checa que sea un numero decimal, para la calificacion
es_decimal() {
    local valor="$1"
    if [[ "$valor" =~ ^[0-9]+([.][0-9]+)?$ ]]; then
        return 0
    else
        return 1
    fi
}

existe_id() {
    local id_buscar="$1"
    awk -F"$SEPARADOR" -v id="$id_buscar" '$1 == id {found=1} END {exit !found}' "$ARCHIVO"
}

# nada mas para que no se cierre de un de la pantalla y se pueda leer el resultado
pausar() {
    echo ""
    read -p "Presiona ENTER para continuar..." dummy
}

alta_registro() {
    clear
    echo "------- ALTA DE NUEVO REGISTRO -------"
    echo ""

    nuevo_id=$(obtener_siguiente_id)
    echo "ID asignado automaticamente: $nuevo_id"

    # pide el titulo y no deja que este vacio ni que se repita uno ya existente
    while true; do
        read -p "Titulo: " titulo
        if [ -z "$titulo" ]; then
            echo "Error: el titulo no puede estar vacio"
            continue
        fi
        if awk -F"$SEPARADOR" -v t="$titulo" 'BEGIN{IGNORECASE=1} $2 == t {found=1} END{exit !found}' "$ARCHIVO"; then
            echo "Error: ya existe un registro con ese titulo"
            continue
        fi
        break
    done

    while true; do
        read -p "Tipo (Pelicula/Serie): " tipo
        if [ "$tipo" != "Pelicula" ] && [ "$tipo" != "Serie" ]; then
            echo "Error: el tipo debe ser exactamente 'Pelicula' o 'Serie'"
            continue
        fi
        break
    done

    while true; do
        read -p "Genero: " genero
        if [ -z "$genero" ]; then
            echo "Error: el genero no puede estar vacio"
            continue
        fi
        break
    done

    while true; do
        read -p "Anio de estreno (4 digitos): " anio
        if ! es_numero "$anio" || [ ${#anio} -ne 4 ]; then
            echo "Error: el anio debe ser un numero de 4 digitos"
            continue
        fi
        break
    done

    # si es serie pide temporadas, si es pelicula no aplica
    if [ "$tipo" == "Serie" ]; then
        while true; do
            read -p "Numero de temporadas: " temporadas
            if ! es_numero "$temporadas"; then
                echo "Error: las temporadas deben ser un numero entero"
                continue
            fi
            break
        done
    else
        temporadas="N/A"
    fi

    while true; do
        read -p "Estado (Vista/Pendiente/EnCurso): " estado
        if [ "$estado" != "Vista" ] && [ "$estado" != "Pendiente" ] && [ "$estado" != "EnCurso" ]; then
            echo "Error: el estado debe ser 'Vista', 'Pendiente' o 'EnCurso'"
            continue
        fi
        break
    done

    while true; do
        read -p "Calificacion (0.0 - 10.0): " calificacion
        if ! es_decimal "$calificacion"; then
            echo "Error: la calificacion debe ser un numero"
            continue
        fi
        # con awk porque bash no compara bien los decimales
        dentro_rango=$(awk -v c="$calificacion" 'BEGIN{print (c>=0 && c<=10) ? "si" : "no"}')
        if [ "$dentro_rango" != "si" ]; then
            echo "Error: la calificacion debe estar entre 0.0 y 10.0"
            continue
        fi
        break
    done

    # antes de guardar muestra un resumen y pide confirmacion, por eso es "alta segura"
    echo ""
    echo "Resumen del nuevo registro:"
    echo "ID: $nuevo_id | Titulo: $titulo | Tipo: $tipo | Genero: $genero"
    echo "Anio: $anio | Temporadas: $temporadas | Estado: $estado | Calificacion: $calificacion"
    read -p "¿Confirmar alta? (s/n): " confirmar

    if [ "$confirmar" == "s" ] || [ "$confirmar" == "S" ]; then
        echo "$nuevo_id$SEPARADOR$titulo$SEPARADOR$tipo$SEPARADOR$genero$SEPARADOR$anio$SEPARADOR$temporadas$SEPARADOR$estado$SEPARADOR$calificacion" >> "$ARCHIVO"
        echo "Registro agregado correctamente"
    else
        echo "Alta cancelada"
    fi

    pausar
}

baja_registro() {
    clear
    echo "------- BAJA DE REGISTRO -------"
    echo ""

    if [ ! -s "$ARCHIVO" ]; then
        echo "No hay registros para eliminar"
        pausar
        return
    fi

    read -p "Ingresa el ID del registro a eliminar: " id_eliminar

    if ! es_numero "$id_eliminar"; then
        echo "Error: el ID debe ser numerico"
        pausar
        return
    fi

    registro=$(awk -F"$SEPARADOR" -v id="$id_eliminar" '$1 == id' "$ARCHIVO")

    if [ -z "$registro" ]; then
        echo "No se encontro ningun registro con ID $id_eliminar"
        pausar
        return
    fi

    echo "Registro encontrado:"
    echo "$registro" | awk -F"$SEPARADOR" '{print "ID: "$1" | Titulo: "$2" | Tipo: "$3" | Genero: "$4}'
    read -p "¿Confirmas que deseas eliminar este registro? (s/n): " confirmar

    if [ "$confirmar" == "s" ] || [ "$confirmar" == "S" ]; then
        # guarda todo menos la linea del id que se va a borrar, y reemplaza el archivo original
        awk -F"$SEPARADOR" -v id="$id_eliminar" '$1 != id' "$ARCHIVO" > "${ARCHIVO}.tmp"
        mv "${ARCHIVO}.tmp" "$ARCHIVO"
        echo "Registro eliminado correctamente"
    else
        echo "Baja cancelada"
    fi

    pausar
}

consulta_registros() {
    clear
    echo "------- CONSULTA DE REGISTROS -------"
    echo ""
    echo "1. Ver todos los registros"
    echo "2. Buscar por ID"
    echo "3. Buscar por titulo (busqueda parcial)"
    echo "4. Buscar por genero"
    echo "5. Regresar al menu principal"
    echo ""
    read -p "Elige una opcion: " opcion_consulta

    if [ ! -s "$ARCHIVO" ]; then
        echo "No hay registros en el sistema"
        pausar
        return
    fi

    case $opcion_consulta in
        1)
            mostrar_tabla "$(cat "$ARCHIVO")"
            ;;
        2)
            read -p "ID a buscar: " id_buscar
            resultado=$(awk -F"$SEPARADOR" -v id="$id_buscar" '$1 == id' "$ARCHIVO")
            if [ -z "$resultado" ]; then
                echo "No se encontro ningun registro con ese ID"
            else
                mostrar_tabla "$resultado"
            fi
            ;;
        3)
            read -p "Texto a buscar en el titulo: " texto_buscar
            resultado=$(awk -F"$SEPARADOR" -v t="$texto_buscar" 'BEGIN{IGNORECASE=1} $2 ~ t' "$ARCHIVO")
            if [ -z "$resultado" ]; then
                echo "No se encontraron coincidencias"
            else
                mostrar_tabla "$resultado"
            fi
            ;;
        4)
            read -p "Genero a buscar: " genero_buscar
            resultado=$(awk -F"$SEPARADOR" -v g="$genero_buscar" 'BEGIN{IGNORECASE=1} $4 ~ g' "$ARCHIVO")
            if [ -z "$resultado" ]; then
                echo "No se encontraron coincidencias"
            else
                mostrar_tabla "$resultado"
            fi
            ;;
        5)
            return
            ;;
        *)
            echo "Opcion invalida"
            ;;
    esac

    pausar
}

# imprime los registros en formato de tabla, mas facil de leer que el csv crudo
mostrar_tabla() {
    local datos="$1"
    echo ""
    printf "%-4s %-25s %-10s %-15s %-6s %-6s %-10s %-6s\n" "ID" "TITULO" "TIPO" "GENERO" "ANIO" "TEMP" "ESTADO" "CALIF"
    echo "--------------------------------------------------------------------------------------------------"
    echo "$datos" | awk -F"$SEPARADOR" '{printf "%-4s %-25s %-10s %-15s %-6s %-6s %-10s %-6s\n", $1, $2, $3, $4, $5, $6, $7, $8}'
}

modificar_registro() {
    clear
    echo "------- MODIFICACION DE REGISTRO -------"
    echo ""

    if [ ! -s "$ARCHIVO" ]; then
        echo "No hay registros para modificar"
        pausar
        return
    fi

    read -p "Ingresa el ID del registro a modificar: " id_modificar

    if ! es_numero "$id_modificar"; then
        echo "Error: el ID debe ser numerico"
        pausar
        return
    fi

    registro=$(awk -F"$SEPARADOR" -v id="$id_modificar" '$1 == id' "$ARCHIVO")

    if [ -z "$registro" ]; then
        echo "No se encontro ningun registro con ese ID"
        pausar
        return
    fi

    echo "Registro actual:"
    mostrar_tabla "$registro"

    echo ""
    echo "¿Que campo deseas modificar?"
    echo "1. Titulo"
    echo "2. Tipo"
    echo "3. Genero"
    echo "4. Anio"
    echo "5. Temporadas"
    echo "6. Estado"
    echo "7. Calificacion"
    read -p "Opcion: " campo

    # separa el registro encontrado en sus 8 campos para poder editar solo uno
    IFS="$SEPARADOR" read -r r_id r_titulo r_tipo r_genero r_anio r_temp r_estado r_calif <<< "$registro"

    case $campo in
        1)
            read -p "Nuevo titulo: " nuevo_valor
            if [ -z "$nuevo_valor" ]; then
                echo "Error: el titulo no puede estar vacio"
                pausar
                return
            fi
            r_titulo="$nuevo_valor"
            ;;
        2)
            read -p "Nuevo tipo (Pelicula/Serie): " nuevo_valor
            if [ "$nuevo_valor" != "Pelicula" ] && [ "$nuevo_valor" != "Serie" ]; then
                echo "Error: tipo invalido"
                pausar
                return
            fi
            r_tipo="$nuevo_valor"
            ;;
        3)
            read -p "Nuevo genero: " nuevo_valor
            if [ -z "$nuevo_valor" ]; then
                echo "Error: el genero no puede estar vacio"
                pausar
                return
            fi
            r_genero="$nuevo_valor"
            ;;
        4)
            read -p "Nuevo anio (4 digitos): " nuevo_valor
            if ! es_numero "$nuevo_valor" || [ ${#nuevo_valor} -ne 4 ]; then
                echo "Error: anio invalido"
                pausar
                return
            fi
            r_anio="$nuevo_valor"
            ;;
        5)
            read -p "Nuevas temporadas: " nuevo_valor
            if ! es_numero "$nuevo_valor"; then
                echo "Error: las temporadas deben ser un numero"
                pausar
                return
            fi
            r_temp="$nuevo_valor"
            ;;
        6)
            read -p "Nuevo estado (Vista/Pendiente/EnCurso): " nuevo_valor
            if [ "$nuevo_valor" != "Vista" ] && [ "$nuevo_valor" != "Pendiente" ] && [ "$nuevo_valor" != "EnCurso" ]; then
                echo "Error: estado invalido"
                pausar
                return
            fi
            r_estado="$nuevo_valor"
            ;;
        7)
            read -p "Nueva calificacion (0.0-10.0): " nuevo_valor
            if ! es_decimal "$nuevo_valor"; then
                echo "Error: calificacion invalida"
                pausar
                return
            fi
            dentro_rango=$(awk -v c="$nuevo_valor" 'BEGIN{print (c>=0 && c<=10) ? "si" : "no"}')
            if [ "$dentro_rango" != "si" ]; then
                echo "Error: la calificacion debe estar entre 0.0 y 10.0"
                pausar
                return
            fi
            r_calif="$nuevo_valor"
            ;;
        *)
            echo "Opcion invalida"
            pausar
            return
            ;;
    esac

    nueva_linea="$r_id$SEPARADOR$r_titulo$SEPARADOR$r_tipo$SEPARADOR$r_genero$SEPARADOR$r_anio$SEPARADOR$r_temp$SEPARADOR$r_estado$SEPARADOR$r_calif"

    # recorre el archivo y cuando encuentra el id que se esta editando, imprime la linea nueva en vez de la vieja
    awk -F"$SEPARADOR" -v id="$id_modificar" -v nueva="$nueva_linea" \
        'BEGIN{OFS=FS} $1 == id {print nueva; next} {print}' "$ARCHIVO" > "${ARCHIVO}.tmp"
    mv "${ARCHIVO}.tmp" "$ARCHIVO"

    echo "Registro modificado correctamente"
    pausar
}

menu_reportes() {
    clear
    echo "------- REPORTES -------"
    echo ""
    echo "1. Conteo total: Peliculas vs Series"
    echo "2. Promedio de calificacion por genero"
    echo "3. Listado de pendientes por ver (ordenado por calificacion)"
    echo "4. Top 5 mejor calificados"
    echo "5. Distribucion de titulos por anio"
    echo "6. Regresar al menu principal"
    echo ""
    read -p "Elige un reporte: " opcion_reporte

    if [ ! -s "$ARCHIVO" ]; then
        echo "No hay registros en el sistema"
        pausar
        return
    fi

    case $opcion_reporte in
        1) reporte_conteo_tipo ;;
        2) reporte_promedio_genero ;;
        3) reporte_pendientes ;;
        4) reporte_top5 ;;
        5) reporte_distribucion_anio ;;
        6) return ;;
        *) echo "Opcion invalida." ;;
    esac

    pausar
}

reporte_conteo_tipo() {
    echo ""
    echo "Conteo de Peliculas vs Series:"
    awk -F"$SEPARADOR" '
        $3=="Pelicula" {peliculas++}
        $3=="Serie" {series++}
        END {
            print "Peliculas: " peliculas+0
            print "Series:    " series+0
            print "Total:     " (peliculas+series)
        }' "$ARCHIVO"
}

reporte_promedio_genero() {
    echo ""
    echo "Promedio de calificacion por genero:"
    awk -F"$SEPARADOR" '
        {
            suma[$4] += $8
            cuenta[$4]++
        }
        END {
            for (g in suma) {
                printf "%-20s Promedio: %.2f  (basado en %d titulos)\n", g, suma[g]/cuenta[g], cuenta[g]
            }
        }' "$ARCHIVO" | sort
}

reporte_pendientes() {
    echo ""
    echo "Pendientes por ver, de mayor a menor calificacion:"
    pendientes=$(awk -F"$SEPARADOR" '$7=="Pendiente"' "$ARCHIVO" | sort -t"$SEPARADOR" -k8 -nr)
    if [ -z "$pendientes" ]; then
        echo "No hay titulos pendientes"
    else
        mostrar_tabla "$pendientes"
    fi
}

reporte_top5() {
    echo ""
    echo "Top 5 mejor calificados:"
    top5=$(sort -t"$SEPARADOR" -k8 -nr "$ARCHIVO" | head -5)
    mostrar_tabla "$top5"
}

reporte_distribucion_anio() {
    echo ""
    echo "Cantidad de titulos por anio:"
    awk -F"$SEPARADOR" '{conteo[$5]++} END {for (a in conteo) print a, conteo[a]}' "$ARCHIVO" | sort -n | \
        awk '{printf "Anio %-6s -> %d titulo(s)\n", $1, $2}'
}

# arranca el programa
inicializar_archivo

while true; do
    clear
    echo "------------------------------------"
    echo "   GESTOR DE PELICULAS Y SERIES"
    echo "------------------------------------"
    echo "1. Alta de registro"
    echo "2. Baja de registro"
    echo "3. Consulta de registros"
    echo "4. Modificacion de registro"
    echo "5. Reportes"
    echo "6. Salir"
    echo "------------------------------------"
    read -p "Elige una opcion: " opcion

    case $opcion in
        1) alta_registro ;;
        2) baja_registro ;;
        3) consulta_registros ;;
        4) modificar_registro ;;
        5) menu_reportes ;;
        6)
            echo "Saliendo del sistema Bye!"
            exit 0
            ;;
        *)
            echo "Opcion invalida vuelve a intentarlo"
            sleep 1
            ;;
    esac
done