#include "dict.h"

#include <stdbool.h>

Trie* create() {
    Trie* result = malloc(sizeof(Trie));

    for (int q = 0; q < 26; q++) {
        result->child[q] = NULL;
    }
    result->IsWord = false;
    result->wordpath = false;

    return result;
}

void insert(Trie* dict, char* str) {
    int c = *str;
    if (c < 'a' || c > 'z') {
        dict->IsWord = true;
        return;
    }
    c -= 'a';
    if (dict->child[c] == NULL) {
        dict->child[c] = create();
        insert(dict->child[c], str + 1);
    } else {
        insert(dict->child[c], str + 1);
    }
}

bool lookup(Trie* dict, char* str) {
    if (dict == NULL) {
        return true;
    }
    int c = *str;
    if (c == '\0') {
        return dict->IsWord;
    }
    c -= 'a';
    if (dict->child[c] == NULL) {
        return false;
    } else {
        return lookup(dict->child[c], str + 1);
    }
}

void destroy(Trie* dict) {
    for (int q = 0; q < 26; q++) {
        if (dict->child[q] != NULL) {
            destroy(dict->child[q]);
        }
    }

    free(dict);
}
// end
