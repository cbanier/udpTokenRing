#include "primitives.h"

#include "trame.h"

int main(int argc, char **argv) {
    /* allocation de l'hostD */
    Host *D = creerHost(PORT_D, PORT_A);

    /* allocation de la trame */
    Trame *trame = creerTrame();

    printf("HostD écoute ...\n\n");

    boucleHost(D, trame);

    cleanTrame(trame);

    cleanHost(D);

    return 0;
}
