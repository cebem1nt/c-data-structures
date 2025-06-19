#include "../map/map.h"
#include <stdio.h>

int main() 
{
    Map* map = map_create();

    map_insert(map, "key1", 1);
    map_insert(map, "key2", 1);
    map_insert(map, "key3", 1);
    map_insert(map, "key4", 4);
    map_insert(map, "key5", 1);

    map_print(map);

    printf("%i", *map_get(map, "key4"));
}