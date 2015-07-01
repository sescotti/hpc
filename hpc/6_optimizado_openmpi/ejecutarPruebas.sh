#/bin/bash

echo " "
echo "Version;matmul_openMPI"
echo "Tipo;Paralelo OpenMPI"
echo " "
echo "n;descartar;descartar;descartar;ejecucion 1;ejecucion 2;ejecucion 3;"

for valor in ${valores}
do
	num=0
	echo -e ${valor}";\c"
	while [ $num -le 5 ]
	do
		./pruebampi matmul_openMPI ${valor}
		let num=${num}+1
	done
	echo " "
done

exit 0
