#include "primitives.h"

#include "trame.h"

int main(int argc, char **argv) {
    /* allocation de l'hostB */
    Host *B = creerHost(PORT_B, PORT_C);

    /* allocation de la trame */
    Trame *trame = creerTrame();

    printf("HostB écoute ...\n\n");

    boucleHost(B, trame);

    cleanTrame(trame);

    cleanHost(B);

    return 0;
}
