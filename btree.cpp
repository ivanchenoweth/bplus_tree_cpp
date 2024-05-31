// Searching on a B+ tree in C++
// Based on : https://www.programiz.com/dsa/b-plus-tree


#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>


using namespace std;
int MAX = 3;

// BP node
class Node {
  bool IS_LEAF;
  int *key, size;
  Node **ptr;
  friend class BPTree;

   public:
  Node();
};

// BP tree
class BPTree {
  Node *root;
  void insertInternal(int, Node *, Node *);
  Node *findParent(Node *, Node *);

   public:
  BPTree();
  std::string  search(int);
  void insert(int);
  void display(Node *);
  Node *getRoot();
};

Node::Node() {
  key = new int[MAX];
  ptr = new Node *[MAX + 1];
}

BPTree::BPTree() {
  root = NULL;
}

// Search operation
 // Search Operation
std::string BPTree::search(int x) {
  if (root == NULL) {
    // Si la raíz es nula, el árbol está vacío
    cout << "Tree is empty\n";
  } else {
    Node *cursor = root;
    while (cursor->IS_LEAF == false) {
      // Recorremos hacia abajo hasta llegar a una hoja
      for (int i = 0; i < cursor->size; i++) {
        if (x < cursor->key[i]) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    // Buscamos en la hoja actual
    for (int i = 0; i < cursor->size; i++) {
      if (cursor->key[i] == x) {
        return "Found"; // Clave encontrada
      }
    }
    return "Not Found"; // Clave no encontrada
  }
}

// Insert Operation
void BPTree::insert(int x) {
  if (root == NULL) {
    // Si la raíz es nula, creamos un nuevo nodo como raíz
    root = new Node;
    root->key[0] = x;
    root->IS_LEAF = true;
    root->size = 1;
  } else {
    Node *cursor = root;
    Node *parent;
    while (cursor->IS_LEAF == false) {
      // Recorremos hacia abajo hasta llegar a una hoja
      parent = cursor;
      for (int i = 0; i < cursor->size; i++) {
        if (x < cursor->key[i]) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    if (cursor->size < MAX) {
      // Si el nodo no está lleno, insertamos directamente en él
      int i = 0;
      while (x > cursor->key[i] && i < cursor->size)
        i++;
      for (int j = cursor->size; j > i; j--) {
        cursor->key[j] = cursor->key[j - 1];
      }
      cursor->key[i] = x;
      cursor->size++;
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      cursor->ptr[cursor->size - 1] = NULL;
    } else {
      // Si el nodo está lleno, creamos un nuevo nodo hoja
      // y redistribuimos las claves entre los nodos
      Node *newLeaf = new Node;
      int virtualNode[MAX + 1];
      for (int i = 0; i < MAX; i++) {
        virtualNode[i] = cursor->key[i];
      }
      int i = 0, j;
      while (x > virtualNode[i] && i < MAX)
        i++;
      for (int j = MAX + 1; j > i; j--) {
        virtualNode[j] = virtualNode[j - 1];
      }
      virtualNode[i] = x;
      newLeaf->IS_LEAF = true;
      cursor->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;
      cursor->ptr[cursor->size] = newLeaf;
      newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
      cursor->ptr[MAX] = NULL;
      for (i = 0; i < cursor->size; i++) {
        cursor->key[i] = virtualNode[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
        newLeaf->key[i] = virtualNode[j];
      }
      if (cursor == root) {
        Node *newRoot = new Node;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      } else {
        insertInternal(newLeaf->key[0], parent, newLeaf);
      }
    }
  }
}

// Insert Operation (Para los nodos internos en el un B+ tree)
void BPTree::insertInternal(int x, Node *cursor, Node *child) {
  if (cursor->size < MAX) {
    // Si el tamaño del nodo cursor es menor que el máximo permitido (MAX),
    // podemos insertar directamente en este nodo.
    int i = 0;
    while (x > cursor->key[i] && i < cursor->size)
      i++;
    for (int j = cursor->size; j > i; j--) {
      cursor->key[j] = cursor->key[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--) {
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    cursor->key[i] = x;
    cursor->size++;
    cursor->ptr[i + 1] = child;
  } else {
    // Si el nodo está lleno, creamos un nuevo nodo interno (newInternal)
    // y redistribuimos las claves y punteros entre los nodos.
    Node *newInternal = new Node;
    int virtualKey[MAX + 1];
    Node *virtualPtr[MAX + 2];
    for (int i = 0; i < MAX; i++) {
      virtualKey[i] = cursor->key[i];
    }
    for (int i = 0; i < MAX + 1; i++) {
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x > virtualKey[i] && i < MAX)
      i++;
    for (int j = MAX + 1; j > i; j--) {
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[i] = x;
    for (int j = MAX + 2; j > i + 1; j--) {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->IS_LEAF = false;
    cursor->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
      newInternal->key[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
      newInternal->ptr[i] = virtualPtr[j];
    }
    if (cursor == root) {
      // Si el cursor es la raíz, creamos un nuevo nodo raíz (newRoot)
      // y actualizamos los punteros.
      Node *newRoot = new Node;
      newRoot->key[0] = cursor->key[cursor->size];
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    } else {
      // Si no es la raíz, llamamos a la función insertInternal recursivamente para manejar
      // la inserción en nodos internos.
      insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
    }
  }
}

// Encontrando el Padre del Nodo 
Node *BPTree::findParent(Node *cursor, Node *child) {
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
    // Si el nodo cursor es una hoja o su primer puntero es una hoja,
    // no tiene un nodo padre (es la raíz o una hoja).
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++) {
    if (cursor->ptr[i] == child) {
      // Si encontramos que el puntero actual apunta al nodo hijo,
      // asignamos el nodo cursor como el nodo padre.
      parent = cursor;
      return parent;
    } else {
      // Si no es el nodo hijo actual, llamamos recursivamente a findParent
      // para buscar en los nodos descendientes.
      parent = findParent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  // Si no encontramos el nodo hijo en los punteros del nodo cursor,
  // devolvemos NULL (no se encontró un nodo padre).
  return parent;
}


// Imprimiendo el Arbol
void BPTree::display(Node *cursor) {
  if (cursor != NULL) {
    // Si el nodo cursor no es nulo, procedemos a mostrar su contenido
    for (int i = 0; i < cursor->size; i++) {
      cout << cursor->key[i] << " ";
      // Imprimimos cada clave en el nodo
    }
    cout << "\n"; // Agregamos un salto de línea después de imprimir las claves

    if (cursor->IS_LEAF != true) {
      // Si el nodo cursor no es una hoja (es un nodo interno),
      // continuamos mostrando los nodos descendientes
      for (int i = 0; i < cursor->size + 1; i++) {
        display(cursor->ptr[i]);
        // Llamamos recursivamente a la función display para mostrar
        // los nodos descendientes (hijos) del nodo actual
      }
    }
  }
}


// Get the root
Node *BPTree::getRoot() {
  return root;
}

int main() {

  // Implementación  
  BPTree node;
  node.insert(5);
  node.insert(15);
  node.insert(25);
  node.insert(35);
  node.insert(45);
  node.insert(55);
  node.insert(40);
  node.insert(30);
  node.insert(20);
  node.display(node.getRoot());

  cout << "Buscando 15\n";
  cout << node.search(15);
  cout << "\n";   cout << "\n";


  // Implementación con Pruebas unitarias
  BPTree tree;

  // Prueba 1: Verificar que el árbol esté vacío al inicio
  assert(tree.getRoot() == nullptr);

  // Prueba 2: Insertar un valor y verificar que se encuentre
  tree.insert(42);
  assert(tree.search(42) == "Found"); // Verifica que el valor 42 esté presente

  // Prueba 3: Insertar un valor y verificar que no se encuentre
  tree.insert(10);
  assert(tree.search(20) == "Not Found"); // Verifica que el valor 20 no esté presente

  // ... (agregar más pruebas según tus necesidades)

  std::cout << "Todas las pruebas pasaron correctamente." << std::endl;
  return 0;

}