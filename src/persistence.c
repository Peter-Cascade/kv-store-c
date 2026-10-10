#include "kv.h"
#include "persistence.h"

char* default_file = "data.txt";

int kv_save(KVStore* store, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("fopen failed: ");
        return -1;
    }

    for (int i = 0; i < HASH_SIZE; i++) {
        KVNode* current = store->buckets[i];
        while (current != NULL) {
            int count = fprintf(fp, "%s:%s\n", current->key, current->value);
            if (count < 0) {
                return -1;
            }

            current = current->next;
        }
    }

    // close
    int check = fclose(fp);
    if (check != 0) {
        return -1;
    }

    return 0;
}

int kv_load(KVStore* store, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return -1;
    }
    
    char buffer[2048];
    // read every line
    while (fgets(buffer, KV_KEY_MAX + KV_VAL_MAX + 1, fp) != NULL) {
        // del \n
        buffer[strcspn(buffer, "\n")] = '\0';
        
        // :
        char* colon = strchr(buffer, ':');
        if (colon != NULL) {
            *colon = '\0'; // cut
            char* key = buffer;
            char* value = colon + 1;
            
            // pass data to kv_put 
            kv_put(store, key, value);
        }
    }
    int check = fclose(fp);
    if (check != 0) {
        return -1;
    }

    return 0;
}