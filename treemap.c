#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode * new = createTreeNode(key, value);
  if(tree->root == NULL){
    tree->root = new;
    tree->current = new;
    return;
  }
  TreeNode * aux = tree->root;
  while(aux != NULL){
    if(is_equal(tree, aux->pair->key, key)){
      aux->pair->value = value;
      return;
    }
    if(tree->lower_than(aux->pair->key, key)){
      if(aux->right == NULL){
        aux->right = new;
        new->parent = aux;
        tree->current = new;
        return;
      }
      aux = aux->right;
      
    }else{
      if(aux->left == NULL){
        aux->left = new;
        new->parent = aux;
        tree->current = new;
        return;
      }
      aux = aux->left;
    }
  }
}

TreeNode * minimum(TreeNode * x){
    while(x->left!=NULL){
        x=x->left;
    }
    return x;
}

void removeNode(TreeMap * tree, TreeNode* node) {
  if(node->left == NULL && node->right == NULL){
    if(node->parent == NULL){
      tree->root = NULL;
      return;
    }
    if(node->parent->left == node){
      node->parent->left = NULL;
    }
    else{
      node->parent->right = NULL;
    }
    tree->current = node->parent;
    free(node);
    return;
  }
  if(node->left == NULL || node->right == NULL){
    if(node->parent == NULL){
      if(node->left == NULL){
        tree->root = node->right;
        tree->root->parent = NULL;
      }
      else{
        tree->root = node->left;
        tree->root->parent = NULL;
      }
      tree->current = tree->root;
      free(node);
      return;
    }
    if(node->parent->left == node){
      if(node->left == NULL){
        node->parent->left = node->right;
        node->right->parent = node->parent;
      }
      else{
        node->parent->left = node->left;
        node->left->parent = node->parent;
      }
    }
    else{
      if(node->left == NULL){
        node->parent->right = node->right;
        node->right->parent = node->parent;
      }
      else{
        node->parent->right = node->left;
        node->left->parent = node->parent;
      }
    }
    tree->current = node->parent;
    free(node);
    return;
  }
  TreeNode * aux = minimum(node->right);
  node->pair->key = aux->pair->key;
  node->pair->value = aux->pair->value;
  removeNode(tree, aux);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  if (tree == NULL || tree->root == NULL) return NULL;  
  TreeNode *aux = tree->root;
  while(aux!=NULL){
    if(is_equal(tree,aux->pair->key,key)){
      tree->current=aux;
      return aux->pair;
    }
    if(tree->lower_than(aux->pair->key,key)){
      aux=aux->right;
    }else{
      aux=aux->left;
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  if (tree == NULL || tree->root == NULL) return NULL;
  TreeNode *aux = tree->root;
  TreeNode *ub = NULL;
  
  while(aux!=NULL){
    if(is_equal(tree,key,aux->pair->key)){
      tree->current= aux;
      return aux->pair;
    }
    if(tree->lower_than(key,aux->pair->key)){
      ub = aux;
      aux= aux->left;
    }
    else
    {
      aux=aux->right;
    }
  }
  if(ub !=NULL){
    return ub->pair;
  }
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *aux = tree->root;
    while(aux->left!=NULL){
        aux=aux->left;
    }
    tree->current=aux;
    return aux->pair;
    
}

// Pair* nextTreeMap(TreeMap* tree) retornar el siguiente Pair del mapa a partir del puntero TreeNode* current. Recuerde actualizar este puntero.
Pair * nextTreeMap(TreeMap * tree) {
  if(tree->current == NULL) return NULL;
  TreeNode *aux = tree->current;
  if(aux->right != NULL){
    tree->current = minimum(aux->right);
    return tree->current->pair;
  }
  TreeNode *aux2 = aux->parent;
  while(aux2 != NULL && aux == aux2->right){
    aux = aux2;
    aux2 = aux2->parent;
  }
  tree->current = aux2;
  if(aux2 !=NULL) return aux2->pair;
  return NULL;
}
