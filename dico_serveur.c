#include <stdio.h>
#include <stdlib.h> /* pour exit */
#include <unistd.h> /* pour sleep */
#include <string.h> /* pour memset */
#include <sys/types.h> /* pour socket */
#include <sys/socket.h>
#include <netinet/in.h> /* pour struct sockaddr_in */
#include <arpa/inet.h> /* pour htons et inet_aton */
#define PORT IPPORT_USERRESERVED // = 5000
#define LG_MESSAGE 256

int main() {
	int socketEcoute,socketPrivee;
	struct sockaddr_in adresse; /* adresse dâ€™attachement locale */
	socklen_t longueurAdresse;

	int nbo_ecrits, nbo_lus;
	char messageEnvoi[LG_MESSAGE]; /* le message envoye */
	char messageRecu[LG_MESSAGE]; /* le message recu */
	
	socketEcoute = socket(AF_INET, SOCK_STREAM, 0);
	if(socketEcoute < 0) {
		perror("socket");
		exit(1);
	}
	printf("Socket creee avec succes ! (%d)\n", socketEcoute);
	// On prÃ©pare lâ€™adresse dâ€™attachement locale
	longueurAdresse = sizeof(struct sockaddr_in);
	memset(&adresse, 0x00, longueurAdresse);
	adresse.sin_family = AF_INET;
	adresse.sin_addr.s_addr = htonl(INADDR_ANY);
	adresse.sin_port = htons(PORT); // = 5000
	// On demande lâ€™attachement local de la socket
	if(( bind(socketEcoute, (struct sockaddr *)&adresse, longueurAdresse) ) < 0) {
		perror("bind");
		exit(2);
	}
	printf("Socket attachee avec succes !\n");
	// La taille de la file dâ€™attente est fixee a 5
	if(listen(socketEcoute, 5) < 0) {
		perror("listen");
		exit(3);
	}
	printf("Socket placee en ecoute passive ...\n");
	// On ferme la ressource avant de quitter
	
	while (1) {
	printf("\nAttente d'une demande de connexion (quitter avec Ctrl-C)\n");
	
	// accept est un appel systeme bloquant!
	socketPrivee = accept(socketEcoute, (struct sockaddr *)&adresse, &longueurAdresse);
	if (socketPrivee < 0) {
		perror("accept");
		close(socketPrivee);
		close(socketEcoute);
		exit(1); // Erreur
	}
	
	// On initialise Ã  0 les messages
	memset(messageEnvoi, 0x00, LG_MESSAGE*sizeof(char));
	memset(messageRecu, 0x00, LG_MESSAGE*sizeof(char));
	
	/* On recoit des donnÃ©es du serveur */
	nbo_lus = read(socketPrivee, messageRecu, LG_MESSAGE*sizeof(char)); /* on attend un message de TAILLE fixe */
	switch(nbo_lus) {
		case -1 :
		perror("read");
		close(socketPrivee);
		exit(4);
		case 0 : /* la socket est fermee */
		fprintf(stderr, "La socket a ete fermee par le serveur !\n\n");
		close(socketPrivee);
		exit(5);
		default: /* reception de nbo_lus octets */
		printf("Message recu du serveur : %s (%d octets)\n\n", messageRecu, nbo_lus);
	}
	// Envoie un message au serveur
// sleep(10);

	sprintf(messageEnvoi, "Salut client!\n");
	nbo_ecrits = write(socketPrivee, messageEnvoi, strlen(messageEnvoi));
	switch(nbo_ecrits) {
		case -1 : /* en cas d'une erreur */
		perror("write");
		close(socketPrivee);
		exit(6);
		case 0 : /* la socket est fermee */
		fprintf(stderr, "La socket a ete fermee par le serveur !\n\n");
		close(socketPrivee);
		exit(7);
		default: /* envoi de n octets */
		printf("Message %s a ete envoye avec succes (%d octets)\n\n", messageEnvoi, nbo_ecrits);
	}
	
	shutdown(socketPrivee,2); // Equivalent Ã  close(socketPrivee)
}
	shutdown(socketEcoute,2);
	return EXIT_SUCCESS;
}	

