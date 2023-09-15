# Contacts
This CLI based app is an implementation of a contact book using a trie data structure and linked lists.

The Trie data structure in this code is defined as a structure that has a character "letra" to store the current character of the word, a pointer to a "Contacto" structure to store the contact information when the trie node represents a complete word, and a pointer to a linked list of child Trie nodes representing the next characters in the word.

In this code, the linked list is used to represent the children of a node in the trie data structure. Each node in the trie can have multiple children, with each child node representing the next character in a string.

The linked list is implemented using the _nodoT struct, which has two fields: a pointer to the next node in the linked list (sig), and a pointer to the child node in the trie (trie).

The menu provides several options for interacting with the contact manager:

"Agregar un contacto" - allows the user to add a new contact to the trie. The user is prompted to enter the contact's name, phone number, and email address, which are then stored in the trie.

"Ver todos los contactos" - allows the user to view all contacts currently stored in the trie. The program prints out the name, phone number, and email address for each contact.

"Importar Agenda" - allows the user to import a list of contacts from a file. The program reads in the file and adds the contacts to the trie.

"Eliminar Contacto" - allows the user to delete a contact from the trie. The user is prompted to enter the name of the contact to be deleted.

"imprimir estructura Trie" - allows the user to print out the entire trie data structure, including all nodes and their children.

"Buscar contacto" - allows the user to search for a contact in the trie. The user is prompted to enter the name of the contact to be searched for.

"Salir y guardar agenda" - allows the user to exit the program and save the current state of the trie to a file.
