# Aplicación de Busqueda de Peliculas

## Descripción de la aplicación
Este sistema permite a los usuarios buscar por diferentes criterios mas de 1500 peliculas. El usuario debe cargar las peliculas y podra buscar por id, género, director y mas. Esta herramienta está diseñada para entregar una comoda y eficaz busqueda especializada de peliculas.

## Ejemplo de uso de la aplicación
### Ejecución
Dado que el sistema está diseñado para ser accesible y fácil de probar, recomendamos usar `repl.it` para una rápida configuración y ejecución. Sigue estos pasos para comenzar:
* Visita `repl.it`.
* Crea una nueva cuenta o inicia sesión si ya tienes una.
* Una vez en tu dashboard, selecciona `New Repl` y elige `Import from GitHub`.
* Pega la URL de este repositorio.
* repl.it clonará el repositorio y preparará un entorno de ejecución.
* Para ejecutar el main en `repl.it` primero debemos compilar (en la carpeta raíz):
````
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2
````

* Y luego ejecutar:
````
./tarea2
````
### Uso de la aplicación
Al realizar la ejecución indicada se abrira el siguiente menu:
````
========================================
     Base de Datos de Películas
========================================
1) Cargar Películas
2) Buscar por id
3) Buscar por género
4) Buscar por director
5) Buscar por década
6) Buscar por rango de calificaciones
7) Buscar por década y género
8) Salir
Ingrese su opción:
````
Se debera ingresar el numero que acompaña a la accion en el menu para realizar la función deaseada.

A continuación lo que se puede realizar en cada una de las opciones:
* `Cargar Películas`: Ingresa automaticamente los datos de mas de 1000 peliculas y se muestra la siguiente información de cada una: id, titulo y año. Como en el ejemplo a continuación:
````
========================================
     Cargar Peliculas
========================================

Peliculas cargadas:

ID: tt0068646, Título: The Godfather, Año: 1972
ID: tt0034583, Título: Casablanca, Año: 1942
ID: tt0061722, Título: The Graduate, Año: 1967
ID: tt0033467, Título: Citizen Kane, Año: 1941
...
...
ID: tt0143145, Título: The World Is Not Enough, Año: 1999
ID: tt0040982, Título: You Gotta Stay Happy, Año: 1948
Presione una tecla para continuar...
````
* `Buscar por id`: Ingresa el ID de alguna pelicula cargada y se muestra la pelicula en cuestión. Como en el ejemplo a continuación:
````
========================================
     Busqueda por Id
========================================

Ingrese el id de la película: tt1758692

La pelicula es 'Like Crazy' del año 2011.

Presione una tecla para continuar...
````
* `Buscar por género`: El usuario ingresa el genero de peliculas que desee (en ingles) y se muestra cada pelicula con el genero ingresado. Como en el ejemplo a continuación:
````
========================================
     Busqueda por Género
========================================

Ingrese el género en el idioma ingles: Drama

Peliculas con el genero 'Drama':

ID: tt0068646, Título: The Godfather, Año: 1972

ID: tt0034583, Título: Casablanca, Año: 1942
...
...
ID: tt0084602, Título: Rocky III, Año: 1982

ID: tt2027064, Título: Blue Caprice, Año: 2013

Presione una tecla para continuar...
````
* `Buscar por director`: El usuario ingresa el nombre completo de un director en especifico y se muestra cada pelicula del director solicitado. Como en el ejemplo a continuación:
````
========================================
     Busqueda por Director
========================================

Ingrese el director: Steven Spielberg

Peliculas del director 'Steven Spielberg':

ID: tt0108052, Título: Schindler's List, Año: 1993

ID: tt0120815, Título: Saving Private Ryan, Año: 1998
...
...
ID: tt0118607, Título: Amistad, Año: 1997

ID: tt0119567, Título: The Lost World: Jurassic Park, Año: 1997

Presione una tecla para continuar..
````
* `Buscar por década`: Ingresa una decada y se muestran todas las peliculas de la decada ingresada. Como en el ejemplo a continuación:
````
========================================
     Busqueda por Decada
========================================

Ingrese la década (Ej: 1980): 2010

Peliculas en la década de 2010s:

Título: Inception, Año: 2010

Título: Black Swan, Año: 2010
...
...
Título: Sweetwater, Año: 2013

Título: Blue Caprice, Año: 2013

Presione una tecla para continuar...
````
* `Buscar por rango de calificaciones`: Ingresa dos calificaciónes, la primera menor que la segunda, y busca las todas peliculas que esten dentro de ese rango de calificación. Finalmente muestra los datos de las peliculas como en el ejemplo a continuación:
````
========================================
     Búsqueda por Rango de Calificacion
========================================

Ingrese el rango de calificaciones (por ejemplo, 6.0-6.4): 9.0-9.4

Peliculas en el rango de 9.0s-9.4:

ID: tt0050083, Título: 12 Angry Men, Calificación: 9.0
ID: tt0108052, Título: Schindler's List, Calificación: 9.0
ID: tt0071562, Título: The Godfather Part II, Calificación: 9.0
ID: tt0167260, Título: The Lord of the Rings: The Return of the King, Calificación: 9.0
ID: tt0468569, Título: The Dark Knight, Calificación: 9.0
ID: tt0068646, Título: The Godfather, Calificación: 9.2
ID: tt0111161, Título: The Shawshank Redemption, Calificación: 9.3
Presione una tecla para continuar...
````
* `Buscar por década y género`: El usuario ingresa en primer lugar la decada para posteriormente ingresar el genero, esto para filtrar todas las peliculas con aquellos parametros y muestra los datos de esas peliculas. Como en el ejemplo a continuación:
````
========================================
     Busqueda por Decada y Genero
========================================

Ingrese la década de películas a mostrar: 1940
Ingrese el género de películas a mostrar: Drama

Películas estrenadas en la década de los 1940 y del género 'Drama':

ID: tt0034583, Título: Casablanca, Año: 1942

ID: tt0033467, Título: Citizen Kane, Año: 1941
...
...
ID: tt0034890, Título: In This Our Life, Año: 1942

Presione una tecla para continuar...
````
* `Salir`: Terminar el programa.

## Especificaciones de la aplicacion
### Funcionalidades de la aplicación
* `cargarpeli` y variantes: Se ingresa la ubicación de un archivo csv, y se cargan automaticamente las películas en los diversos mapas con las siguientes funciones: `cargarpeli_genero`, `cargarpeli_director`, `cargarpeli_decada`, `cargarpeli_califica`.
* `buscar_por_id`: Se ingresa el ID de una película y la aplicación muestra la información de esta.
* `buscar_por_genero`: Muestra todas las películas que pertenecen al género ingresado. Si no hay peliculas o no existe el genero ingresado se muestra un aviso.
* `buscar_por_director`: Muestra todas las películas dirigidas por el director ingresado. Si no existe el director o se ingresa el nombre de forma erronea se muestra un aviso.
* `buscar_por_decada`: Muestra todas las películas que fueron estrenadas en la década ingresada. Si la decada ingresada no existe o no hay peliculas en aquella decada se muestra un aviso.
* `buscar_por_rango_calificaciones`: Muestra todas las películas que tengan una calificación dentro de un rango específico. Si no hay peliculas en el rango se muestra un aviso.
* `buscar_por_decada_y_genero`: Muestra todas las películas que pertenezcan al género y hayan sido lanzadas en la década indicados por la usuaria. Si no existe el genero o no hay peliculas en la decada se muestra un aviso.

### Posibles errores en la aplicación
* Los principales errores que podrian ocurrir en la aplicación son `errores humanos`, ya que, si el usuario ingresa palabras/letras en las secciones donde se deben ingresar numeros  la aplicación se salta la seccion completa.
* Otro error, es cuando el usuario desea buscar en cualquier bajo cualquier tipo de criterio sin haber cargado las peliculas previamente, esto se traduce en que ninguna de las funciones dedicadas a buscar se efectuen correctamente y muestren el aviso que no se encontro lo solicitado.