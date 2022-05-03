#include "primitives.h"

#include "menu.h"
#include "trame.h"

int main(int argc, char **argv) {
    /* allocation de l'hostA */
    Host *A = creerHost(PORT_A, PORT_B);

    /* allocation de la trame */
    Trame *trame = creerTrame();

    /* buffer */
    char buffer[TAILLE_MAX_BUFFER];

    /* création de la trame */
    /* HostA envoie un message à HostC */
    newTrame(trame, PORT_A, PORT_C, 40, "Hello_World!", A->priseEmission, buffer);

    printf("HostA écoute ...\n\n");

    boucleHost(A, trame);

    cleanTrame(trame);

    cleanHost(A);

    return 0;
}