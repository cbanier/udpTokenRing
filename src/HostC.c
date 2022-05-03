#include "primitives.h"

#include "trame.h"

int main(int argc, char **argv) {
    /* allocation de l'hostC */
    Host *C = creerHost(PORT_C, PORT_D);

    /* allocation de la trame */
    Trame *trame = creerTrame();

    printf("HostC écoute ...\n\n");

    boucleHost(C, trame);

    cleanTrame(trame);

    cleanHost(C);

    return 0;
}
