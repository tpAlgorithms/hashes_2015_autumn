#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stack>

typedef std::string data_t;
 
struct node_t {
  node_t(const data_t& _data, int _priority = 0)
    :data(_data)
    ,priority(_priority)
    ,left(NULL)
    ,right(NULL)
    ,count(1) 
  {}

  data_t data;
  int priority;
  node_t *left;
  node_t *right;
  size_t count;
  //node_t *parent;
};

node_t *tree_add(node_t *root, node_t *new_node) {
  if (root == NULL) return new_node;
 
  node_t *curr = root; 
  while(1) {
    node_t **next = &curr->left;
    
    if (curr->data < new_node->data) {
      next = &curr->right;
    }

    if (*next == NULL) {
      //std::cout << "+" << std::endl;
      *next = new_node;
      return root;
    } else {
      //std::cout <<"VALUE["<< curr->data << "] ->" << ((*next == curr->left) ? "L" : "R") << std::endl;
      curr = *next;
    }
  }
}

size_t calc_tree_size(node_t *root) {
  if (root == NULL) { return 0; }
  return root->count;
}
void split(node_t *root, const data_t &data, node_t **left, node_t **right) {
  std::cout << "SPLIT: (" << data << ") cnt:: " << calc_tree_size(root)  << "" << std::endl;
  if (root == NULL) {
    *left = NULL;
    *right = NULL;
    return;
  }
  if (root->data <= data) {
    *left = root;
    split(root->right, data, &root->right, right);
    (*left)->count = 1 + calc_tree_size((*left)->left) + calc_tree_size((*left)->right);
  } else {
    *right = root;
    split(root->left, data, left, &root->left);
    (*right)->count = 1 + calc_tree_size((*right)->left) + calc_tree_size((*right)->right);
  }
}

node_t *tree_add_rnd(node_t *root, node_t *new_node) {
  std::cout << "tree_add_rnd " << new_node->data << std::endl;
  if (root == NULL) return new_node; 

  if ( rand() % (calc_tree_size(root) + 1) == 0) {
    split(root, new_node->data, &new_node->left, &new_node->right);
    new_node->count = 1 + calc_tree_size(new_node->left), calc_tree_size(new_node->right);
    return new_node;
  } else {
    if (new_node->data <= root->data) {
      root->left = tree_add_rnd(root->left, new_node);
    } else {
      root->right = tree_add_rnd(root->right, new_node);
    }
    return root;
  }
}


void action(node_t *root) {
  std::cout << "NODE:" << root->data << std::endl;
}
void inorder_traverse(node_t *root) {
  //std::cout << ".";
  if (root == NULL) return;
  inorder_traverse(root->left);
  action(root);
  inorder_traverse(root->right);
}

struct tree_it_t {
  tree_it_t() {}
  tree_it_t(node_t *_ptr) {
    history.push(_ptr);
  }

  node_t *ptr() const { 
    if (history.empty()) return NULL; 
    return history.top() ;
  }

  std::stack<node_t*> history;
};

bool operator == (const tree_it_t &lhs, const tree_it_t &rhs) {
  return lhs.ptr() == rhs.ptr();
}
bool operator != (const tree_it_t &lhs, const tree_it_t &rhs) {
  return !operator==(lhs, rhs);
}

tree_it_t tree_begin(node_t *root) {
  tree_it_t it;
  while(root->left != NULL) {
    it.history.push(root); 
    root = root->left;
  }
  it.history.push(root); 
  return it;
}

tree_it_t tree_end() {
  return tree_it_t();
}

tree_it_t tree_next(const tree_it_t &it) {
  if (it.ptr()->right != NULL) {
    std::cout << "IT-> " << it.ptr()->data << "\n";
    //return tree_begin(it.ptr->right);
    tree_it_t new_it = it;
    node_t *curr = it.ptr()->right;
    //new_it.ptr = curr;
    //new_it.history.push(curr);
    std::cout << "RIGHT-> " << curr->data << "\n";
    
    while(curr->left != NULL) {
      std::cout << "LEFT-> " << curr->data << "\n";
      new_it.history.push(curr); 
      curr = curr->left;
    }
    new_it.history.push(curr); 
    return new_it;
  } else {
    tree_it_t new_it = it;
    while(1) { //!it.history.empty()) {
      std::cout << "IT-> " << new_it.history.top()->data << "\tSTACK_SIZE:" << new_it.history.size() << "\n";
      node_t *curr = new_it.ptr();
      new_it.history.pop();
      if (new_it.history.empty()) break;
         
      if (new_it.history.top()->left == curr) {
        std::cout << "EXIT-> " << new_it.history.top()->data << "\n";
        return new_it;
      }
    }
    return tree_it_t();
  }
}

void iterate_tree(node_t *root) {
  for (tree_it_t it = tree_begin(root); it != tree_end(); it = tree_next(it)) {
    action(it.history.top());
  }
}

size_t tree_height(node_t *root) {
  if (root == NULL) return 0;
  return 1 + std::max(tree_height(root->left), tree_height(root->right));
}


void treep_split(node_t *root, const data_t &key, node_t **left, node_t **right) {
  if (!root) {
    *left = NULL;
    *right = NULL;
    return;
  }
 
  if (root->data <= key) {
    *left = root;
    treep_split(root->right, key, &(*left)->right, right); 
  } else {
    *right = root;
    treep_split(root->left, key, left, &(*right)->left);
  }
}

void treep_split_fast(node_t *root, const data_t &key, node_t **left, node_t **right) {
  while (root != NULL) {

    if (root->data <= key) {
      *left = root;
      left = &root->right;
      root = *left;
    } else {
      *right = root;
      right = &root->left;
      root = *right;
    }
  }
  *left = NULL;
  *right = NULL;
}

node_t *treep_merge(node_t *left, node_t *right) {
  if (!left) return right;
  if (!right) return left;
  if (left->priority >= right->priority) {
    left->right = treep_merge(left->right, right);
    return left;
  } else {
    right->left = treep_merge(left, right->left);
    return right;
  }
}

node_t *treep_add(node_t *root, node_t *new_node) {
  node_t *left;
  node_t *right;
  treep_split_fast(root, new_node->data, &left, &right);
  return treep_merge(left, treep_merge(new_node, right));
}


int main() {
  //std::cout  << "PID: " << getpid() << std::endl; 
  srand(getpid());
  char *buff[1024];
   
  node_t *root = NULL;
  while(std::cin.good()) {
    std::string line;
    int value = std::getline(std::cin, line);
    if (value == 0) break; 

    //root = tree_add(root, new node_t(line));
    //root = tree_add_rnd(root, new node_t(line));
    root = treep_add(root, new node_t(line, rand()));

    std::cout << "[" << line << "]{" << value << "} count: {" << calc_tree_size(root) <<"}\n";
  }

  inorder_traverse(root);
  //iterate_tree(root);
  std::cout << tree_height(root) << std::endl;

  return 0;
}


















