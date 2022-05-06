#include "trame.h"

/* ************************************************************************** */
/*                                    HOST                                    */
/* ************************************************************************** */

Host *creerHost(port port_actuel, port port_suivant) {
    Host *host = (Host *)malloc(sizeof(Host));
    assert(host);
    strcpy(host->adresse, ADRESSE_IP);
    host->port_actuel = port_actuel;
    host->priseEmission = creePriseEmission(host->adresse, port_suivant);
    host->priseReception = creePriseReception(port_actuel);
    return host;
}

/* ************************************************************************** */

void cleanHost(Host *h) { free(h); }

/* ************************************************************************** */

port getPortRecepteur(port port_actuel) {
    char port_tmp;

    switch (port_actuel) {
        case PORT_A:
            printf("A qui voulez-vous ennvoyer le message ? tapez " BGRN "B" RESET
                   ", " BGRN "C" RESET " ou " BGRN "D" RESET " \n");
            scanf(" %c", &port_tmp);
            if (port_tmp == 'A') {
                printf(
                     "\n ~ Vous ne pouvez pas vous envoyer un message à vous "
                     "même.\nVeuillez "
                     "recommencer : \n");
                return getPortRecepteur(port_actuel);
            } else if (port_tmp == 'B')
                return PORT_B;
            else if (port_tmp == 'C')
                return PORT_C;
            else if (port_tmp == 'D')
                return PORT_D;
            else
                break;

        case PORT_B:
            printf("A qui voulez-vous ennvoyer le message ? tapez " BGRN "A" RESET
                   ", " BGRN "C" RESET " ou " BGRN "D" RESET " \n");
            scanf(" %c", &port_tmp);
            if (port_tmp == 'B') {
                printf(
                     "\n ~ Vous ne pouvez pas vous envoyer un message à vous "
                     "même.\nVeuillez "
                     "recommencer : \n");
                return getPortRecepteur(port_actuel);
            } else if (port_tmp == 'A')
                return PORT_A;
            else if (port_tmp == 'C')
                return PORT_C;
            else if (port_tmp == 'D')
                return PORT_D;
            else
                break;

        case PORT_C:
            printf("A qui voulez-vous ennvoyer le message ? tapez " BGRN "A" RESET
                   ", " BGRN "B" RESET " ou " BGRN "D" RESET " \n");
            scanf(" %c", &port_tmp);
            if (port_tmp == 'C') {
                printf(
                     "\n ~ Vous ne pouvez pas vous envoyer un message à vous "
                     "même.\nVeuillez "
                     "recommencer : \n");
                return getPortRecepteur(port_actuel);
            } else if (port_tmp == 'A')
                return PORT_A;
            else if (port_tmp == 'B')
                return PORT_B;
            else if (port_tmp == 'D')
                return PORT_D;
            else
                break;

        case PORT_D:
            printf("A qui voulez-vous ennvoyer le message ? tapez " BGRN "A" RESET
                   ", " BGRN "B" RESET " ou " BGRN "C" RESET " \n");
            scanf(" %c", &port_tmp);
            if (port_tmp == 'D') {
                printf(
                     "\n ~ Vous ne pouvez pas vous envoyer un message à vous "
                     "même.\nVeuillez "
                     "recommencer : \n");
                return getPortRecepteur(port_actuel);
            } else if (port_tmp == 'A')
                return PORT_A;
            else if (port_tmp == 'B')
                return PORT_B;
            else if (port_tmp == 'C')
                return PORT_C;
            else
                break;
    }
    printf(
         "\n ~ Ce port n'existe pas. Veuillez "
         "recommencer : \n");
    return getPortRecepteur(port_actuel);
}

/* ************************************************************************** */

void boucleHost(Host *h, Trame *t) {
    /* buffer */
    char buffer[TAILLE_MAX_BUFFER];

    do {
        if (t->token == 0) {
            memset(buffer, '\0', sizeof(buffer));
            recoit(h->priseReception, buffer, sizeof(buffer) - 1);

            /* boucle pour faire circuler le jeton tant qu'aucun hôte ne veut diffuser un
             * message */
            char adresse_source[TAILLE_ADRESSE], adresse_destinataire[TAILLE_ADRESSE];
            char message[TAILLE_MAX_MESSAGE];
            int tmp_flag, tmp_ci;
            sscanf(buffer, "%1d%14s%14s%d%255s", &tmp_flag, adresse_source,
                   adresse_destinataire, &tmp_ci, message);
            if (strcmp(adresse_source, "127.0.0.1:3000") == 0 &&
                strcmp(adresse_destinataire, "127.0.0.1:3000") == 0 &&
                strcmp(message, "token_disponible") == 0) {
                /* on va passer dans le prochain bloc else if afin de demander à l'hôte
                 * s'il veut envoyer un message */
                t->token = 1;
            } else {
                loadTrame(t, buffer);
                traiteTrame(t, h->port_actuel, h->priseEmission, buffer);
            }
        } else if (t->token == 1) {
            printf("Envoyer un autre message ? \n");
            printf("Touche " BGRN "y" RESET " pour oui, n'importe quoi pour non ... \n");
            char cmd;
            scanf(" %c", &cmd);

            if (cmd == 'y') {
                int port_recepteur = getPortRecepteur(h->port_actuel);

                int taille_message;
                printf("Saisir la taille du message : \n");
                scanf("%d", &taille_message);

                /* clean stdin */
                fseek(stdin, 0, SEEK_END);

                char new_message[taille_message + 1];
                printf("Saisir le message à envoyer : \n");
                fgets(new_message, taille_message, stdin);

                /* enlève le dernier caractère de new_message qui est '\n' */
                int index = 0;
                while (new_message[index] != '\n') index += 1;

                new_message[index] = '\0';

                newTrame(t, h->port_actuel, port_recepteur, taille_message, new_message,
                         h->priseEmission, buffer);
            } else {
                /* envoie de cette trame particulière qui permet de donner l'information
                 * au prochain hôte qu'il va pouvoir envoyer un message */
                memset(buffer, '\0', TAILLE_MAX_BUFFER);
                sprintf(buffer, "%1d%14s%14s%d%255s", t->flag, "127.0.0.1:3000",
                        "127.0.0.1:3000", 0, "token_disponible");
                envoie(h->priseEmission, buffer, strlen(buffer));
                t->token = 0;
            }
        }
    } while (1);
}

/* ************************************************************************** */
/*                                    TRAME                                   */
/* ************************************************************************** */

Trame *creerTrame(void) {
    Trame *trame = (Trame *)malloc(sizeof(Trame));
    assert(trame);
    trame->token = 0;
    return trame;
}

/* ************************************************************************** */

void cleanTrame(Trame *t) { free(t); }

/* ************************************************************************** */

void initTrame(Trame *t, port emetteur, port recepteur, int taille_message,
               char *message) {
    t->token = 1;
    t->flag = ENVOIE;
    /* création des adresses ip */
    char tmp_port_emetteur[5], tmp_port_recepteur[5];
    char tmp_emetteur[15], tmp_recepteur[15];
    strcpy(tmp_emetteur, ADRESSE_IP);
    strcpy(tmp_recepteur, ADRESSE_IP);

    strcat(tmp_emetteur, ":");
    strcat(tmp_recepteur, ":");

    sprintf(tmp_port_emetteur, "%d", emetteur);
    sprintf(tmp_port_recepteur, "%d", recepteur);

    strcat(tmp_emetteur, tmp_port_emetteur);
    strcat(tmp_recepteur, tmp_port_recepteur);

    strcpy(t->adresse_emetteur, tmp_emetteur);
    strcpy(t->adresse_recepteur, tmp_recepteur);

    t->taille_message = taille_message;
    strcpy(t->message, message);

    t->ci = getHash(t->message);
}

/* ************************************************************************** */

void newTrame(Trame *t, port emetteur, port recepteur, int taille_message, char *message,
              int priseEmission, char *buffer) {
    initTrame(t, emetteur, recepteur, taille_message, message);
    printf("Touche " BGRN "s" RESET " pour envoyer ...\n");
    while (getchar() != 's') {
        ;
    }
    memset(buffer, '\0', TAILLE_MAX_BUFFER);
    sprintf(buffer, "%1d%14s%14s%d%255s", t->flag, t->adresse_emetteur,
            t->adresse_recepteur, t->ci, t->message);
    envoie(priseEmission, buffer, strlen(buffer));
    t->token = 0; /* on passe le token à 0 car une fois que l'emetteur a envoyé le
         message, il attend l'accusé de réception */
}

/* ************************************************************************** */

port getPort(char *adresse) {
    if (strcmp(adresse, "127.0.0.1:1920") == 0)
        return PORT_A;
    else if (strcmp(adresse, "127.0.0.1:1930") == 0)
        return PORT_B;
    else if (strcmp(adresse, "127.0.0.1:1940") == 0)
        return PORT_C;
    else
        return PORT_D;
}

/* ************************************************************************** */

void loadTrame(Trame *t, char *buffer) {
    sscanf(buffer, "%1d%14s%14s%d%255s", &t->flag, t->adresse_emetteur,
           t->adresse_recepteur, &t->ci, t->message);
}

/* ************************************************************************** */

void traiteTrame(Trame *t, port recepteur_courant, int priseEmission, char *buffer) {
    /* Gestion de la trame pour le cas où une trame circule sur le réseau */
    /* on revient sur l'hôte qui a émit la trame */
    if (t->flag == RECEPTION) {
        if (getPort(t->adresse_emetteur) == recepteur_courant) {
            printf(BRED "Accusé de reception : \n" RESET);
            printf("Le destinataire a bien reçu le message \n\n");
            t->token = 1;
            t->flag = ENVOIE;
            t->taille_message = 0;
            strcpy(t->message, "");
        } else {
            printf("Je ne suis pas le destinataire du message \n\n");
            envoie(priseEmission, buffer, strlen(buffer));
        }
    } else if (t->flag == ENVOIE) {
        if (getPort(t->adresse_recepteur) == recepteur_courant) {
            t->flag = RECEPTION;

            /* vérification du hash (ci) */
            int hash = getHash(t->message);
            if (hash != t->ci) {
                t->flag = ERREUR;
            } else {
                printf(BRED "Je suis le destinataire. \n" RESET);
                printf("Le message qui m'est destiné : " CYN "%s \n\n" RESET, t->message);

                printf("Renvoie de l'accusé de réception \n");
                printf("Touche " BGRN "c" RESET " pour continuer ...\n\n");

                while (getchar() != 'c')
                    ;

                memset(buffer, '\0', TAILLE_MAX_BUFFER);
                sprintf(buffer, "%1d%14s%14s%d%255s", RECEPTION, t->adresse_emetteur,
                        t->adresse_recepteur, t->ci, t->message);
                envoie(priseEmission, buffer, strlen(buffer));
            }
        } else {
            printf("Je ne suis pas le destinataire du message \n\n");
            envoie(priseEmission, buffer, strlen(buffer));
        }
    } else if (t->flag == ERREUR) {
        printf("Erreur lors de la transmission du message \n\n");
    }
}

/* ************************************************************************** */
/*                               HACHAGE mod 29                               */
/* ************************************************************************** */

int getHash(char *message) {
    int hash = 0, index = 0;
    int p = 1, prime = 31;
    while (message[index] != '\0' && index < strlen(message)) {
        hash += (message[index] - '0') * p;
        p = (p * prime) % 29;
        index += 1;
    }
    return hash % 29;
}