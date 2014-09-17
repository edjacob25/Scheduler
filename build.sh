#Compilar, la opcion -Wall es para todos los warnings 
gcc -o test FileIO.c dispatcher.c `pkg-config --cflags glib-2.0` scheduler.c `pkg-config --libs glib-2.0`
