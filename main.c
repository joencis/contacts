#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Agregar la biblioteca stdbool.h
#include <ctype.h>

typedef struct contacto {
    char nombre[64];
    char telefono[32];
    char correo[64];
} _contacto;

typedef _contacto* Contacto;

typedef struct nodoT {
    struct trie* trie;
    struct nodoT* sig;
} _nodoT;
typedef _nodoT* ListaT;

typedef struct trie {
    char letra;
    Contacto contacto;
    ListaT hijos;
} _trie;
typedef _trie* Trie;

void add_contact(Trie trie, char* nombre,char* original, char* telefono, char* correo);
void print_contacts(Trie trie);
void guardar_agenda(Trie trie); // Agregar el prototipo de la función guardar_agenda()
void importar_agenda(Trie trie);
void eliminar_contacto(Trie trie);
void imprimir_trie(Trie trie, int nivel);
void eliminar_nodos_vacios(Trie trie);
void search_contact(Trie trie);
void listarContactos(Trie t,char* c);
void BuscarContacto(Trie t);
Trie buscarTrie(ListaT t, char c);
void flushstdin(FILE *where);
char* strlwr(char* str);
int main() {
    Trie trie = malloc(sizeof(_trie));
    trie->letra = '\0';
    trie->contacto = NULL;
    trie->hijos = NULL;
    
    int opcion = 0;
    
    while (opcion != 7) {
        printf("\nSeleccione una opcion:\n");
        printf("1. Agregar un contacto\n");
        printf("2. Ver todos los contactos\n");
        printf("3. Importar Agenda\n");
        printf("4. Eliminar Contacto\n");
        printf("5. imprimir estructura Trie\n");
        printf("6. Buscar contacto\n");
        printf("7. Salir y guardar agenda\n"); 
        
        scanf("%d", &opcion);
        getchar(); // Leer el carácter de nueva línea del buffer 
        switch (opcion) {
            case 1: {
                char nombre[64];
                char telefono[32];
                char correo[64];
                
                printf("\nNombre: ");
                scanf("%s", nombre);
                
                printf("\nTelefono: ");
                scanf("%s", telefono);
                
                printf("\nCorreo: ");
                scanf("%s", correo);
                
                add_contact(trie, nombre, nombre, telefono, correo);
                break;
            }
            case 2: {
                print_contacts(trie);
                break;
            }
            case 3: {
                importar_agenda(trie); // Llamar a la función guardar_agenda() antes de salir del programa
                printf("\nAgenda Importada...\n");
                print_contacts(trie);
                
                break;
            }
            case 4: {
                eliminar_contacto(trie);
                //eliminar_nodos_vacios(trie);
                
                break;
            }
            case 5: {
                imprimir_trie(trie, 0);
                
                break;
            }
            case 6: {
                BuscarContacto(trie);
                
                break;
            }
            case 7: {
                guardar_agenda(trie); // Llamar a la función guardar_agenda() antes de salir del programa
                printf("\nSaliendo del programa...\n");
                break;
            }

            default: {
                printf("\nOpcion invalida. Por favor seleccione una opcion valida.\n");
                break;
            }
        }
    }
    
    return 0;
}

void add_contact(Trie trie, char* nombre,char* original, char* telefono, char* correo) {
    if (*nombre == '\0') {
        trie->contacto = malloc(sizeof(_contacto));
        strcpy(trie->contacto->nombre, original);
        strcpy(trie->contacto->telefono, telefono);
        strcpy(trie->contacto->correo, correo);
        return;
    }
    
    ListaT hijos = trie->hijos;
    ListaT anterior = NULL;
    while (hijos != NULL && hijos->trie->letra < *nombre) {
        anterior = hijos;
        hijos = hijos->sig;
    }
    
    if (hijos != NULL && hijos->trie->letra == *nombre) {
        add_contact(hijos->trie, nombre + 1, original, telefono, correo);
    } else {
        Trie nuevo_trie = malloc(sizeof(_trie));
        nuevo_trie->letra = *nombre;
        nuevo_trie->contacto = NULL;
        nuevo_trie->hijos = NULL;

        ListaT nuevo_nodo = malloc(sizeof(_nodoT));
        nuevo_nodo->trie = nuevo_trie;
        nuevo_nodo->sig = hijos;
        if (anterior == NULL) {
            trie->hijos = nuevo_nodo;
        } else {
            anterior->sig = nuevo_nodo;
        }
        
        add_contact(nuevo_trie, nombre + 1, original, telefono, correo);
    }
}

void print_contacts_helper(Trie trie, char* nombre_actual, int nivel) {
    if (trie->contacto != NULL) {
        printf("\n%s (%s, %s)", nombre_actual, trie->contacto->telefono, trie->contacto->correo);
    }
    
    ListaT hijos = trie->hijos;
    while (hijos != NULL) {
        nombre_actual[nivel] = hijos->trie->letra;
        nombre_actual[nivel+1] = '\0';
        
        print_contacts_helper(hijos->trie, nombre_actual, nivel + 1);
        
        hijos = hijos->sig;
    }
}

void print_contacts(Trie trie) {
    char nombre_actual[64];
    nombre_actual[0] = '\0';
    
    printf("\nContactos:");
    print_contacts_helper(trie, nombre_actual, 0);
}
void eliminar_contacto(Trie trie) {
    char nombre[64];
    printf("Ingrese el nombre del contacto a eliminar: ");
    fgets(nombre, 64, stdin);
    nombre[strcspn(nombre, "\n")] = 0; // Eliminar carácter de nueva línea

    // Buscar el nodo correspondiente al nombre completo del contacto
    Trie nodo_actual = trie;
    Trie padre_actual = trie;
    int i = 0;
    while (nombre[i] != '\0') {
        char letra_actual = tolower(nombre[i]);
        ListaT hijos = nodo_actual->hijos;
        ListaT anterior = NULL;
        while (hijos != NULL) {
            if (hijos->trie->letra == letra_actual) {
                padre_actual = nodo_actual;
                nodo_actual = hijos->trie;
                break;
            }
            anterior = hijos;
            
            hijos = hijos->sig;
        }
        if (hijos == NULL) {
            printf("\nContacto no encontrado.\n");
            return;
        }
        i++;
    }

    // Eliminar el contacto del nodo actual
    if (nodo_actual->contacto == NULL) {
        printf("\nContacto no encontrado.\n");
        return;
    }
    free(nodo_actual->contacto);
    nodo_actual->contacto = NULL;

    // Eliminar los nodos vacíos en el camino hacia la raíz
    eliminar_nodos_vacios(padre_actual);

    printf("\nContacto eliminado.\n");
}

void eliminar_nodos_vacios(Trie trie) {
    if (trie == NULL) {
        return;
    }
    ListaT hijos = trie->hijos;
    ListaT anterior = NULL;
    while (hijos != NULL) {
        eliminar_nodos_vacios(hijos->trie);
        if (hijos->trie->contacto == NULL && hijos->trie->hijos == NULL) {
            if (anterior == NULL) {
                trie->hijos = hijos->sig;
            } else {
                anterior->sig = hijos->sig;
            }
            free(hijos->trie);
            free(hijos);
            hijos = anterior == NULL ? trie->hijos : anterior->sig;
        } else {
            anterior = hijos;
            hijos = hijos->sig;
        }
    }
}
void guardar_agenda_helper(Trie trie, FILE* archivo, char* nombre_actual, int nivel) {
    if (trie->contacto != NULL) {
        fprintf(archivo, "%s,%s,%s\n", nombre_actual, trie->contacto->telefono, trie->contacto->correo);
    }
    
    ListaT hijos = trie->hijos;
    while (hijos != NULL) {
        nombre_actual[nivel] = hijos->trie->letra;
        nombre_actual[nivel+1] = '\0';
        
        guardar_agenda_helper(hijos->trie, archivo, nombre_actual, nivel + 1);
        
        hijos = hijos->sig;
    }
}

void guardar_agenda(Trie trie) {
    FILE* archivo = fopen("agenda.txt", "w"); // Abrir el archivo en modo de escritura
    if (archivo == NULL) {
        printf("\nError al abrir el archivo de agenda.\n");
        return;
    }
    
    char nombre_actual[64];
    nombre_actual[0] = '\0';
    
    guardar_agenda_helper(trie, archivo, nombre_actual, 0);
    
    fclose(archivo); // Cerrar el archivo
}
void importar_agenda(Trie trie) {
    FILE* archivo = fopen("agenda.txt", "r"); // Abrir el archivo en modo de lectura
    if (archivo == NULL) {
        printf("\nError al abrir el archivo de agenda.\n");
        return;
    }
    
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo)) {
        char nombre[64];
        char telefono[32];
        char correo[64];
        
        if (sscanf(linea, "%[^,],%[^,],%[^\n]", nombre, telefono, correo) == 3) {
            add_contact(trie, nombre, nombre, telefono, correo);
        } else {
            printf("\nError al leer una linea del archivo de agenda: %s\n", linea);
        }
    }
    
    fclose(archivo); // Cerrar el archivo
}
void imprimir_trie(Trie trie, int nivel) {
    if (trie != NULL) {
        ListaT hijos = trie->hijos;
        while (hijos != NULL) {
            int i;
            for (i = 0; i < nivel; i++) {
                printf("  ");
            }
            if (hijos->trie->contacto != NULL) {
                printf("(%c)\n", hijos->trie->letra);
            } else {
                printf("%c\n", hijos->trie->letra);
            }
            imprimir_trie(hijos->trie, nivel + 1);
            hijos = hijos->sig;
        }
    }
}
void search_contact(Trie trie) {
    char nombre[64];
    printf("Ingrese el nombre del contacto a buscar: ");
    scanf("%s", nombre);
    
    Trie nodo_actual = trie;
    ListaT lista_actual = nodo_actual->hijos;
    
    for (int i = 0; i < strlen(nombre); i++) {
        char letra = tolower(nombre[i]);
        ListaT nodo_encontrado = NULL;
        
        while (lista_actual != NULL && nodo_encontrado == NULL) {
            if (lista_actual->trie->letra == letra) {
                nodo_encontrado = lista_actual;
            } else {
                lista_actual = lista_actual->sig;
            }
        }
        
        if (nodo_encontrado != NULL) {
            nodo_actual = nodo_encontrado->trie;
            lista_actual = nodo_actual->hijos;
        } else {
            printf("El contacto no existe\n");
            return;
        }
    }
    
    if (nodo_actual->contacto != NULL) {
        printf("%s - %s - %s\n", nodo_actual->contacto->nombre, nodo_actual->contacto->telefono, nodo_actual->contacto->correo);
    } else {
        printf("El contacto no existe\n");
    }
}
void listarContactos(Trie t,char* c)
{
    if(*c)
    {
        Trie hijo = buscarTrie(t->hijos, *c);
        
        if(hijo)
        {   
            
            listarContactos(hijo,c + 1);
        }
        else
        {   
            printf("No se encontraron coincidencias ");
        }
    }
    else
    {
        if (t->contacto != NULL)
        {   
            printf("\n%s (%s, %s)", t->contacto->nombre, t->contacto->telefono, t->contacto->correo);
    
        }
        ListaT q = t->hijos;
        while(q)
        {   
            listarContactos(q->trie, c);
            q = q->sig;
        }
    }
    
}
void BuscarContacto(Trie t)
{
    char nombre[64];
    printf("Ingrese el nombre del contacto a Buscar: ");
    fgets(nombre, 64, stdin);
    nombre[strcspn(nombre, "\n")] = 0; // Eliminar carácter de nueva línea

    listarContactos(t,nombre);

}
Trie buscarTrie(ListaT t, char c)
{       
        ListaT l = t;
    
        while (l != NULL && l->trie->letra != c)
        {
            l=l->sig;
        }
        if(l!=NULL)
        {   
            return l->trie;
        }
        else
        {   
            return NULL;
        }
        
        
}
void flushstdin(FILE *where) {
    int ch;
    while (((ch = fgetc(where)) != '\n') && (ch != EOF)) /* void */;
}
char* strlwr(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}
