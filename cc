#ifndef GRAPH_H
#define GRAPH_H

#define MAX_SPECIES 100  // Nombre maximal d'espèces

typedef struct {
    char name[50];
    int population;
    int trophic_level;
} Species;

typedef struct {
    Species species[MAX_SPECIES];
    int adj_matrix[MAX_SPECIES][MAX_SPECIES]; // Matrice d'adjacence
    int num_species;
} TrophicNetwork;

// Fonctions de gestion du réseau trophique
void init_network(TrophicNetwork *network);
void add_species(TrophicNetwork *network, const char *name, int population, int trophic_level);
void add_interaction(TrophicNetwork *network, int from, int to);
void display_network(const TrophicNetwork *network);
void save_network(const TrophicNetwork *network, const char *filename);
void load_network(TrophicNetwork *network, const char *filename);

#endif
#include <stdio.h>
#include <string.h>
#include "graph.h"

// Initialisation du réseau trophique
void init_network(TrophicNetwork *network) {
    network->num_species = 0;
    memset(network->adj_matrix, 0, sizeof(network->adj_matrix));
}

// Ajouter une espèce au réseau
void add_species(TrophicNetwork *network, const char *name, int population, int trophic_level) {
    if (network->num_species >= MAX_SPECIES) {
        printf("Erreur : Nombre maximal d'espèces atteint.\n");
        return;
    }
    Species *new_species = &network->species[network->num_species];
    strncpy(new_species->name, name, 50);
    new_species->population = population;
    new_species->trophic_level = trophic_level;
    network->num_species++;
}

// Ajouter une interaction entre deux espèces
void add_interaction(TrophicNetwork *network, int from, int to) {
    if (from >= network->num_species || to >= network->num_species) {
        printf("Erreur : Index d'espèce invalide.\n");
        return;
    }
    network->adj_matrix[from][to] = 1; // Interaction du prédateur (from) vers la proie (to)
}

// Afficher le réseau trophique
void display_network(const TrophicNetwork *network) {
    printf("\nRéseau trophique :\n");
    for (int i = 0; i < network->num_species; i++) {
        printf("Espèce %d : %s, Population : %d, Niveau trophique : %d\n",
               i, network->species[i].name, network->species[i].population,
               network->species[i].trophic_level);
    }

    printf("\nMatrice d'adjacence :\n");
    for (int i = 0; i < network->num_species; i++) {
        for (int j = 0; j < network->num_species; j++) {
            printf("%d ", network->adj_matrix[i][j]);
        }
        printf("\n");
    }
}

// Sauvegarder un réseau trophique dans un fichier
void save_network(const TrophicNetwork *network, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        return;
    }
    fprintf(file, "%d\n", network->num_species);
    for (int i = 0; i < network->num_species; i++) {
        fprintf(file, "%s %d %d\n", network->species[i].name,
                network->species[i].population, network->species[i].trophic_level);
    }
    for (int i = 0; i < network->num_species; i++) {
        for (int j = 0; j < network->num_species; j++) {
            fprintf(file, "%d ", network->adj_matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

// Charger un réseau trophique depuis un fichier
void load_network(TrophicNetwork *network, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        return;
    }
    init_network(network);
    fscanf(file, "%d", &network->num_species);
    for (int i = 0; i < network->num_species; i++) {
        fscanf(file, "%s %d %d", network->species[i].name,
               &network->species[i].population, &network->species[i].trophic_level);
    }
    for (int i = 0; i < network->num_species; i++) {
        for (int j = 0; j < network->num_species; j++) {
            fscanf(file, "%d", &network->adj_matrix[i][j]);
        }
    }
    fclose(file);
}

#include <stdio.h>
#include "graph.h"

int main() {
    TrophicNetwork network;
    init_network(&network);

    int choice;
    char filename[100];

    while (1) {
        printf("\n1. Ajouter une espèce\n");
        printf("2. Ajouter une interaction\n");
        printf("3. Afficher le réseau\n");
        printf("4. Sauvegarder le réseau\n");
        printf("5. Charger un réseau\n");
        printf("6. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char name[50];
                int population, trophic_level;
                printf("Nom : ");
                scanf("%s", name);
                printf("Population : ");
                scanf("%d", &population);
                printf("Niveau trophique : ");
                scanf("%d", &trophic_level);
                add_species(&network, name, population, trophic_level);
                break;
            }
            case 2: {
                int from, to;
                printf("Interaction de (proie) : ");
                scanf("%d", &from);
                printf("Vers (predateur) : ");
                scanf("%d", &to);
                add_interaction(&network, from, to);
                break;
            }
            case 3:
                display_network(&network);
                break;
            case 4:
                printf("Nom du fichier : ");
                scanf("%s", filename);
                save_network(&network, filename);
                break;
            case 5:
                printf("Nom du fichier : ");
                scanf("%s", filename);
                load_network(&network, filename);
                break;
            case 6:
                printf("Au revoir");
                return 0;
            default:
                printf("Choix invalide.\n");
        }
    }
}
#include <stdio.h>
#include "simulation.h"

void simulate_population_dynamics(TrophicNetwork *network, int iterations) {
    for (int t = 0; t < iterations; t++) {
        printf("\nItération %d :\n", t + 1);
        for (int i = 0; i < network->num_species; i++) {
            int new_population = network->species[i].population;
            for (int j = 0; j < network->num_species; j++) {
                if (network->adj_matrix[j][i] == 1)
                {  // Si i dépend de j
                    new_population += network->species[j].population / 10;  // Simplification
                }
            }
            network->species[i].population = new_population;
            printf("%s : Population = %d\n", network->species[i].name, new_population);
        }
    }
}
