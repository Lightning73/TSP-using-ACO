#include "../settings.h"
#include "shortest_path.h"

typedef struct Coordonees Coordonees;

/// @brief Structure avec les coordonées d'un point.
struct Coordonees
{
    /// @brief La longitude
    float longitude;
    /// @brief La latitude
    float latitude;
};

/// @brief Crée un tableau de toutes les coordonées GPS des points.
/// @param fileGPS le fichier qui contient toutes les coordonées GPS des points.
/// @return Le tableau des coordonées.
Coordonees* coordonees(char* fileGPS);

/// @brief Convertis les points en un fichier .geojson .
/// @param Graph le graphe
/// @param path les points à récupérer
/// @param fileJson le fichier .json dans lequel on ecrit
/// @param fileGPS le fichier qui contient toutes les coordonées GPS des points.
/// @param tab des points à visiter
/// @param nb le nombre d'endroits à visiter
void ConvertToGeojson(Graph* graph, Path* path, char* fileJson, char* fileGPS, int* tab, int nb);
