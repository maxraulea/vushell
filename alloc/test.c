#include <stdio.h>
#include <unistd.h>

typedef struct obj_metadata{
size_t size;
long int is_free;
struct obj_metadata *next;
} obj_metadata;


int main(int argc, char **argv) {
    printf("%ld\n", sizeof(obj_metadata));
}
