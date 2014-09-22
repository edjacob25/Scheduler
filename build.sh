#eliminar viejo ejecutable
rm debug
rm release
#Compilar, la opcion -Wall es para todos los warnings 
gcc -o debug FileIO.c dispatcher.c `pkg-config --cflags glib-2.0` scheduler.c `pkg-config --libs glib-2.0` -DDEBUG
gcc -o release FileIO.c dispatcher.c `pkg-config --cflags glib-2.0` scheduler.c `pkg-config --libs glib-2.0`