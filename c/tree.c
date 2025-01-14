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

node* tree_new(int val)
{
  node* n = calloc(sizeof(node), 1);
  n->val = val;
  return n;
}

void node_print(node *n, char *postfix) {
  postfix = postfix ? postfix : "";
  char *l = n->left ? "<" : ".";
  char *r = n->right ? ">" : ".";
  printf("%s[%d]%s%s", l, n->val, r, postfix);
}

void tree_free(node *n) {
  if (n == NULL) { return; }
  tree_free(n->left);
  tree_free(n->right);
  free(n);
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
    node *n = tree_new(5);
    tree_print(n);

    int list[] = { 1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,2,2,2,1,1,2,5 };
    int llist = sizeof(list)/sizeof(int);
    n = tree_from_list(list,  llist, 0);
    tree_print(n);
    printf("Depth: %d\n", depth(n));

    node *m = dfs(n, 5);
    node_print(m, "\n");

    node *m2 = dfs(n, 3);
    node_print(m2, "\n");

    printf("Length of path with val: %d\n", length_path_with_given_val(n,n->val));
    tree_free(n);

    printf("Tree insertion\n");
    n = tree_new(0);
    for (int i=0; i<100; i++) {
      n = tree_insert(n, i+1);
    }
    tree_print(n);
}
