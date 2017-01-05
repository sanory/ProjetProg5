#!/bin/sh

#TEST AFFICHAGE HEADER
readelf -h $1 > fichier1

./projet -h  $1 > fichier2

echo
diff -uwbB fichier1 fichier2


rm fichier1 fichier2






#TEST AFFICHAGE SECTION HEADER
readelf -S $1 > fichier1

./projet -S  $1 > fichier2

echo
diff -uwbB fichier1 fichier2


rm fichier1 fichier2






#TEST AFFICHAGE TABLE DES SYMBOLES
readelf -s $1 > fichier1

./projet -s  $1 > fichier2

echo
diff -uwbB fichier1 fichier2


rm fichier1 fichier2





#Ne pas effacer
#~/Téléchargements/horloge_irq.o