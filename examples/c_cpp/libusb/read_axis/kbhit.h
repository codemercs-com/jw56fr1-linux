//Workaround für Tastenabfrage in einer schleife zum beenden

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int kbhit(void);