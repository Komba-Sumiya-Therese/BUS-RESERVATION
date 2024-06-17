#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Definitions des structures
typedef struct {
    char nom[50];
    char contact[50];
    int numeroSiege;
    char itineraireBus[50];
    int codeReservation;
} Passager;

typedef struct {
    int numeroBus;
    char itineraire[50];
    int sieges[40]; // 0: Disponible, 1: Reservé
} Bus;

// Prototypes des fonctions
void afficherMenu();
void rechercherItinerairesBus();
void reserverSiege();
void afficherReservations();
void effectuerPaiement();
void genererBillet(Passager passager);
void rechercherPassager();
void ajouterBus();

// Opérations de fichiers
void sauvegarderInfoPassager(Passager passager);
void chargerInfoBus();
void sauvegarderInfoBus();

// Variables globales
Bus bus[5];
int nombreBus = 0;

int main() {
    chargerInfoBus();
    int choix;
    do {
        afficherMenu();
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                system("cls");
                rechercherItinerairesBus();
                break;
            case 2:
                system("cls");
                reserverSiege();
                break;
            case 3:
                system("cls");
                afficherReservations();
                break;
            case 4:
                system("cls");
                rechercherPassager();
                break;
            case 5:
                system("cls");
                ajouterBus();
                break;
            case 6:
                system("cls");
                printf("Merci d'utiliser le systeme de reservation de bus.\n");
                break;
            default:
                printf("Choix invalide, veuillez reessayer.\n");
        }
        sleep(3);
    } while (choix != 6);
    return 0;
}

// Afficher le menu principal
void afficherMenu() {
    printf("\n--- Systeme de Reservation de Bus ---\n\n");
    printf("1. Rechercher des itineraires de bus\n");
    printf("2. Reserver un siege\n");
    printf("3. Afficher les reservations\n");
    printf("4. Rechercher un passager\n");
    printf("5. Ajouter un bus\n");
    printf("6. Quitter\n\n");
    printf("Entrez votre choix: ");
}

// Rechercher et afficher les itineraires des bus disponibles
void rechercherItinerairesBus() {
    printf("\nItineraires de bus disponibles:\n");
    for (int i = 0; i < nombreBus; i++) {
        printf("Numero du bus: %d, Itineraire: %s\n", bus[i].numeroBus, bus[i].itineraire);
    }
}

// Reserver un siège pour un passager
void reserverSiege() {
    Passager passager;
    int numeroBus, numeroSiege;

    srand(time(NULL));

    printf("\nEntrez le numero du bus: ");
    scanf("%d", &numeroBus);
    for (int i = 0; i < nombreBus; i++) {
        if (bus[i].numeroBus == numeroBus) {
            printf("Sieges disponibles: ");
            for (int j = 0; j < 40; j++) {
                if (bus[i].sieges[j] == 0) {
                    printf("%d ", j + 1);
                }
            }
            printf("\nEntrez le numero du siege: ");
            scanf("%d", &numeroSiege);

            if (numeroSiege < 1 || numeroSiege > 40 || bus[i].sieges[numeroSiege - 1] == 1) {
                printf("Numero de siege invalide ou d�ja reserve.\n");
                return;
            }

            bus[i].sieges[numeroSiege - 1] = 1;
            printf("Entrez le nom du passager: ");
            scanf("%s", passager.nom);
            printf("Entrez le numero de contact du passager: ");
            scanf("%s", passager.contact);
            passager.numeroSiege = numeroSiege;
            strcpy(passager.itineraireBus, bus[i].itineraire);
            passager.codeReservation = rand() % 10000 + 1; // Code de réservation aléatoire simple

            sauvegarderInfoPassager(passager);
            sauvegarderInfoBus();
            effectuerPaiement();
            genererBillet(passager);
            return;
        }
    }
    printf("Bus non trouve.\n");
}

// Afficher toutes les réservations existantes
void afficherReservations() {
    FILE *file = fopen("reservations.txt", "r");
    if (!file) {
        printf("Aucune reservation trouvee.\n");
        return;
    }

    Passager passager;
    printf("\nReservations actuelles:\n");
    while (fread(&passager, sizeof(Passager), 1, file)) {
        printf("Code de reservation: %d, Nom: %s, Contact: %s, Numero de siege: %d, Itineraire: %s\n",
               passager.codeReservation, passager.nom, passager.contact,
               passager.numeroSiege, passager.itineraireBus);
    }
    fclose(file);
}

// Rechercher un passager par nom ou code de réservation
void rechercherPassager() {
    FILE *file = fopen("reservations.txt", "r");
    if (!file) {
        printf("Aucune reservation trouvee.\n");
        return;
    }

    char nom[50];
    printf("Entrez le nom du passager ou le code de reservation: ");
    scanf("%s", nom);

    Passager passager;
    int trouve = 0;
    while (fread(&passager, sizeof(Passager), 1, file)) {
        if (strcmp(passager.nom, nom) == 0 || passager.codeReservation == atoi(nom)) {
            printf("Code de reservation: %d, Nom: %s, Contact: %s, Numero de siege: %d, Itineraire: %s\n",
                   passager.codeReservation, passager.nom, passager.contact,
                   passager.numeroSiege, passager.itineraireBus);
            trouve = 1;
            break;
        }
    }
    if (!trouve) {
        printf("Passager non trouve.\n");
    }
    fclose(file);
}

// Effectuer le paiement (simulation)
void effectuerPaiement() {
    printf("Paiement reussi via Mobile Money.\n");
}

// Générer le billet après la réservation
void genererBillet(Passager passager) {
    system("cls");
    printf("\n------ Billet ------\n");
    printf("Code de reservation: %d\n", passager.codeReservation);
    printf("Nom: %s\n", passager.nom);
    printf("Contact: %s\n", passager.contact);
    printf("Numero de siege: %d\n", passager.numeroSiege);
    printf("Itineraire: %s\n", passager.itineraireBus);
    printf("----------------------\n");
}

// Sauvegarder les informations du passager dans un fichier
void sauvegarderInfoPassager(Passager passager) {
    FILE *file = fopen("reservations.txt", "ab");
    fwrite(&passager, sizeof(Passager), 1, file);
    fclose(file);
}

// Charger les informations des bus depuis un fichier
void chargerInfoBus() {
    FILE *file = fopen("buses.txt", "r");
    if (file) {
        fread(&nombreBus, sizeof(int), 1, file);
        fread(bus, sizeof(Bus), nombreBus, file);
        fclose(file);
    } else {
        nombreBus = 2;
        bus[0] = (Bus){1, "Route A", {0}};
        bus[1] = (Bus){2, "Route B", {0}};
        sauvegarderInfoBus();
    }
}

// Sauvegarder les informations des bus dans un fichier
void sauvegarderInfoBus() {
    FILE *file = fopen("buses.txt", "w");
    fwrite(&nombreBus, sizeof(int), 1, file);
    fwrite(bus, sizeof(Bus), nombreBus, file);
    fclose(file);
}

// Ajouter un nouveau bus
void ajouterBus() {
    if (nombreBus >= 5) {
        printf("Impossible d'ajouter plus de bus. Limite maximale atteinte.\n");
        return;
    }

    Bus nouveauBus;
    printf("Entrez le numero du bus: ");
    scanf("%d", &nouveauBus.numeroBus);
    printf("Entrez l'itineraire du bus: ");
    scanf("%s", nouveauBus.itineraire);
    memset(nouveauBus.sieges, 0, sizeof(nouveauBus.sieges)); // Initialiser tous les sièges à 0 (disponible)

    bus[nombreBus] = nouveauBus;
    nombreBus++;
    sauvegarderInfoBus();

    printf("Nouveau bus ajoute avec succes.\n");
}
