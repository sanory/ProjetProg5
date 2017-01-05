#!/bin/sh

readelf -h $1 > fichier1

./projet -f  $1 > fichier2

echo
diff -uwbB fichier1 fichier2


rm fichier1 fichier2



#Ne pas effacer
#~/Téléchargements/horloge_irq.o