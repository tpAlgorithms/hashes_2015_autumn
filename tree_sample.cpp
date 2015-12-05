#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stack>

typedef std::string data_t;
 
struct node_t {
  node_t(const data_t& _data)
    :data(_data)
    ,left(NULL)
    ,right(NULL)
  {}

  data_t data;
  node_t *left;
  node_t *right;
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


void action(node_t *root) {
  std::cout << "NODE:" << root->data << std::endl;
}
void inorder_traverse(node_t *root) {
  std::cout << ".";
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

int main() {
  char *buff[1024];
  
  node_t *root = NULL;
  while(std::cin.good()) {
    std::string line;
    int value = std::getline(std::cin, line);
    if (value == 0) break; 
    //std::cout << "[" << line << "]{" << value << "}\n";

    root = tree_add(root, new node_t(line));
  }

  //inorder_traverse(root);
  //iterate_tree(root);
  std::cout << tree_height(root) << std::endl;

  return 0;
}


















