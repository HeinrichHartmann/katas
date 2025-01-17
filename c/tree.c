#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

/*
 * Stack implementation backed by Linked List
 */

typedef struct stack stack;
typedef struct stack_node stack_node;
struct stack_node {
  void *val;
  stack_node *next;
};

struct stack {
  stack_node *head;
};

stack *stack_new(void) {
  stack *s = malloc(sizeof(stack));
  s->head = NULL;
  return s;
}

void stack_free(stack *s) {
  if (s == NULL) { return; }
  stack_node *n = s->head;
  while (n) {
    stack_node *next = n->next;
    free(n);
    n = next;
  }
  free(s);
}

void stack_push(stack *s, void *val) {
  stack_node *n = malloc(sizeof(stack_node));
  n->val = val;
  n->next = s->head;
  s->head = n;
}

void* stack_pop(stack *s) {
  if (s->head == NULL) {
    return NULL;
  }
  stack_node *n = s->head;
  s->head = s->head->next;
  void *out = n->val;
  free(n);
  return out;
}

int stack_empty(stack *s) {
  assert(s);
  return s->head == NULL;
}

/*
 * Simple Queue Structure backed by Linked List
 *
 * Q:
 *  - back  -- we pop elements here
 *  - front -- we insert elements here
 *
 *  back -> [1] -> [2] -> [3] <- front
 *
 */
typedef struct queue queue;
typedef struct queue_node queue_node;

struct queue_node {
  void *val;
  queue_node *next;
};

struct queue {
  void *val;
  queue_node *front;
  queue_node *back;
};

queue* queue_new(void)
{
  return calloc(sizeof(queue), 1);
}

void queu_free(queue *q)
{
  if (q == NULL) { return; }
  queue_node *n = q->back;
  while (n) {
    queue_node *m = n->next;
    free(n);
    n = m;
  }
  free(q);
}

void queue_push(queue *q, void *val)
{
  assert(q);
  queue_node *n = calloc(sizeof(queue_node), 1);
  n->val = val;
  if (q->front) {
    q->front->next = n;
  }
  q->front = n;
  if (q->back == NULL) {
    q->back = n;
  }
}

void* queue_pop(queue *q)
{
  assert(q);
  if (q->back == NULL) {
    return NULL;
  }
  queue_node *n = q->back;
  q->back = q->back->next;
  void *out = n->val;
  free(n);
  if(q->back == NULL) {
    q->front = NULL;
  }
  return out;
}

int queue_empty(queue *q)
{
  assert(q);
  return q->back == NULL;
}


//
// Tree Datastructure
//
typedef struct node node;

struct node {
  int val;
  node *left;
  node *right;
};

node* tree_new(int val)
{
  node* n = calloc(sizeof(node), 1);
  n->val = val;
  return n;
}

void tree_free(node *n) {
  if (n == NULL) { return; }
  tree_free(n->left);
  tree_free(n->right);
  free(n);
}

node* tree_from_list(int *list, int len, int idx) {
  if (!(idx < len)) { return NULL; }
  node *n = tree_new(list[idx]);

  if (len < 2*(idx + 1)) { return n; }
  node *a = tree_from_list(list, len, 2*idx+1);
  node *b = tree_from_list(list, len, 2*(idx+1));
  n->left = a;
  n->right = b;
  return n;
}

node *tree_insert(node *root, int val) {
  if (root == NULL) {
    return tree_new(val);
  }

  if( val < root->val ) {
    root->left = tree_insert(root->left, val);
  }
  else if( root->val <= val ) {
    root->right = tree_insert(root->right, val);
  }
  return root;
}

node *tree_insert_array(node *root, int *vals, int len) {
  for (int i=0; i<len; i++) {
    root = tree_insert(root, vals[i]);
  }
  return root;
}

void node_print(node *n, char *postfix) {
  postfix = postfix ? postfix : "";
  char *l = n->left ? "<" : ".";
  char *r = n->right ? ">" : ".";
  printf("%s[%d]%s%s", l, n->val, r, postfix);
}

void _tree_print(node *n, int indent) {
  if (n == NULL) { return; }
  for (int i = indent; i>0; i--) {
    printf("-");
  };
  node_print(n, "\n");
  if (n->left) { _tree_print(n->left, indent+1); }
  if (n->right) { _tree_print(n->right,indent+1); }
}

void tree_print(node *root) {
  _tree_print(root, 0);
}


/* Depth first search in tree for node with given value.
 * Returns nude with given value;
 */
node* tree_dfs(node *n, int val)
{
  if (n->val == val) {
    return n;
  }
  if (n->left) {
    node *l = tree_dfs(n->left, val);
    if (l) {
      return l;
    }
  }
  if (n->right) {
    node *r = tree_dfs(n->right, val);
    if (r) {
      return r;
    }
  }
  return NULL;
}

int tree_depth(node *n)
{
  if (n==NULL) { return 0; }
  return 1 + MAX(tree_depth(n->left), tree_depth(n->right));
}

void tree_walk_dfs(node *n, void (*visit)(node* n, void* closure), void *closure)
{
  if (n == NULL) { return; }
  visit(n, closure);
  tree_walk_dfs(n->left, visit, closure);
  tree_walk_dfs(n->right, visit, closure);
}

void tree_walk_bfs(node *n, void (*visit)(node* n, void* closure), void *closure)
{
  queue *q = queue_new();
  queue_push(q, n);
  while (!queue_empty(q)) {
    node *n = queue_pop(q);
    visit(n, closure);
    if (n->left) { queue_push(q, n->left); }
    if (n->right) { queue_push(q, n->right); }
  }
}

/*
 * Returns length of the longest path of nodes with the given value as the given node in the tree
*/
int length_path_with_given_val(node *n, int val)
{
  if (!n) { return 0; }
  if (n-> val != val) { return 0; }
  return 1
    + length_path_with_given_val(n->left, val)
    + length_path_with_given_val(n->right, val);
}



//
// Test cases
//

// Test framework
typedef struct {
    const char* name;
    int (*test_fn)(void);
} test_case;

#define ASSERT(condition, message) do { \
    if (!(condition)) { \
        printf("FAIL: %s\n", message); \
        return 0; \
    } \
    return 1; \
} while (0)

// Test helpers for tree walks
#define MAX_WALK_NODES 100

typedef struct {
    int values[MAX_WALK_NODES];
    int count;
} walk_collector;

void walk_collector_visit(node *n, void *cls) {
  walk_collector *c = (walk_collector*) cls;
  if (c->count < MAX_WALK_NODES) {
    c->values[c->count++] = n->val;
  }
}

int arrays_equal(int *a, int *b, int len) {
    return memcmp(a, b, len * sizeof(int)) == 0;
}

// Test implementation
int test_create_tree(void) {
    node *n = tree_new(5);
    int result = (n != NULL && n->val == 5 && n->left == NULL && n->right == NULL);
    tree_free(n);
    ASSERT(result, "tree_new should create a valid node");
}

int test_tree_from_list(void) {
    int list[] = {1, 2, 3, 4, 5};
    node *n = tree_from_list(list, 5, 0);
    int depth = tree_depth(n);
    int result = (depth == 3 && n->val == 1);
    tree_free(n);
    ASSERT(result, "tree_from_list should create correct tree structure");
}

int test_tree_dfs(void) {
    int list[] = {1, 2, 3, 4, 5};
    node *n = tree_from_list(list, 5, 0);
    node *found = tree_dfs(n, 3);
    int result = (found != NULL && found->val == 3);
    tree_free(n);
    ASSERT(result, "tree_dfs should find existing value");
}

int test_tree_insert(void) {
    node *n = tree_new(5);
    n = tree_insert(n, 3);
    n = tree_insert(n, 7);
    int result = (n->val == 5 && 
                 n->left->val == 3 && 
                 n->right->val == 7);
    tree_free(n);
    ASSERT(result, "tree_insert should maintain BST properties");
}

int test_length_path(void) {
    int list[] = {1, 1, 1, 3, 1, 1};
    node *n = tree_from_list(list, 6, 0);
    int length = length_path_with_given_val(n, 1);
    int result = (length > 0);
    tree_free(n);
    ASSERT(result, "length_path_with_given_val should find paths");
}

int test_queue_new(void) {
    queue *q = queue_new();
    int result = (q != NULL && q->front == NULL && q->back == NULL);
    queu_free(q);
    ASSERT(result, "queue_new should create empty queue");
}

int test_queue_push_pop(void) {
    queue *q = queue_new();
    int val1 = 42;
    int val2 = 43;
    
    queue_push(q, &val1);
    queue_push(q, &val2);
    
    int *pop1 = queue_pop(q);
    int *pop2 = queue_pop(q);
    int *pop3 = queue_pop(q);
    
    int result = (*pop1 == 42 && *pop2 == 43 && pop3 == NULL);
    queu_free(q);
    ASSERT(result, "queue push/pop should maintain FIFO order");
}

int test_queue_empty(void) {
    queue *q = queue_new();
    int val = 42;
    
    int empty1 = queue_empty(q);
    queue_push(q, &val);
    int empty2 = queue_empty(q);
    queue_pop(q);
    int empty3 = queue_empty(q);
    
    int result = (empty1 && !empty2 && empty3);
    queu_free(q);
    ASSERT(result, "queue_empty should correctly report queue state");
}

int test_tree_walk_dfs(void) {
    // Create a simple tree:
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    node *root = tree_new(1);
    root->left = tree_new(2);
    root->right = tree_new(3);
    root->left->left = tree_new(4);
    root->left->right = tree_new(5);

    walk_collector collector = {0};
    tree_walk_dfs(root, walk_collector_visit, &collector);

    // Expected pre-order traversal: 1,2,4,5,3
    int expected[] = {1, 2, 4, 5, 3};
    int result = (collector.count == 5 && 
                 arrays_equal(collector.values, expected, 5));
    
    tree_free(root);
    ASSERT(result, "tree_walk_dfs should traverse in pre-order");
}

int test_tree_walk_bfs(void) {
    // Create the same tree:
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    node *root = tree_new(1);
    root->left = tree_new(2);
    root->right = tree_new(3);
    root->left->left = tree_new(4);
    root->left->right = tree_new(5);

    walk_collector collector = {0};
    tree_walk_bfs(root, walk_collector_visit, &collector);

    // Expected pre-bfs traversal: 1,2,4,5,3
    int expected[] = {1, 2, 3, 4, 5};
    int result = (collector.count == 5 && 
                 arrays_equal(collector.values, expected, 5));
    
    tree_free(root);
    ASSERT(result, "tree_walk_bfs should traverse in level-order");
}

// Stack test cases
int test_stack_new(void) {
    stack *s = stack_new();
    int result = (s != NULL && s->head == NULL);
    stack_free(s);
    ASSERT(result, "stack_new should create empty stack");
}

int test_stack_push_pop(void) {
    stack *s = stack_new();
    int val1 = 42;
    int val2 = 43;
    
    stack_push(s, &val1);
    stack_push(s, &val2);
    
    int *pop1 = stack_pop(s);
    int *pop2 = stack_pop(s);
    int *pop3 = stack_pop(s);
    
    // LIFO order: last in (43) should come out first
    int result = (*pop1 == 43 && *pop2 == 42 && pop3 == NULL);
    stack_free(s);
    ASSERT(result, "stack push/pop should maintain LIFO order");
}

int test_stack_empty(void) {
    stack *s = stack_new();
    int val = 42;
    
    int empty1 = stack_empty(s);
    stack_push(s, &val);
    int empty2 = stack_empty(s);
    stack_pop(s);
    int empty3 = stack_empty(s);
    
    int result = (empty1 && !empty2 && empty3);
    stack_free(s);
    ASSERT(result, "stack_empty should correctly report stack state");
}

//
// Main
//
int main(void) {
    test_case tests[] = {
        {"Stack New", test_stack_new},
        {"Stack Push/Pop", test_stack_push_pop},
        {"Stack Empty", test_stack_empty},
        {"Queue New", test_queue_new},
        {"Queue Push/Pop", test_queue_push_pop},
        {"Queue Empty", test_queue_empty},
        {"Create Tree", test_create_tree},
        {"Tree From List", test_tree_from_list},
        {"Tree DFS", test_tree_dfs},
        {"Tree Insert", test_tree_insert},
        {"Length Path", test_length_path},
        {"Tree Walk DFS", test_tree_walk_dfs},
        {"Tree Walk BFS", test_tree_walk_bfs}
    };

    int total_tests = sizeof(tests) / sizeof(test_case);
    int passed = 0;

    printf("Running %d tests:\n", total_tests);
    printf("================\n");

    for (int i = 0; i < total_tests; i++) {
        printf("Test: %s - ", tests[i].name);
        if (tests[i].test_fn()) {
            printf("PASS\n");
            passed++;
        }
    }

    printf("\nResults: %d/%d tests passed\n", passed, total_tests);
    return passed == total_tests ? 0 : 1;
}
