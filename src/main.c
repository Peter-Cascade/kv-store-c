#include "kv.h"
#include <stdio.h>

int main(void) {
    KVStore* store = kv_create();
    while (1){
        printf("kv> ");
        char buffer[32] = {0};  // ** *
        char* order = NULL;
        char* key = NULL;
        char* value = NULL;
        char* other = NULL;
        char* result = fgets(buffer, 32, stdin);
        if (result == NULL) {
            perror("fget failed\n");
        }
        
        order = strtok(buffer, " \n");
        key = strtok(NULL, " \n");
        value = strtok(NULL, " \n");
        other = strtok(NULL, " \n");

        // exit
        if (strcmp(order, "exit") == 0 && key == NULL) {
            kv_destroy(store);
            break;

        }

        // get
        else if(strcmp(order, "get") == 0 && key != NULL && value == NULL) {
            char out_value[KV_VAL_MAX];  // !
            int out_size = KV_VAL_MAX;
            kv_get(store, key, out_value, out_size);
            continue;
        }

        // del
        else if (strcmp(order, "del") == 0 && key != NULL && value == NULL) {
            kv_delete(store, key);
            continue;
        }

        // put
        else if (strcmp(order, "put") == 0 && key != NULL && value != NULL && other == NULL) {
            kv_put(store, key, value);
            continue;
        }

        else {
            fprintf(stderr, "invalid input\n");
            continue;
        }

    }
    return 0;

}