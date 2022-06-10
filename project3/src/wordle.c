#include "wordle.h"

#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "dict.h"
#include "util.h"

bool dfs(Trie *dict, int *letters) {
    if (dict->IsWord == true) {
        dict->wordpath = true;
        return true;
    }

    for (int q = 0; q < 26; q++) {
        if (dict->child[q] != NULL && letters[q] != 1) {
            dict->wordpath = dict->wordpath || dfs(dict->child[q], letters);
        }
    }

    return dict->wordpath;
}

Trie *generateDict(char *filename, int k, char *selected1,
                   /*@unused@*/ char *selected2) {
    FILE *f = fopen(filename, "r");
    char *c = malloc(k + 1);
    Trie *result = create();
    while (fscanf(f, "%s", c) != EOF) {
        c[k] = '\0';
        insert(result, c);
    }

    free(c);
    fclose(f);

    Trie *cur;
re_select1:
    cur = result;
    for (int q = 0; q < k; q++) {
        int valid_children[26];
        int sz = 0;
        for (int i = 0; i < 26; i++) {
            if (cur->child[i] != NULL) {
                valid_children[sz] = i;
                sz++;
            }
        }
        int random = sz * drand48();
        selected1[q] = (valid_children[random] + 'a');
        cur = cur->child[valid_children[random]];
    }
    selected1[k] = '\0';

    if (selected2 != NULL) {
        int *letters = malloc(26 * 4);
        memset(letters, 0, 26 * 4);
        for (int q = 0; q < k; q++) {
            letters[selected1[q] - 'a'] = 1;
        }
        bool flag = dfs(result, letters);
        free(letters);
        if (!flag) goto re_select1;
        cur = result;
        for (int q = 0; q < k; q++) {
            int valid_children[26];
            int sz = 0;
            for (int i = 0; i < 26; i++) {
                if (cur->child[i] != NULL && cur->child[i]->wordpath) {
                    valid_children[sz] = i;
                    sz++;
                }
            }
            int random = sz * drand48();
            selected2[q] = (valid_children[random] + 'a');
            cur = cur->child[valid_children[random]];
        }
        selected2[k] = '\0';
    }

    return result;
}

char *guess(Trie *dict, int k) {
    printf("Please input your guess: ");
    bool flag = false;
    char *c = malloc(k + 1);
    scanf("%s", c);
    for (int q = 0; q < k; q++) {
        if (c[q] >= 'A' && c[q] <= 'Z') c[q] = c[q] + 32;
    }
    if (c[k] == '\0') {
        flag = lookup(dict, c);
    }
    while (!flag) {
        printf("Invalid word. Try again: ");
        scanf("%s", c);
        for (int q = 0; q < k; q++) {
            if (c[q] >= 'A' && c[q] <= 'Z') c[q] = c[q] + 32;
        }
        if (c[k] == '\0') {
            flag = lookup(dict, c);
        }
    }

    return c;
}

feedback_result *getFeedback(char *guess, char *word1, char *word2, int k) {
    feedback_result *result = malloc(sizeof(feedback_result) * k);
    int w1[26];
    memset(w1, 0, 26 * 4);
    for (int q = 0; q < k; q++) {
        w1[word1[q] - 'a']++;
        result[q] = WRONG;
    }

    for (int q = 0; q < k; q++) {
        if (word1[q] == guess[q]) {
            result[q] = CORRECT;
            w1[word1[q] - 'a']--;
        }
    }
    for (int q = 0; q < k; q++) {
        if (result[q] != CORRECT) {
            if (w1[guess[q] - 'a'] > 0) {
                result[q] = WRONGPOS;
                w1[guess[q] - 'a']--;
            }
        }
    }
    bool flag1 = true;
    for (int q = 0; q < k; q++) {
        if (result[q] != WRONG) flag1 = false;
    }

    if (word2 != NULL) {
        int w2[26];
        memset(w2, 0, 26 * 4);
        feedback_result *ndfeed = malloc(sizeof(feedback_result) * k);
        for (int q = 0; q < k; q++) {
            w2[word2[q] - 'a']++;
            ndfeed[q] = WRONG;
        }

        for (int q = 0; q < k; q++) {
            if (word2[q] == guess[q]) {
                ndfeed[q] = CORRECT;
                w2[word2[q] - 'a']--;
            }
        }
        for (int q = 0; q < k; q++) {
            if (ndfeed[q] != CORRECT) {
                if (w2[guess[q] - 'a'] > 0) {
                    ndfeed[q] = WRONGPOS;
                    w2[guess[q] - 'a']--;
                }
            }
        }
        bool flag2 = true;
        for (int q = 0; q < k; q++) {
            if (ndfeed[q] != WRONG) flag2 = false;
        }
        if (flag1) {
            free(result);
            return ndfeed;
        } else if (flag2) {
            free(ndfeed);
            return result;
        } else {
            for (int q = 0; q < k; q++) {
                if (ndfeed[q] == CORRECT || result[q] == CORRECT)
                    result[q] = QUANTUMCORRECT;
                else if (ndfeed[q] == WRONGPOS || result[q] == WRONGPOS)
                    result[q] = QUANTUMWRONGPOS;
            }
            free(ndfeed);
        }
    }

    return result;
}

void printFeedback(feedback_result *feedback, int k) {
    printf("Result: ");
    for (int q = 0; q < k; q++) {
        if (feedback[q] == CORRECT) {
            printf("🟩");
        } else if (feedback[q] == WRONGPOS) {
            printf("🟨");
        } else if (feedback[q] == WRONG) {
            printf("⬛");
        } else if (feedback[q] == QUANTUMCORRECT) {
            printf("🟢");
        } else {
            printf("🟡");
        }
    }
    printf("\n");
}
bool checkWin(feedback_result *feedback, int k) {
    for (int q = 0; q < k; q++) {
        if (feedback[q] != CORRECT) return false;
    }
    return true;
}
