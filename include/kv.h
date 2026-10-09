#ifndef KV_H
#define KV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KV_KEY_MAX 256
#define KV_VAL_MAX 1024
#define HASH_SIZE 1024

typedef struct KVNode {
    char key[KV_KEY_MAX];
    char value[KV_VAL_MAX];
    struct KVNode* next;
} KVNode;

typedef struct KVStore {
    KVNode* buckets[HASH_SIZE];
} KVStore;

KVStore* kv_create();

void kv_destroy(KVStore* store);

int kv_put(KVStore* store, const char* key, const char* value);

int kv_get(KVStore* store, const char* key, char* out_value, size_t out_size);

int kv_delete(KVStore* store, const char* key);

unsigned int kv_hash(const char* key);

#endif