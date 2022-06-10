#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


struct Trie {
    // This is an array of pointers to all next possible paths
    // from ('a'=0) to ('z'=25), if child[i] has the value null,
    // then we have no path, otherwise we do have a path
    struct Trie *child[26];

    // this boolean value will indicate if this current Trie node
    // is an end of a word or not
    bool IsWord;
    bool wordpath;
};

typedef struct Trie Trie;

Trie *create();
void insert(Trie *dict, char *str);
bool lookup(Trie *dict, char *str);
void destroy(Trie *dict);
