#!/bin/bash

USER_IPCS=$(echo $USER | cut -c1-10)

#MEMORIAS COMPARTIDAS
echo "Memorias eliminadas:"
IPCSMEM=$(ipcs -m |tr -s " ") 
SEP=" "
IFS="
"

for i in $IPCSMEM; do
	IPCSMEM_KEY=$(echo "$i" | cut -d "${SEP}" -f 2)
	IPCSMEM_USER=$(echo "$i" | cut -d "${SEP}" -f 3) #usuario
	IPCSMEM_PRIV=$(echo "$i" | cut -d "${SEP}" -f 4) #privilegios
	IPCSMEM_PRO=$(echo "$i" | cut -d "${SEP}" -f 6) #procesos atachados

	if [ $IPCSMEM_USER = $USER -o $IPCSMEM_USER = $USER_IPCS ];then
	if [ $IPCSMEM_PRIV = "666" ];then
	if [ $IPCSMEM_PRO = "0" ]; then
		echo $IPCSMEM_KEY $IPCSMEM_USER $IPCSMEM_PRIV $IPCSMEM_PRO     		
		ipcrm -m $IPCSMEM_KEY
		echo ""
	fi
	fi	
	fi
done


#SEMAFOROS
echo "Semaforos eliminados:"
IPCSSEM=$(ipcs -s |tr -s " ") 

IFS="
"

for i in $IPCSSEM; do
	IPCSSEM_KEY=$(echo "$i" | cut -d "${SEP}" -f 2)
	IPCSSEM_USER=$(echo "$i" | cut -d "${SEP}" -f 3) #usuario
	IPCSSEM_PRIV=$(echo "$i" | cut -d "${SEP}" -f 4) #privilegios
	
	if [ $IPCSSEM_USER = $USER -o $IPCSSEM_USER = $USER_IPCS ];then
	if [ $IPCSSEM_PRIV = "666" ];then
		echo $IPCSSEM_KEY $IPCSSEM_USER $IPCSSEM_PRIV $IPCSSEM_PRO     		
		ipcrm -s $IPCSSEM_KEY
		echo ""
	fi	
	fi
done


#COLAS
echo "Colas eliminados:"
IPCSQUE=$(ipcs -q |tr -s " ") 

IFS="
"

for i in $IPCSQUE; do
	IPCSQUE_KEY=$(echo "$i" | cut -d "${SEP}" -f 2)
	IPCSQUE_USER=$(echo "$i" | cut -d "${SEP}" -f 3) #usuario
	IPCSQUE_PRIV=$(echo "$i" | cut -d "${SEP}" -f 4) #privilegios
	
	if [ $IPCSQUE_USER = $USER -o $IPCSQUE_USER = $USER_IPCS ];then
	if [ $IPCSQUE_PRIV = "666" ];then
		echo $IPCSQUE_KEY $IPCSQUE_USER $IPCSQUE_PRIV $IPCSQUE_PRO     		
		ipcrm -q $IPCSQUE_KEY
		echo ""
	fi	
	fi
done
