# Proyecto 3 - FileSystem
## Sistemas Operativos 2020

### Terminal Operators
## Folder
    crear : mkdir dirname
    eliminar : rmdir dirname
    desplazarse : mv path/source_dirname path/dest_dirname
    renombrar : mv source_dirname dest_dirname

## Files
    crear : touch filename.txt
    eliminar : unlink filename | rm filename
    abrir : cat filename.txt
    cerrar : close
    leer : less filename.txt
    escribir : cat >> filename.txt
    renombrar : mv source_file dest_file
    obtener atributos : get filename.txt
    establecer atributos : set filename.txt -o owner -tm timeModification -dm dateModification -tc timeCreation -dc dateCreation -s currentSize

## Other - Display
    listado default: ls | ls -a
    listado por tiempo de modificacion : ls -t

### FileSystem Functions
## Iniciar FileSystem
    startFileSystem

## Folder
    crear: mkdir
    eliminar: rmdir
    desplazarse: search
    renombrar: renameElement

## Files
    crear: touch
    eliminar: rm
    abrir: usar la misma de leer
    cerrar: -no aplica-
    leer: readFile
    escribir: writeFile
    renombrar: renameElement
    obtener atributos: getFileData
    establecer atributos: 
	size  -> changeSize
	name  -> changeName
	owner -> changeOwner
