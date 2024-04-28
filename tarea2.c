#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  char director[200];
  float rating;
  int year;
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

void cargar_peliculas(Map *pelis_byid) {
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
    char *temp = strdup(campos[14]);
    strcpy(peli->director,temp);
    peli->rating = atof(campos[8]);
    peli->genres = list_create();

    char *genre_token;
    char *genre_copy = strdup(campos[11]);
    genre_token = strtok(genre_copy, ",");
    while (genre_token != NULL) {
        list_pushBack(peli->genres, strdup(genre_token));
        genre_token = strtok(NULL, ",");
    }
    
    peli->year = atoi(campos[10]);

    map_insert(pelis_byid, peli->id, peli);
  }
  fclose(archivo);

  // se muestran las peliculas
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title, peli->year);
    pair = map_next(pelis_byid);
  }
}

void buscar_por_id(Map *pelis_byid) {
  char id[10];
  printf("Ingrese el id de la película: ");
  scanf("%s", id);

  MapPair *pair = map_search(pelis_byid, id);
  if (pair != NULL) {
    Film *peli = pair->value;
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } else
    printf("La película con id %s no existe\n", id);
}

void buscar_por_director(Map *pelis_byid) {
  char director[200];
  printf("Ingrese el nombre del director: ");
  scanf(" %[^\n]", director);
  getchar();
  printf("Buscando películas del director: %s\n", director);

  MapPair *pair = map_first(pelis_byid);
  int count = 0;
  int count2 = 0;
  while (pair != NULL) {
    Film *peli = pair->value;
    count++;
    if (strcmp(director, peli->director) == 0) {
      printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title, peli->year);
      count2++;
    }
    pair = map_next(pelis_byid);
  }
  printf("Se leyeron %d\n", count);
  printf("Se mostraron %d\n", count2);
}

void buscar_por_genero(Map *pelis_byid) {
  char genero[100];
  printf("Ingrese el género: ");
  scanf("%s", genero);

  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    void *it = list_first(peli->genres);
    while(it != NULL) {
      if (is_equal_str(it, genero)) {
        printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title, peli->year);
        break;
      }
      it = list_next(peli->genres);
    }
    pair = map_next(pelis_byid);
  }
}

void buscar_por_rango_calificaciones(Map *pelis_byid) {
    float rango_inicial, rango_final;
    printf("Ingrese el rango de calificaciones (ejemplo: 6.0-6.4, 6.5-6.9, 7.0-7.4, etc.): ");
    scanf("%f-%f", &rango_inicial, &rango_final);

    MapPair *pair = map_first(pelis_byid);
    while (pair != NULL) {
        Film *peli = pair->value;
        if (peli->rating >= rango_inicial && peli->rating <= rango_final) {
            printf("ID: %s, Título: %s, Calificación: %.1f\n", peli->id, peli->title, peli->rating);
        }
        pair = map_next(pelis_byid);
    }
}

void buscar_por_decada(Map *pelis_byid) {
  int decada;
  printf("Ingrese la década (ejemplo: 1980, 2010): ");
  scanf("%d", &decada);
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    if (peli->year >= decada && peli->year < (decada + 10)) {
      printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title, peli->year);
    }
    pair = map_next(pelis_byid);
  }
  /*char decada[100];
  printf("Ingrese la década (ejemplo: 1980s, 2010s): ");
  scanf("%s", decada);

  // Extraer el año inicial de la década
  int inicio_decada = 0;
  int i = 0;
  while (decada[i] >= '0' && decada[i] <= '9') {
    inicio_decada = inicio_decada * 10 + (decada[i] - '0');
    i++;
  }

  if (inicio_decada == 0 || decada[i] != 's') {
    printf("Formato de década no válido.\n");
    return;
  }

  int fin_decada = inicio_decada + 9;

  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    Film *peli = pair->value;
    if (is_equal_int(&peli->year, &inicio_decada)) {
      printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title, peli->year);
    }
    pair = map_next(pelis_byid);
  }*/
}

int main() {
  char opcion;
  Map *pelis_byid = map_create(is_equal_str);
  // Recuerde usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid);//Listo
      break;
    case '2':
      buscar_por_id(pelis_byid);//Listo
      break;
    case '3':
      buscar_por_genero(pelis_byid);//Listo
      break;
    case '4':
      buscar_por_director(pelis_byid);//No Listo
      break;
    case '5':
      buscar_por_decada(pelis_byid);//Listo
      break;
    case '6':
      buscar_por_rango_calificaciones(pelis_byid);//No listo
      break;
    case '7':
      break;
    default:
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}
