#include "kv.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

unsigned kv_hash(const char* key) {
    unsigned int h = 0;
    while (*key){
        h = h * 31 + (unsigned char)(*key);
        key++;
    }
    return h;
}

KVStore* kv_create(void) {
    KVStore* store = (KVStore*)malloc(sizeof(KVStore));
    if (store == NULL) {
        perror("malloc failed");
        return NULL;
    }
    for (int i = 0; i < HASH_SIZE; i ++) {
        store->buckets[i] = NULL;
    }
    return store;
}

void kv_destroy(KVStore* store) {
    for (int i = 0; i < HASH_SIZE; i++) {
        KVNode* current = store->buckets[i];
        while (current != NULL) {
            KVNode* temp = current->next;
            free(current);
            current = temp;

        }
        store->buckets[i] = NULL;

    }
    free(store);
    store = NULL;
}

int kv_put(KVStore* store, const char* key, const char* value) {
    int buchet_num = kv_hash(key) % HASH_SIZE;

    KVNode* new_node = (KVNode*)malloc(sizeof(KVNode));
    if (new_node == NULL) {
        perror("malloc failed: ");
        return -1;

    }

    int needed1 = snprintf(new_node->key, sizeof(new_node->key), "%s", key);
    int needed2 = snprintf(new_node->value, sizeof(new_node->value), "%s", value);
    if ((unsigned long)needed1 >= sizeof(new_node->key) || (unsigned long)needed2 >= sizeof(new_node->value)) {
        perror("snprintf failed\n");
        free(new_node);
        new_node = NULL;
        return -1;
    }

    // try to find existing key
    KVNode* current = store->buckets[buchet_num];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            int need = snprintf(current->value, sizeof(current->value), "%s", value);
            if ((unsigned long)need >= sizeof(current->value)) {
                perror("snprintf failed\n");
                free(new_node);
                new_node = NULL;
                return -1;
            }
            free(new_node);
            new_node = NULL;
            return 0;
        }
        current = current->next;
    }

    // no existing key, putin a new one
    new_node->next = store->buckets[buchet_num];
    store->buckets[buchet_num] = new_node;
    return 0;
}

int kv_get(KVStore* store, const char* key, char* out_value, size_t out_size) {
    int bucket_num = kv_hash(key) % HASH_SIZE;

    KVNode* current = store->buckets[bucket_num];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            //
            int need = snprintf(out_value, out_size, "%s", current->value);  //
            if ((unsigned long)need >= out_size) {
                perror("snprintf failed\n");
                return -1;

            }
            printf("value: %s\n", out_value);
            return 0;
        }
        current = current->next;
    }
    fprintf(stderr, "no matched key\n");
    return -1;
}

int kv_delete(KVStore* store, const char* key) {
    int bucket_num = kv_hash(key) % HASH_SIZE;

    KVNode* current = store->buckets[bucket_num];
    KVNode* previous = NULL;
    // current = NULL
    if (current == NULL) {
        perror("empty list\n");
        return -1;
    }

    // del head
    else if (strcmp(current->key, key) == 0) {
        store->buckets[bucket_num] = current->next;
        free(current);
        current = NULL;
        return 0;
        }
    // del other
    while (current->next != NULL) {
        previous = current;
        current = current->next;
        if (strcmp(current->key, key) == 0) {
            //
            previous->next = current->next;
            free(current);
            current = NULL;
            return 0;
            }
        }
    fprintf(stderr, "no matched key\n");
    return -1;
    }