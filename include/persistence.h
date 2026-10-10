#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "kv.h"

extern char* default_file;

int kv_save(KVStore* store, const char* filename);

int kv_load(KVStore* store, const char* filename);

#endif