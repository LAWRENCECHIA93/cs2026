#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char ch;
    int count;
    struct Node *next;
} Node;

Node *create_node(char ch) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->ch = ch;
    new_node->count = 1;
    new_node->next = NULL;
    return new_node;
}


void process_char(Node **head, Node **tail, Node *index_table[], int c) {
    unsigned char idx = (unsigned char)c;
    
    if (c == EOF) return;

    if (index_table[idx] != NULL) {
        index_table[idx]->count++;
    } else {
        Node *new_node = create_node((char)c);
        if (new_node == NULL) return;
        
        if (*head == NULL) {
            *head = new_node;
            *tail = new_node;
        } else {
            (*tail)->next = new_node;
            *tail = new_node;
        }
        index_table[idx] = new_node;
    }
}

void print_list(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        if (curr->ch == '\n') {
            printf("\\n:%d\n", curr->count);
        } else if (curr->ch == '\r') {
        } else {
            printf("%c:%d\n", curr->ch, curr->count);
        }
        curr = curr->next;
    }
}

void free_list(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
}

int main(void) {
    FILE *fp;
    int c;
    Node *head = NULL;
    Node *tail = NULL;
    Node *index_table[256];
    int i;

    for (i = 0; i < 256; i++) {
        index_table[i] = NULL;
    }

    fp = fopen(__FILE__, "r");
    if (fp == NULL) {
        return 1;
    }

    while ((c = fgetc(fp)) != EOF) {
        process_char(&head, &tail, index_table, c);
    }

    fclose(fp);

    // 輸出結果 [cite: 41]
    print_list(head);

    free_list(head);
    return 0;
}
