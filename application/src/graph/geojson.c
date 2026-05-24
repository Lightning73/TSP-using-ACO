#include "geojson.h"
#include "graph.h"
#include "utils/heap_int.h"

Coordonees* coordonees(char* fileGPS)
{

    FILE* pfile = fopen(fileGPS, "r");
    if (pfile == NULL)
    {
        printf("Erreur ouverture fichier GPS\n");
        return NULL;
    }

    int nb = 0;
    float latitude, longitude;
    fscanf(pfile, "%d", &nb);
    Coordonees* tabCoord = (Coordonees*)calloc(nb, sizeof(Coordonees));
    //on récupère toutes les latitudes et longitudes des points qu'on stocke dans un seul indice de tableau 
    //pour faciliter le parcours par indice
    for (int i = 0; i < nb; i++)
    {
        fscanf(pfile, "%f %f", &longitude, &latitude);
        tabCoord[i].latitude = latitude;
        tabCoord[i].longitude = longitude;
    }
    fclose(pfile);
    return tabCoord;
}

void ConvertToGeojson(Graph* graph, Path* path, char* fileJson, char* fileGPS, int* points, int nbpoints)
{
    Coordonees* tabCoord = coordonees(fileGPS);
    FILE* pfile = fopen(fileJson, "w");
    if (pfile == NULL)
    {
        printf("Erreur creation fichier json\n");
        return;
    }

    fprintf(pfile, "{\n");
    fprintf(pfile, "    \"type\": \"FeatureCollection\",\n");
    fprintf(pfile, "    \"features\" : [\n");


    // Mettre les points à visiter en rouge
    for (int j = 0; j < nbpoints; j++)
    {
        fprintf(pfile, "    {\n");
        fprintf(pfile, "      \"type\": \"Feature\",\n");
        fprintf(pfile, "      \"geometry\": {\n");
        fprintf(pfile, "        \"type\": \"Point\",\n");
        fprintf(pfile, "        \"coordinates\": [%f, %f]\n", tabCoord[points[j]].longitude, tabCoord[points[j]].latitude);
        fprintf(pfile, "      },\n");
        fprintf(pfile, "      \"properties\": {\n");
        fprintf(pfile, "        \"name\": \"Point %d\",\n", j + 1);
        fprintf(pfile, "        \"_umap_options\": {\n");
        fprintf(pfile, "          \"color\": \"Red\",\n");
        fprintf(pfile, "          \"iconClass\": \"Drop\"\n");
        fprintf(pfile, "        }\n");
        fprintf(pfile, "      }\n");
        fprintf(pfile, "    }");

        if (j < nbpoints - 1 || path->list->nodeCount > 0)
        {
            fprintf(pfile, ",");
        }

        fprintf(pfile, "\n");
    }
    int prev = points[ListInt_popFirst(path->list)];
    int valeur = prev;
    for (int g = 0; g < nbpoints; g++)
    {
        //on recupère le plus court chemin entre le point actuel et celui d'avant
        valeur = points[ListInt_popFirst(path->list)];
        Path* pcc_points = Graph_shortestPath(graph, prev, valeur);
        prev = valeur;

        fprintf(pfile, "      {\n");
        fprintf(pfile, "        \"type\": \"Feature\",\n");
        fprintf(pfile, "        \"geometry\" :\n");
        fprintf(pfile, "      {\n");
        fprintf(pfile, "            \"type\": \"LineString\",\n");
        fprintf(pfile, "            \"coordinates\" : [");

        int nb = pcc_points->list->nodeCount;
        for (int i = 0; i < nb; i++)
        {
            int trajet = ListInt_popFirst(pcc_points->list);
            fprintf(pfile, "\n           [%f, %f]", tabCoord[trajet].longitude, tabCoord[trajet].latitude);

            if (i < nb - 1) //ajouter les virgules, sauf à la fin
            {
                fprintf(pfile, ",");
            }
        }
        fprintf(pfile, "\n            ]\n");
        fprintf(pfile, "       }\n");
        if (g < nbpoints - 1) //ajouter les virgules, sauf à la fin
        {
            fprintf(pfile, "      },\n");
        }


    }
    fprintf(pfile, "      }\n");
    fprintf(pfile, "    ]\n");
    fprintf(pfile, "}\n");
    fclose(pfile);
}
