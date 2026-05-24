#include "shortest_path.h"
#include "binary_heap.h"

HeapInt* heap_create(int size)
{
    HeapInt* heap = (HeapInt*)calloc(1, sizeof(HeapInt));
    heap->pairs = (HeapIntPair*)calloc(size, sizeof(HeapIntPair));
    heap->size = 0;
    heap->capacity = size;
    return heap;

}
void heap_destroy(HeapInt* heap)
{
    if (heap == NULL)
        return;

    if (heap->pairs != NULL)
    {
        free(heap->pairs);
        heap->pairs = NULL;
    }

    free(heap);
}
void heap_down(HeapInt* heap, int i)
{
    int largest = i;
    int leftchild = 2 * i + 1;
    int rightchild = 2 * i + 2;

    if (leftchild < heap->size && heap->pairs[leftchild].priority >heap->pairs[largest].priority)
    {
        largest = leftchild;
    }
    if (rightchild < heap->size && heap->pairs[rightchild].priority > heap->pairs[largest].priority)
    {
        largest = rightchild;
    }
    if (largest != i)
    {
        swap(&heap->pairs[i], &heap->pairs[largest]);
        heap_down(heap, largest);
    }
}

void heap_up(HeapInt* heap, int i)
{
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (heap->pairs[i].priority > heap->pairs[parent].priority)
        {
            swap(&heap->pairs[i], &heap->pairs[parent]);
            i = parent;
        }
        else
        {
            break;
        }
    }
}
void heap_insertnode(HeapInt* heap, int element, float priority)
{
    if (heap->size >= heap->capacity)
    {
        return -1;
    }
    heap->pairs[heap->size].priority = priority;
    heap->pairs[heap->size].value = element;
    heap->size++;
    heap_up(heap, heap->size - 1);
    return 0;
}

HeapIntPair heap_extract_max(HeapInt* heap)
{
    HeapIntPair max = { -INFINITY, -1 };

    if (heap->size == 0)
        return max;

    max = heap->pairs[0];
    heap->pairs[0] = heap->pairs[heap->size - 1];
    heap->size--;

    if (heap->size > 0)
        heap_down(heap, 0);

    return max;
}
void heap_increase_key(HeapInt* heap, int nodeId, float newPriority)
{
    int idx = -1;
    for (int i = 0; i < heap->size; i++)
    {
        if (heap->pairs[i].value == nodeId)
        {
            idx = i;
            break;
        }
    }

    if (idx >= 0 && newPriority > heap->pairs[idx].priority)
    {
        heap->pairs[idx].priority = newPriority;
        heap_up(heap, idx);
    }
}
void swap(HeapIntPair* a, HeapIntPair* b)
{
    HeapIntPair temp = *a;
    *a = *b;
    *b = temp;
}
