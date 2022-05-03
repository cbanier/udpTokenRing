/**
 * @file trame.h
 * @author Corentin Banier
 * @brief Gestion de la trame
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "primitives.h"

/* custom */
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define CYN "\x1B[36m"
#define RESET "\x1B[0m"

#define ADRESSE_IP "127.0.0.1"
#define TAILLE_ADRESSE 15
#define TAILLE_MAX_MESSAGE 256
#define TAILLE_MAX_BUFFER 2 * sizeof(int) + 2 * TAILLE_ADRESSE + TAILLE_MAX_MESSAGE

typedef enum { PORT_A = 1920, PORT_B = 1930, PORT_C = 1940, PORT_D = 1950 } port;

typedef struct {
    char adresse[TAILLE_ADRESSE - 4];
    port port_actuel;
    int priseEmission;
    int priseReception;
} Host;

/**
 * @brief Enumération des status du flag
 *
 */
typedef enum {
    ENVOIE = 0,     // envoie / transmission
    RECEPTION = 1,  // acquitement du message
    ERREUR = 2      // acquitement d'une erreur
} flagACK;

/**
 * @brief Structure qui stock la trame
 *
 */
typedef struct {
    int token;
    char adresse_emetteur[TAILLE_ADRESSE]; /* format : IP:PORT -> 127.0.0.1:1920 */
    char adresse_recepteur[TAILLE_ADRESSE];
    flagACK flag;
    int taille_message;
    char message[TAILLE_MAX_MESSAGE];
    int ci; /* contrôle d'intégrité */
} Trame;

/**
 * @brief Alloue la structure Host
 *
 * @param port_actuel
 * @param port_suivant
 * @return Host*
 */
Host *creerHost(port port_actuel, port port_suivant);

/**
 * @brief Désaloue l'espace memoire alloué de l'Host
 *
 * @param h
 */
void cleanHost(Host *h);

/**
 * @brief Retourne le port destinataire du nouveau message
 *
 * @param port_actuel
 * @return port
 */
port getPortRecepteur(port port_actuel);

/**
 * @brief Boucle principale de l'Host
 *
 * @param h
 * @param t
 */
void boucleHost(Host *h, Trame *t);

/**
 * @brief Alloue l'espace nécessaire au stockage de la structure Trame
 *
 * @return Trame*
 */
Trame *creerTrame(void);

/**
 * @brief Désaloue l'espace memoire alloué pour stocker le message à transmettre
 *
 * @param t
 */
void cleanTrame(Trame *t);

/**
 * @brief Initialise la trame lorsqu'on veut envover un message du port emetteur vers le
 * port recepteur ; Couche 1 - 2 - 3
 *
 * @param t
 * @param emetteur
 * @param recepteur
 * @param taille_message
 * @param message
 */
void initTrame(Trame *t, port emetteur, port recepteur, int taille_message,
               char *message);

/**
 * @brief Fonction qui permet à l'utilisateur de visualiser l'envoie d'une nouvelle trame
 * Couche 1 - 2 - 3
 *
 * @param t
 * @param emetteur
 * @param recepteur
 * @param taille_message
 * @param message
 * @param priseEmission
 * @param buffer
 */
void newTrame(Trame *t, port emetteur, port recepteur, int taille_message, char *message,
              int priseEmission, char *buffer);

/**
 * @brief Charge les informations contenus dans le buffer au sein la structure Trame
 * Couche 1 - 2 - 3
 *
 * @param t
 * @param buffer
 */
void loadTrame(Trame *t, char *buffer);

/**
 * @brief Retourne le port correspondant à l'adresse passée en paramètre ; Couche 2
 *
 * @param adresse
 * @return port
 */
port getPort(char *adresse);

/**
 * @brief Fonction qui traite la trame reçue ; Couche 1 - 2 - 3
 *
 * @param t
 * @param recepteur_courant
 * @param priseEmission
 * @param buffer
 */
void traiteTrame(Trame *t, port recepteur_courant, int priseEmission, char *buffer);

/**
 * @brief Fonction de hachage modulo 29 ; Couche 3
 *
 * @param message
 * @return int
 */
int getHash(char *message);