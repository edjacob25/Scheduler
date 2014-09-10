#Compilar, la opcion -Wall es para todos los warnings 
gcc -Wall -o test FileIO.c `pkg-config --cflags glib-2.0` scheduler.c `pkg-config --libs glib-2.0`
