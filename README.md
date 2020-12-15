# Proyecto 3 - FileSystem
## Sistemas Operativos 2020

### Instalation
    sudo apt-get install libjson-c-dev
    sudo add-apt-repository ppa:allegro/5.2
    sudo apt-get install liballegro*5.2 liballegro*5-dev
    sudo apt install libcanberra-gtk-module libcanberra-gtk3-module

    sudo apt-get install python-tk

### Intrucctions (Interface)
    make
    /bin/.FileSystem

### Terminal Operators
## Folder
    crear : mkdir dirname
    eliminar : rmdir dirname
    desplazarse : mv old_dirname new_dirname
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
	size  -> modifySize
	name  -> renameElement
	owner -> modifyOwner
