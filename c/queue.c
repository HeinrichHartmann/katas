#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

typedef struct queue Q;
typedef struct queue_node QN;

/* Interface declarations */
Q* q_new(void);
void q_free(Q* q);
int q_len(Q *q);
void q_enqueue(Q *q, int val);
int q_dequeu(Q *q);
int q_peek(Q *q);

/* Data Structures */

/*
 * Q
 *  - head -- points to head of linked list holding values
 *  - tail -- points to tail of linked list
 *
 *    head                           tail
 *     v                              v
 *    qn_1 -> qb_2 -> qn_3 -> ... -> qn_4
 *
 *    We enqueue at the tail
 *    We deque at the head
 *
*/

struct queue {
  QN* head;
  QN* tail;
  int len;
};

struct queue_node {
  QN* next;
  int val;
};

/* Implementations */

Q* q_new(void) {
  Q* q = calloc(sizeof(Q), 1);
  q->head = NULL;
  q->tail = NULL;
  q->len = 0;
  return q;
}

void q_free(Q* q) {
  QN *qn = q->head;
  while(qn) {
    QN *tmp = qn;
    qn = qn->next;
    free(tmp);
  }
  free(q);
}

int q_len(Q* q) {
  return q->len;
}

int q_peek(Q* q) {
  assert(q_len(q) > 0);
  return q->head->val;
}

void q_enqueue(Q* q, int val) {
  /* Dequeue at tail of the list */
  QN* qn = calloc(sizeof(QN), 1);
  qn->val = val;
  if(q_len(q) == 0) {
    q->tail = qn;
    q->head = qn;
  }
  else {
    assert(q->tail != NULL);
    assert(q->head != NULL);
    q->tail->next = qn;
    q->tail = qn;
  }
  q->len += 1;
}

int q_dequeue(Q* q) {
  /* Dequeue at head of the list */
  assert(q_len(q) > 0);
  QN *qn = q->head;
  int out = qn->val;
  q->head = qn->next;
  q->len -= 1;
  free(qn);
  return out;
}

void q_print(Q* q) {
  printf("Q[ ");
  QN *qn = q->head;
  while(qn) {
    printf("%d ", qn->val);
    qn = qn->next;
  }
  printf("]\n");
}

/*****************************************************************************
 * TESTS
 *****************************************************************************/

int test_new_q(void) {
  Q* q = q_new();
  if (q == NULL) {
    return 1;
  }
  q_free(q);

  q = q_new();
  for(int i=0; i<100; i++) {
    q_enqueue(q, i);
  }
  q_print(q);
  q_free(q);
  return 0;
}

int test_queue(void) {
  Q* q = q_new();

  q_enqueue(q, 1);
  assert(q_len(q) == 1);
  assert(q_peek(q) == 1);
  assert(q_dequeue(q) == 1);
  assert(q_len(q) == 0);
  q_free(q);

  q = q_new();
  for(int i=0; i<100; i++) {
    q_enqueue(q, i);
  }
  for(int i=0; i<100; i++) {
    assert(q_dequeue(q) == i);
  }
  q_free(q);
  return 0;
}


void run_test(char* name, int (*test)(void)) {
  printf("Test %s ... ", name);
  if (test() == 0) {
    printf("OK\n");
  } else {
    printf("FAILED\n");
    exit(1);
  }
}

int main(void) {
  run_test("new_q", test_new_q);
  run_test("en-de-q", test_queue);
  return 0;
}
