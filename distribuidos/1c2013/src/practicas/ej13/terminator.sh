#!/bin/bash

# Primero limpio todos los IPCs
if [ $# -ne 1 ] ; then
	echo "Error: Debe ingresar el nombre del proceso a asesinar"
	exit 1
fi

echo "Limpiando mecanismos de IPC"
#ipcrm -M 0x04062014
#ipcrm -S 0x01062014
#ipcrm -S 0x02062014
#ipcrm -S 0x03062014

# Luego elimino a los procesos "Puerta"

list=`ps -e | grep "^[^ ].* [^ ].* .*[^ ] $1" | cut -d " " -f 2`
for line in $list ; do
	kill -9 $line
	echo "Proceso $line ha sido asesinado"
done

	

