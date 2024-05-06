#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 100

char **leer_linea_csv(FILE *archivo, char separador) {
  static char linea[MAX_LINE_LENGTH];
  static char *campos[MAX_FIELDS];
  char *ptr, *start;
  int idx = 0;

  if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL) {
    return NULL; // No hay más líneas para leer
  }

  ptr = start = linea;
  while (*ptr) {
    if (idx >= MAX_FIELDS - 1)
      break;

    if (*ptr == '\"') { // Inicio de un campo entrecomillado
      start = ++ptr;    // Saltar la comilla inicial
      while (*ptr && !(*ptr == '\"' && *(ptr + 1) == separador))
        ptr++;
    } else { // Campo sin comillas
      start = ptr;
      while (*ptr && *ptr != separador)
        ptr++;
    }

    if (*ptr) {
      *ptr = '\0'; // Reemplazar comilla final o separador por terminación
      ptr++;
      if (*ptr == separador)
        ptr++;
    }

    // Quitar comilla final si existe
    if (*(ptr - 2) == '\"') {
      *(ptr - 2) = '\0';
    }

    campos[idx++] = start;
  }

  campos[idx] = NULL; // Marcar el final del array
  return campos;
}

typedef struct { // Estructura para almacenar los datos de la linea
  char id[100]; // id de la pelicula 
  char title[100]; // Nombre de la pelicula
  List *genres; // Lista de generos 
  List *director; // Lista de directores
  int rating; // ranting de la pelicula 
  int year; // año de la pelicula 
} Film;

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por género");
  puts("4) Buscar por director");
  puts("5) Buscar por década");
  puts("6) Buscar por rango de calificaciones");
  puts("7) Buscar por década y género");
  puts("8) Salir");
}

List* split_string(const char *str, const char *delim) {
    List *result = list_create();
    char *token = strtok((char *)str, delim);

    while (token != NULL) {
        // Eliminar espacios en blanco al inicio del token
        while (*token == ' ') {
            token++;
        }

        // Eliminar espacios en blanco al final del token
        char *end = token + strlen(token) - 1;
        while (*end == ' ' && end > token) {
            *end = '\0';
            end--;
        }

        // Copiar el token en un nuevo string
        char *new_token = strdup(token);

        // Agregar el nuevo string a la lista
        list_pushBack(result, new_token);

        // Obtener el siguiente token
        token = strtok(NULL, delim);
    }

    return result;
}

void cargarpeli(Map *Mapa) {
  map_clean(Mapa);
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror("Error al abrir el archivo");
    return;
  }

  char **campos;
  campos = leer_linea_csv(archivo, ','); // encabezados
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Film *peli = (Film *)malloc(sizeof(Film));
    strcpy(peli->id, campos[1]);
    strcpy(peli->title, campos[5]);
    peli->director = list_create();
    peli->director = split_string(campos[14], ",");
    peli->rating = (int) (atof(campos[8]) * 10);
    peli->genres = list_create();
    peli->genres = split_string(campos[11], ",");
    peli->year = atoi(campos[10]);

    map_insert(Mapa, peli->id, peli);
  }
  fclose(archivo);
  limpiarPantalla();
  puts("========================================");
  puts("     Cargar Peliculas");
  puts("========================================");
  puts("");
  puts("Peliculas cargadas:");
  puts("");
  // se muestran las peliculas
  MapPair *pair = map_first(Mapa);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title, peli->year);
    pair = map_next(Mapa);
  }
}

void cargarpeli_genero(Map *pelisgenero,Map *pelis){ //Funcion que resive el mapa pelisgenero y el mapa pelis
  MapPair *pair = map_first(pelis);
  map_clean(pelisgenero);
  while (pair != NULL) {
    Film *peli = pair->value;
    void *it = list_first(peli->genres);
    while(it != NULL) {
      MapPair *busqueda = map_search(pelisgenero,it);
      if( busqueda == NULL){ //Si no existe el genero en el mapa pelisgenero
        List *lista = list_create();
        list_pushBack(lista,peli);
        map_insert(pelisgenero, it, lista);
      }
      else{ //Si no existe el genero en el mapa pelisgenero
        list_pushBack(busqueda->value, peli);
      }
       // Insertar la película en el mapa por género
      it = list_next(peli->genres);
    }
    pair = map_next(pelis);
  }
}
void cargarpeli_director(Map *pelisdirector,Map *pelis){
  MapPair *iter = map_first(pelis); // Obtiene el primer par clave-valor del mapa pelis
  map_clean(pelisdirector);
  while (iter != NULL) { // Itera sobre todos los pares clave-valor del mapa pelis
    Film *peli = iter->value; // Obtiene la película del valor del par clave-valor actual
    void *node = list_first(peli->director); // Obtiene el primer elemento de la lista de directores de la película
    while (node != NULL) { // Itera sobre todos los directores de la película
      char *director = (char *)node; // Obtiene el director actual de la película

      // Limpia los espacios en blanco iniciales y finales del nombre del director
      // Convierte el nombre del director a minúsculas para mantener la consistencia
      char cleaned_director[100];
      strcpy(cleaned_director, director);
      // Elimina los espacios iniciales
      while (isspace((unsigned char)cleaned_director[0])) {
        memmove(cleaned_director, cleaned_director + 1, strlen(cleaned_director));
      }
      // Elimina los espacios finales
      char *end = cleaned_director + strlen(cleaned_director) - 1;
      while (end > cleaned_director && isspace((unsigned char)*end)) {
        end--;
      }
      *(end+1) = '\0';
      // Convierte a minúsculas
      for(char *c = cleaned_director; *c; c++) {
        *c = tolower(*c);
      }

      MapPair *par = map_search(pelisdirector, cleaned_director); // Busca el director en el mapa pelisdirector
      if (par == NULL) { // Si el director no existe en el mapa pelisdirector
        List *lista = list_create(); // Crea una nueva lista para las películas del director
        map_insert(pelisdirector, strdup(cleaned_director), lista); // Inserta el director y la lista en el mapa pelisdirector
        list_pushBack(lista, peli); // Agrega la película a la lista
      } else { // Si el director ya existe en el mapa pelisdirector
        List *lista = (List *) par->value; // Obtiene la lista de películas asociadas con el director
        if (lista == NULL) { // Si la lista está vacía
          lista = list_create(); // Crea una nueva lista
          par->value = lista; // Asigna la nueva lista al par clave-valor
        }
        list_pushBack(lista, peli); // Agrega la película a la lista existente
      }
      node = list_next(peli->director); // Avanza al siguiente director de la película
    }
    iter = map_next(pelis); // Avanza al siguiente par clave-valor del mapa pelis
}
}
void cargarpeli_decada(Map *pelisdecada,Map *pelis){
  // Itera sobre el mapa pelis
  MapPair *iter = map_first(pelis);
  map_clean(pelisdecada);
  while (iter != NULL) {
    Film *peli = iter->value; // Obtiene la película del valor del par clave-valor actual

    // Busca si ya existe el año en el mapa pelisdecada
    MapPair *found_year = map_search(pelisdecada, &peli->year);

    if (found_year == NULL) { // Si el año no existe en el mapa pelisdecada
      List *lista = list_create(); // Crea una nueva lista para el año
      map_insert(pelisdecada, &peli->year, lista); // Inserta el año como clave y la lista como valor en el mapa pelisdecada
      list_pushBack(lista, peli); // Agrega la película a la lista
    } else { // Si el año ya existe en el mapa pelisdecada
      List *lista = found_year->value; // Obtiene la lista asociada con el año encontrado
      list_pushBack(lista, peli); // Agrega la película a la lista existente
    }

    iter = map_next(pelis); // Avanza al siguiente par clave-valor en el mapa pelis
  }
}
void cargarpeli_califica(Map *peliscalifica, Map *pelis) {
  // Itera sobre el mapa pelis
  MapPair *iter = map_first(pelis);
  map_clean(peliscalifica);
  while (iter != NULL) {
    Film *peli = iter->value; // Obtiene la película del valor del par clave-valor actual

    // Busca si ya existe el año en el mapa pelisdecada
    MapPair *found_calif = map_search(peliscalifica, &peli->rating);

    if (found_calif == NULL) { // Si el año no existe en el mapa pelisdecada
      List *lista = list_create(); // Crea una nueva lista para el año
      map_insert(peliscalifica, &peli->rating, lista); // Inserta el año como clave y la lista como valor en el mapa pelisdecada
      list_pushBack(lista, peli); // Agrega la película a la lista
    } else { // Si el año ya existe en el mapa pelisdecada
      List *lista = found_calif->value; // Obtiene la lista asociada con el año encontrado
      list_pushBack(lista, peli); // Agrega la película a la lista existente
    }

    iter = map_next(pelis); // Avanza al siguiente par clave-valor en el mapa pelis
  }
}

void buscar_por_id(Map *pelis) {
  limpiarPantalla();
  puts("========================================");
  puts("     Busqueda por Id");
  puts("========================================");
  puts("");
  char id[10];
  printf("Ingrese el id de la película: "); //Ingresa el id de la pelicula 
  scanf("%s", id);
  puts("");
  MapPair *pair = map_search(pelis, id);
 
  if (pair != NULL) { // Si es que encunetra la pelicula lo muestra 
    Film *peli = pair->value;
    printf("La pelicula es '%s' del año %d.\n\n", peli->title, peli->year);
    
  } else // Si es que no encuentra la pelicula muestra el mensaje de que no hay pelicula con esa id
    printf("La película con id %s no existe\n", id);
}
void buscar_por_genero(Map *pelis) {
  limpiarPantalla();
  puts("========================================");
  puts("     Busqueda por Género");
  puts("========================================");
  puts("");
  char genero[100];
  printf("Ingrese el género en el idioma ingles: "); //Ingresa el genero de la pelicula 
  scanf("%s", genero);
  puts("");
  MapPair *pair = map_search(pelis, genero);

  if (pair != NULL) { // Si es que encuentra el genero lo muestra todas las peliculas de ese genero
    printf("Peliculas con el genero '%s':\n", genero);
    puts("");
    List *peli = pair->value;
    Film *aux = list_first(peli);
    while (aux != NULL) {
      printf("ID: %s, Título: %s, Año: %d\n\n", aux->id, aux->title, aux->year);
      aux =list_next(peli);
    }
  } else { // Si es que no encuentra el genero muestra el mensaje de que no hay pelicula
    printf("No se encontraron películas en el género '%s'\n", genero);
    puts("");
  }
}
void buscar_por_director(Map *pelis) {
  limpiarPantalla(); // Limpia la pantalla para una presentación limpia
  puts("========================================");
  puts("     Busqueda por Director");
  puts("========================================");
  puts("");

  char director[100];
  printf("Ingrese el director: ");
  scanf(" %[^\n]", director); // Solicita al usuario que ingrese el nombre del director
  puts("");

  // Limpia el nombre del director para mantener la consistencia
  char cleaned_director[100];
  strcpy(cleaned_director, director);

  // Elimina los espacios iniciales y finales del nombre del director
  while (isspace((unsigned char)cleaned_director[0])) {
    memmove(cleaned_director, cleaned_director + 1, strlen(cleaned_director));
}

char *end = cleaned_director + strlen(cleaned_director) - 1;
while (end > cleaned_director && isspace((unsigned char)*end)) {
  end--;
}
*(end + 1) = '\0';

// Convierte el nombre del director a minúsculas para mantener la consistencia
  for (char *c = cleaned_director; *c; c++) {
    *c = tolower(*c);
}

// Busca películas asociadas con el director limpiado en el mapa de películas
MapPair *pair = map_search(pelis, cleaned_director);

if (pair != NULL) { // Si se encuentra el director en el mapa de películas
  printf("Peliculas del director '%s':\n", director);
  puts("");
  List *peli = pair->value; // Obtiene la lista de películas asociadas con el director
  Film *aux = list_first(peli); // Obtiene la primera película de la lista

  while (aux != NULL) { // Mientras haya películas en la lista
    printf("ID: %s, Título: %s, Año: %d\n\n", aux->id, aux->title, aux->year); // Imprime los detalles de la película
    aux = list_next(peli); // Avanza al siguiente elemento de la lista
  }
} else { // Si no se encuentra el director en el mapa de películas
  printf("No se encontraron películas con el director: '%s'\n", director); // Muestra un mensaje indicando que no se encontraron películas
  puts("");
}
}
void buscar_por_decada(Map *pelis) {
  limpiarPantalla(); // Limpia la pantalla para una presentación limpia
  puts("========================================");
  puts("     Busqueda por Decada");
  puts("========================================");
  puts("");

  int i;
  printf("Ingrese la década (Ej: 1980): ");
  scanf("%d", &i); // Solicita al usuario que ingrese la década
  puts("");

  if (i % 10 == 0) { // Verifica si el número ingresado representa una década

    bool se_encontro_pelicula = false; // Bandera para verificar si se encontraron películas para la década ingresada

    for (int decada = i; decada < i + 10; decada++) { // Itera sobre los años de la década

      MapPair *pair = map_search(pelis, &decada); // Busca películas para el año actual de la década

      if (pair != NULL) { // Si se encontraron películas para el año actual de la década

        // Solo muestra el mensaje si aún no se ha mostrado para esta década.
        if (!se_encontro_pelicula) {
          printf("Peliculas en la década de %ds:\n", decada);
          puts("");
          se_encontro_pelicula = true; // Se ha encontrado al menos una película para esta década.
        }

        List *peli = pair->value; // Obtiene la lista de películas para el año actual de la década
        Film *aux = list_first(peli); // Obtiene la primera película de la lista

        while (aux != NULL) { // Mientras haya películas en la lista
          printf("Título: %s, Año: %d\n\n", aux->title, aux->year); // Imprime los detalles de la película
          aux = list_next(peli); // Avanza al siguiente elemento de la lista
        }
      } else { // Si no se encontraron películas para el año actual de la década

        // Solo muestra el mensaje si aún no se ha mostrado para esta década.
        if (!se_encontro_pelicula) {
          printf("No se encontraron películas en la década de %ds\n", decada);
          puts("");
          se_encontro_pelicula = true; // Se ha encontrado al menos una película para esta década.
        }
      }
    }
  } else {
    printf("%d no es una década\n\n", i); // Muestra un mensaje si el número ingresado no representa una década
  }
}
void buscar_por_rango_calificaciones(Map *pelis) {
  // Imprime los encabezados para la búsqueda por rango de calificación
  limpiarPantalla();
  puts("========================================");
  puts("     Búsqueda por Rango de Calificacion");
  puts("========================================");
  puts("");

  // Variables para almacenar el rango inicial y final de calificaciones
  float rango_inicial, rango_final;

  // Solicita al usuario que ingrese el rango de calificaciones
  printf("Ingrese el rango de calificaciones (por ejemplo, 6.0-6.4, 6.5-6.9): ");
  scanf("%f-%f", &rango_inicial, &rango_final);
  puts("");

  // Convierte las calificaciones de punto flotante a enteros multiplicando por 10
  int int_inicial = (int)(rango_inicial * 10), int_final = (int)(rango_final * 10);
  if(int_inicial%10 == 0 || int_inicial%10 == 5){
    if(int_final%10 == 4 || int_final%10 == 9){  
      // Itera sobre las calificaciones y muestra las películas dentro del rango especificado
      bool se_encontraron_peliculas = false;
      for (int rango = int_inicial; rango <= int_final; rango++) {
        MapPair *pair = map_search(pelis, &rango); // Busca películas para la calificación actual
    
        if (pair != NULL) { // Si se encuentran películas para la calificación actual
    
          // Muestra el mensaje solo si aún no se ha mostrado para este rango de calificación
          if (!se_encontraron_peliculas) {
            printf("Peliculas en el rango de %.1fs-%.1f:\n", rango_inicial, rango_final);
            puts("");
            se_encontraron_peliculas = true; // Se ha encontrado al menos una película en este rango
          }
    
          // Itera sobre las películas para la calificación actual y las imprime
          List *peliculas_calificacion = pair->value;
          void *it = list_first(peliculas_calificacion);
          while (it != NULL) {
            Film *aux = (Film *)it;
            float rangoPeli = ((float) aux->rating) / 10; // Convierte la calificación de la película a punto flotante
            printf("ID: %s, Título: %s, Calificación: %.1f\n", aux->id, aux->title, rangoPeli);
            it = list_next(peliculas_calificacion); // Avanza al siguiente elemento de la lista de películas
          }
        }
      }
    
      // Si no se encontraron películas en el rango de calificaciones especificado, muestra un mensaje
      if (!se_encontraron_peliculas) {
        printf("No se encontraron películas en el rango de calificaciones %.1f-%.1f\n", rango_inicial, rango_final);
      }
    }
    else puts("Rango final mal ingresado");
  }
  else puts("Rango inicial mal ingresado");
}
void buscar_por_decada_y_genero(Map *pelisYear, Map *pelisGenero) {
  limpiarPantalla();
  puts("========================================");
  puts("     Busqueda por Decada y Genero");
  puts("========================================");
  puts("");
  int year; // Variable para almacenar el año proporcionado por el usuario
  char genero[100]; // Buffer para almacenar el género proporcionado por el usuario

  // Solicitar al usuario la década de interés
  printf("Ingrese la década de películas a mostrar: ");
  scanf("%d", &year); // Leer la década del teclado

  // Solicitar al usuario el género de interés
  printf("Ingrese el género de películas a mostrar: ");
  scanf(" %[^\n]s", genero); // Leer el género del teclado

  // Calcular la década correspondiente al año proporcionado
  int decada = (int)(year / 10) * 10;

  // Buscar las películas de la década correspondiente en el mapa
  MapPair *pair_decada = map_search(pelisYear, &decada);

  // Si se encontraron películas para la década proporcionada, mostrarlas
  if (pair_decada != NULL) {
    List *pelisDecada = (List *)pair_decada->value;

    if (pelisDecada != NULL) {
      // Buscar las películas del género proporcionado en el mapa
      MapPair *pair_genero = map_search(pelisGenero, genero);

      if (pair_genero != NULL) {
        printf("\nPelículas estrenadas en la década de los %d y del género '%s':\n\n", decada, genero);
        List *listaGenero = (List *)pair_genero->value;

        // Iterar sobre las películas del género
        void *peliActual = list_first(listaGenero);
        while (peliActual != NULL) {
          Film *peli = (Film *)peliActual;

          // Verificar si la película pertenece a la década especificada
          if (peli->year / 10 * 10 == decada) {
            printf("ID: %s, Título: %s, Año: %d\n\n", peli->id, peli->title, peli->year);
          }

          peliActual = list_next(listaGenero); // Avanzar al siguiente nodo en la lista de películas del género
        }
      } else {
        printf("No se encontraron películas del género '%s' en la década especificada.\n", genero);
      }
    } else {
      printf("No se encontraron películas para la década especificada.\n");
    }
  } else {
    printf("No se encontraron películas para la década especificada.\n");
  }
}

int main() {
  char opcion;
  Map *peliculasid = map_create(is_equal_str); // Se crea el mapa peliculasid
  Map *peliculasGenero = map_create(is_equal_str); // Se crea el mapa peliculasGenero 
  Map *peliculasDirector = map_create(is_equal_str); // Se crea el mapa peliculasDirector
  Map *peliculasDecada = map_create(is_equal_int); // Se crea el mapa peliculasDecada
  Map *peliculasCalificaciones = map_create(is_equal_str); // Se crea el mapa peliculasCalificaciones
  // Recuerde usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargarpeli(peliculasid); // Cargar peliculas por id 
      cargarpeli_genero(peliculasGenero,peliculasid); // Cargar peliculas por genero 
      cargarpeli_director(peliculasDirector,peliculasid); // Cargar peliculas por director 
      cargarpeli_decada(peliculasDecada,peliculasid); // Cargar peliculas por decada 
      cargarpeli_califica(peliculasCalificaciones,peliculasid); // Cargar peliculas por calificacion 
      break;
    case '2':
      buscar_por_id(peliculasid); //Busca por id en el mapa de peliculasid
      break;
    case '3':
      buscar_por_genero(peliculasGenero); //Busca por genero en el mapa de peliculasGenero
      break;
    case '4':
      buscar_por_director(peliculasDirector); //Busca por director en el mapa de peliculasDirector
      break;
    case '5':
      buscar_por_decada(peliculasDecada); //Busca por decada en el mapa de peliculasDecada
      break;
    case '6':
      buscar_por_rango_calificaciones(peliculasCalificaciones); //Busca por calificaciones en el mapa de peliculasCalificaciones
      break;
    case '7':
      buscar_por_decada_y_genero(peliculasDecada, peliculasGenero); //Busca por decada_y_genero en en los mapas peliculasDecada y peliculasGenero
      break;
    default:
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}