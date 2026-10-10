#include "kv.h"
#include "persistence.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    KVStore* store = kv_create();

    // 
    if (argc == 2 && strcmp(argv[0], "./kv_store") == 0) {
        default_file = argv[1];
    }
    kv_load(store, default_file);
    
    while (1){
        printf("kv> ");
        char buffer[32] = {0};  // ** *
        char* order = NULL;
        char* key = NULL;
        char* value = NULL;
        char* other = NULL;
        char* result = fgets(buffer, 32, stdin);
        
        if (result == NULL) {
            printf("\n");  
            kv_save(store, default_file);
            kv_destroy(store);
            break;
        }
        
        order = strtok(buffer, " \n");
        key = strtok(NULL, " \n");
        value = strtok(NULL, " \n");
        other = strtok(NULL, " \n");

        // exit
        if (strcmp(order, "exit") == 0 && key == NULL) {
            int check = kv_save(store, default_file);
            if (check < 0) {
                perror("save failed: ");
            }
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

        else if (strcmp(order, "save") == 0 && key == NULL) {
            int check = kv_save(store, default_file);
            if (check < 0) {
                perror("save failed: ");
                kv_destroy(store);
                break;
            }
            continue;
        }

        else if (strcmp(order, "load") == 0 && key == NULL) {
            int check = kv_load(store, default_file);
            if (check < 0) {
                printf("kv_load failed\n");
                kv_destroy(store);
                break;
            }
            continue;
        }

        else {
            fprintf(stderr, "invalid input\n");
            continue;
        }

    }
    return 0;

}