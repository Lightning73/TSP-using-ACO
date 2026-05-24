/*
    Copyright (c) Arnaud BANNIER, Nicolas BODIN and Matthieu LE BERRE.
    Licensed under the MIT License.
    See LICENSE.md in the project root for license information.
*/

#include "shortest_path.h"
#include "utils/heap_int.h"
#include "utils/list_int.h"

Path* Graph_shortestPath(Graph* self, int start, int end)
{
    assert(self && "self must not be NULL");
    assert(0 <= start && start < self->vertexCount && "The start is invalid");
    assert(end < self->vertexCount && "The end is invalid");

    const int vertexCount = Graph_getVertexCount(self);

    int* predecessors = (int*)calloc(vertexCount, sizeof(int));
    AssertNew(predecessors);

    float* distances = (float*)calloc(vertexCount, sizeof(float));
    AssertNew(distances);

    HeapInt* heap = heap_create(vertexCount);

    Graph_dijkstra(self, start, end, distances, predecessors, heap);

    Path* path = Graph_dijkstraGetPath(distances, predecessors, end);

    free(predecessors);
    predecessors = NULL;
    free(distances);
    distances = NULL;
    heap_destroy(heap);
    heap = NULL;
    return path;
}

void Graph_dijkstra(Graph* self, int start, int end, float* distances, int* predecessors, HeapInt* heap)
{
    assert(self && "self must not be NULL");
    assert(0 <= start && start < self->vertexCount && "The start is invalid");
    assert(end < self->vertexCount && "The end is invalid");
    //assert(predecessors && distances);

    const int vertexCount = Graph_getVertexCount(self);

    assert(0 <= start && start < vertexCount);
    assert(end < vertexCount);

    bool* explored = (bool*)calloc(vertexCount, sizeof(bool));
    AssertNew(explored);

    for (int i = 0; i < vertexCount; i++)
    {
        distances[i] = INFINITY;
        predecessors[i] = -1;
    }
    distances[start] = 0.0f;
    heap_insertnode(heap, start, -0.0f);
    explored[start] = true;
    while (heap->size > 0)
    {
        HeapIntPair max = heap_extract_max(heap);
        int currID = max.value;
        float inversedDist = max.priority;
        float currDist = -inversedDist;

        // Condition d'arret
        if (currID < 0 || currID == end)
            break;

        explored[currID] = true;

        // Met à jour les voisins
        for (ArcList* arc = Graph_getArcList(self, currID);
            arc != NULL; arc = arc->next)
        {
            int nextID = arc->target;
            float dist = distances[currID] + arc->weight;
            if (dist < distances[nextID])
            {
                distances[nextID] = dist;
                predecessors[nextID] = currID;

                if (explored[nextID])
                {
                    // Increase key avec la priorité inversée
                    heap_increase_key(heap, nextID, -dist);
                }
                else
                {
                    // Insert avec la priorité inversée
                    heap_insertnode(heap, nextID, -dist);
                    explored[nextID] = true;
                }
            }
        }
    }
    free(explored);
}

Path* Graph_dijkstraGetPath(float* distances, int* predecessors, int end)
{
    assert(end >= 0);
    if (distances[end] == INFINITY)
    {
        return NULL;
    }

    Path* path = Path_create(end);
    float total = distances[end];
    int currID = end;

    while (predecessors[currID] >= 0)
    {
        currID = predecessors[currID];
        ListInt_insertFirst(path->list, currID);
    }

    path->distance = total;

    return path;
}

Path* Path_create(int start)
{
    Path* path = (Path*)calloc(1, sizeof(Path));
    AssertNew(path);

    path->distance = 0.0f;
    path->list = ListInt_create();
    ListInt_insertLast(path->list, start);

    return path;
}

void Path_destroy(Path* path)
{
    if (path == NULL) return;

    ListInt_destroy(path->list);
    free(path);
}

void Path_print(Path* path)
{
    if (path == NULL)
    {
        printf("path : NULL\n");
        return;
    }

    printf("%.1f\n", path->distance);
    printf("%d\n", ListInt_size(path->list));
    bool first = true;
    for (ListIntNode* node = path->list->sentinel.next; node != &path->list->sentinel; node = node->next)
    {
        if (!first)
            printf(" ");
        printf("%d", node->value);
        first = false;
    }
    printf("\n");
}
