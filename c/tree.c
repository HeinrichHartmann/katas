#include "stdio.h"
#include "stdlib.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct node node;

struct node {
  int val;
  node *left;
  node *right;
};

node* new_node(int val)
{
  node* n = malloc(sizeof(node));
  n->val = val;
  return n;
}

void free_node(node *n) {
  free(n);
}

void _print_node(node *n, int indent) {
  if (!n) {
    return;
  }
  for (int i = indent; i>0; i--) {
    printf("-");
  };
  printf("Node[%d, %p, %p]\n", n->val, (void*)n->left, (void*)n->right);
  if (n->left) {
    _print_node(n->left, indent+1);
  }
  if (n->right) {
    _print_node(n->right,indent+1);
  }
}

void print_node(node *n) {
  _print_node(n, 0);
}

node* tree_from_list(int *list, int len, int idx) {
  if (!(idx < len)) { return NULL; }
  node *n = new_node(list[idx]);
  if (len < 2*(idx + 1)) { return n; }
  node *a = tree_from_list(list, len, 2*idx+1);
  node *b = tree_from_list(list, len, 2*(idx+1));
  n->left = a;
  n->right = b;
  return n;
}

/* Depth first search in tree for node with given value.
 * Returns nude with given value;
 */
node* dfs(node *n, int val)
{
  if (n->val == val) {
    return n;
  }
  if (n->left) {
    node *l = dfs(n->left, val);
    if (l) {
      return l;
    }
  }
  if (n->right) {
    node *r = dfs(n->right, val);
    if (r) {
      return r;
    }
  }
  return NULL;
}

int depth(node *n)
{
  if (n==NULL) { return 0; }
  return 1 + MAX(depth(n->left), depth(n->right));
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

int main(void)
{
    printf("Hello world!\n");
    node *n = new_node(5);
    print_node(n);


    int list[] = { 1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,2,2,2,1,1,2,5 };
    int llist = sizeof(list)/sizeof(int);
    n = tree_from_list(list,  llist, 0);
    print_node(n);
    printf("Depth: %d\n", depth(n));

    node *m = dfs(n, 5);
    print_node(m);

    node *m2 = dfs(n, 3);
    print_node(m2);

    printf("Length of path with val: %d\n", length_path_with_given_val(n,n->val));
}
