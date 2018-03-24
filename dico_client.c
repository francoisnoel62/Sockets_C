#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /* pour socket */
#include <sys/socket.h>
#include <string.h> /* pour memset */
#include <netinet/in.h> /* pour struct sockaddr_in */
#include <arpa/inet.h> /* pour htons et inet_aton */
#define LG_MESSAGE 256

int main() {
	int sock;
	struct sockaddr_in adresse; /* adresse et numero de port du serveur */
	socklen_t longueurAdresse;
	char messageEnvoi[LG_MESSAGE]; /* le message envoye */
	char messageRecu[LG_MESSAGE]; /* le message recu */
	int nbo_ecrits, nbo_lus, retour;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("socket");
		exit(-1);
	}
	printf("Socket creee avec succes ! (%d)\n", sock);
	longueurAdresse = sizeof(adresse);
	memset(&adresse, 0x00, longueurAdresse);
	adresse.sin_family = AF_INET;
	adresse.sin_port = htons(IPPORT_USERRESERVED); // = 5000
	inet_aton("127.0.0.1", &adresse.sin_addr);
	// On se connecte au serveur
	if((connect(sock, (struct sockaddr *)&adresse, longueurAdresse)) == -1) {
		perror("connect");
		close(sock);
		exit(2);
	}
		if (getsockname(sock, (struct sockaddr *)&adresse, & longueurAdresse ) == -1) {
		perror ("getsockname");
		exit(3);
	}
	printf("IP client = %s, Port client = %d\n", inet_ntoa(adresse.sin_addr), ntohs(adresse.sin_port) );
	printf("Connexion au serveur reussie avec succes !\n");
	// On initialise Ã  0 les messages
	memset(messageEnvoi, 0x00, LG_MESSAGE*sizeof(char));
	memset(messageRecu, 0x00, LG_MESSAGE*sizeof(char));
	// Envoie un message au serveur
	//sprintf(messageEnvoi, "HELLO WORLD !\n");
	printf("Quel est votre message ?\n");
	scanf("%s", &messageEnvoi);
	nbo_ecrits = write(sock, messageEnvoi, strlen(messageEnvoi));
	switch(nbo_ecrits) {
		case -1 : /* en cas d'une erreur */
		perror("write");
		close(sock);
		exit(-3);
		case 0 : /* la socket est fermee */
		fprintf(stderr, "La socket a ete fermee par le serveur !\n\n");
		close(sock);
		return 0;
		default: /* envoi de n octets */
		printf("Message %s a ete envoye avec succes (%d octets)\n\n", messageEnvoi, nbo_ecrits);
	}
	/* On recoit des donnÃ©es du serveur */
	nbo_lus = read(sock, messageRecu, LG_MESSAGE*sizeof(char)); /* on attend un message de TAILLE fixe */
	switch(nbo_lus) {
		case -1 :
		perror("read");
		close(sock);
		exit(-4);
		case 0 : /* la socket est fermee */
		fprintf(stderr, "La socket a ete fermee par le serveur !\n\n");
		close(sock);
		return 0;
		default: /* reception de nbo_lus octets */
		printf("Message recu du serveur : %s (%d octets)\n\n", messageRecu, nbo_lus);
	}
	// On ferme toujours la socket avant de quitter
	close(sock);
	return EXIT_SUCCESS;
}		
