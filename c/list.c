#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

typedef struct node node;

/*
 *
 * Linked List
 *
 */
struct node {
  node *next;
  int val;
};

node *node_new(int val) {
  node *n = calloc(1, sizeof(node));
  n->val = val;
  return n;
}

void node_free(node *n){
  if (n == NULL) { return; }
  node *nxt = n->next;
  free(n);
  if(nxt) {
    node_free(nxt);
  }
}

node* node_append(node *n, int val) {
  assert(n);
  while(n->next) {
    n = n->next;
  }
  assert(n);
  assert(n->next == NULL);
  n->next = node_new(val);
  return n->next;
}

/*
 * Delete first node with given value in the list (if any).
 *
 * Returns head of list with removed node.
 *
 */
node* node_delete(node *n, int val) {
  node *head = n;
  node *prev = NULL;
  while ((n != NULL) && (n->val != val)) {
    prev = n;
    n = n->next;
  }
  if (n == NULL) {
    // no node with given value found
    return head;
  }
  assert(n->val == val);
  node *o = n->next;
  // node_free(n);
  if (prev == NULL) {
    return o;
  }
  prev->next = o;
  return head;
}

int node_sum(node *n) {
  int acc = 0;
  while(n) {
    acc += (int) n->val;
    n = n->next;
  }
  return acc;
}

node* node_insert_ordered(node *n, int val) {
  assert(n);
  node *head = n;
  node *m = node_new(val);
  if (n->val >= val) {
    /* Insert value as new head of the list */
    head = m;
    head->next = n;
  }
  while ((n->next != NULL) && (n->next->val < val)) {
    n = n->next;
  }
  /* insert value between n and n->next */
  m->next = n->next;
  n->next = m;
  return head;
}

node* __reverse_rec(node *prev, node *head) {
  // Assume:
  // - prev, head are adjacent nodes in the original list
  //   both may be NULL
  // - Nodes up to prev have already been reversed
  // - Node head is to be reversed
  //
  if(head == NULL) { return prev; }
  node *next = head->next;
  head->next = prev;
  __attribute((musttail))
    return __reverse_rec(head,next);
}

node* node_reverse(node *head) {
  return __reverse_rec(NULL, head);
}

node* node_reverse_2(node *head) {
  node *next;
  node *prev;
  prev = NULL;
  while (head != NULL) {
    next = head->next;
    // ... <- p    h -> n -> ...
    head->next = prev;
    // ... <- p <- h    n -> ...
    prev = head;
    head = next;
  }
  return prev;
}


node* node_reverse_1(node *h) {
  if (h == NULL) {
    /* -># */
    return h;
  }
  if (h->next == NULL) {
    /* ->0-># */
    return h;
  }
  if (h->next->next == NULL) {
    /* ->0->1-># */
    node *a = h;
    node *b = h->next;
    b->next = a;
    a->next = NULL;
    return b;
  }
  // setup
  node *p = h;
  node *c = h->next;
  node *n = h->next->next;
  h->next = NULL; // terminate first node
  while (1) {
    /*
     * # <- 0 <- 1 <- 2    3 -> 4 -> 5 -> 6
     *                p    c    n
     */
    // reverse
    c->next = p;
    // done?
    if(n == NULL) { return c; }
    // advance
    p = c;
    c = n;
    n = n->next;
  }
}


void node_print(node n) {
  node h = n;
  while (1) {
    printf("[%d].", (int) h.val);
    if(h.next == NULL) {
      printf("#\n");
      break;
    }
    h = *h.next;
  }
}

int test_new_free(void) {
  node *n = node_new(0);
  assert(n);
  n->val = 1;
  node_free(n);
  return 0;
}

int test_append(void) {
  node *n = node_new(0);
  for(int i=1; i<10; i++){
    node_append(n, i);
  }
  node_print(*n);
  assert(node_sum(n) == 45);
  node_free(n);
  return 0;
}

int test_insert(void) {
  node *n = node_new(0);
  for(int i=1; i<10; i++){
    node_append(n, i * 10);
  }
  assert(node_sum(n) == 450);
  n = node_insert_ordered(n, 55);
  n = node_insert_ordered(n, -1);
  n = node_insert_ordered(n, 1000);
  node_print(*n);
  node_free(n);
  return 0;
}

int test_reverse(void) {
  node *h, *n;
  node_reverse(NULL);

  h = node_new(0);
  n = h;
  for(int i=1; i<100000; i++){
    n = node_append(n, i * 10);
  }
  h = node_reverse(h);
  node_free(h);

  h = node_new(0);
  for(int i=1; i<10; i++){
    h = node_reverse(h);
    node_append(h, i * 10);
  }
  node_print(*h);
  return 0;
}


int test_delete(void) {
  node *n = node_new(0);
  for(int i=1; i<10; i++){
    node_append(n, i);
  }
  n = node_delete(n, 0);
  n = node_delete(n, 5);
  n = node_delete(n, 9);
  n = node_delete(n, 3);
  node_print(*n);
  node_free(n);
  return 0;
}


int run_test(char* name, int (*f)(void)) {
  printf("%s ... ", name);
  int res = (*f)();
  if(res == 0) {
    printf("OK\n");
    return 0;
  }
  printf("FAILED!\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  (void) argc;
  (void) argv;
  run_test("new-free", test_new_free);
  run_test("append", test_append);
  run_test("insert", test_insert);
  run_test("reverse", test_reverse);
  run_test("delete", test_delete);
  return 0;
}
