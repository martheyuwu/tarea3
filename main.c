#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include "treemap.h"
#include "list.h"
typedef struct{
    char nombre[50];
    char ano_salida[50];                //estructura de juegos
    int valoracion;
    int precio;
}videojuego;
void importar_juegos(List*,HashMap*,TreeMap*,TreeMap*);
void agregar_juegos(List*,HashMap*,TreeMap*,TreeMap*);
int lower_than_int(void* key1, void* key2);
void juegos_precio(List*,TreeMap*);
void juegos_valoracion(List*,TreeMap*);                                     //funciones (lower_than_int es la comparacion de claves de tipo int)
void juegos_valoracion_ano(List*);
char *comparacion_ano(char[]);
void buscar_juegos(List*,HashMap*,TreeMap*,TreeMap*);
void exportar_propiedades(List *listajuegos);
int main(){
    int op,aux=1;       //variables para el menú
    List *listajuego=createList();    //se guardan todos los juegos
    HashMap *mapajuego=createMap(100);    //para buscar un juego
    TreeMap *map=createTreeMap(lower_than_int);      //para ordenar por precio
    TreeMap *mapval=createTreeMap(lower_than_int);      //para ordenar por valor
    while (op!=8){       //se termina el programa cuando op es 8
        if (aux==0){
            printf("\n********************\n");
            printf("operacion realizada\n");
            printf("********************\n");
        }
        printf("----------------------------------------\n");
        printf("1.- Importar juegos\n");
        printf("2.- Agregar juego\n");
        printf("3.- mostrar juegos por precio\n");
        printf("4.- filtrar juegos por valoracion\n");
        printf("5.- Mostrar juegos del año\n");
        printf("6.- buscar juego\n");
        printf("7.- exportar datos\n");
        printf("8.- terminar programa\n");
        printf("----------------------------------------\n");
        printf("ingrese operacion a realizar: ");
        scanf("%d",&op);
        switch (op){
            case 1: importar_juegos(listajuego,mapajuego,map,mapval);
                    aux=0;
                    break;
            case 2: agregar_juegos(listajuego,mapajuego,map,mapval);
                    aux=0;
                    break;                                                       //aux es para imprimir "operación realizada"
            case 3: juegos_precio(listajuego,map);
                    aux=0;
                    break;
            case 4: juegos_valoracion(listajuego,mapval);
                    aux=0;
                    break;
            case 5: juegos_valoracion_ano(listajuego);
                    aux=0;
                    break;
            case 6: buscar_juegos(listajuego,mapajuego,map,mapval);
                    aux=0;
                    break;
            case 7: exportar_propiedades(listajuego);
                    aux=0;
                    break;
            case 8: printf("Adios...\n");
                    break;
        }
    }
}
void importar_juegos(List *listajuego,HashMap *mapajuego,TreeMap *map,TreeMap *mapval){
    char *nombre,*archivo,*ano,*valoracion,*precio;
    videojuego *juegos;
    char linea[1024];
    int precioint,valoracionint;    //variables donde se guardaran los char transformados a int
    Pairt *partree;
    archivo=(char*)malloc(50*sizeof(char));
    printf("ingrese nombre del archivo: ");
    fflush(stdin);
    scanf("%50[^\n]s",archivo);      //para leer todo y no solo una parte del char
    FILE *fp = fopen (archivo, "r");
    fgets(linea,1024,fp);
    while (fgets(linea,1024,fp)!=NULL){  //mientras que no se llegue al final...
        juegos=(videojuego*)malloc(sizeof(videojuego));
        nombre=get_csv_field(linea,0);
        ano=get_csv_field(linea,1);
        valoracion=get_csv_field(linea,2);
        precio=get_csv_field(linea,3);
        strcpy(juegos->nombre,nombre);
        strcpy(juegos->ano_salida,ano);
        valoracionint=atoi(valoracion);   //atoi para pasar a int
        juegos->valoracion=valoracionint;
        precioint=atoi(precio);
        juegos->precio=precioint;
        insertTreeMap(map,&juegos->precio,juegos);
        insertTreeMap(mapval,&juegos->valoracion,juegos);  //se insertan los datos
        pushBack(listajuego,juegos);
        insertMap(mapajuego,juegos->nombre,juegos);
    }
}
void agregar_juegos(List *listajuego,HashMap *mapajuego,TreeMap *map,TreeMap *mapval){
    char nombre[70],ano[70];
    int precio,valoracion;
    printf("ingrese nombre del juego: ");
    fflush(stdin);
    scanf("%70[^\n]s",nombre);
    printf("ingrese el ano de salida: ");
    fflush(stdin);
    scanf("%70[^\n]s",ano);
    printf("ingrese valoracion: ");
    fflush(stdin);
    scanf("%d",&valoracion);
    printf("ingrese precio: ");
    fflush(stdin);
    scanf("%d",&precio);
    videojuego *juegos=(videojuego*)malloc(sizeof(videojuego));
    strcpy(juegos->nombre,nombre);
    strcpy(juegos->ano_salida,ano);
    juegos->valoracion=valoracion;
    juegos->precio=precio;
    insertTreeMap(map,&juegos->precio,juegos);       //lo mismo que el importar pero con los respectivos scanf
    insertTreeMap(mapval,&juegos->valoracion,juegos);
    pushBack(listajuego,juegos);
    insertMap(mapajuego,juegos->nombre,juegos);
}
int lower_than_int(void* key1, void* key2){
    int k1 = *((int*) (key1));
    int k2 = *((int*) (key2));    //se castea como int
    return k1<k2;
}
void juegos_precio(List *listajuegos,TreeMap *map){
    int opcion;
    List *listaux=createList();
    videojuego *juegos=(videojuego*)malloc(sizeof(videojuego));
    videojuego *juegolista=(videojuego*)malloc(sizeof(videojuego));  //se reserva memoria para la estructura videojuegos
    printf("1. de menor a mayor\n");
    printf("2. de mayor a menor\n");
    printf("su opcion: ");
    scanf("%d",&opcion);
    Pairt *recorrer=firstTreeMap(map);  //se define el pair como el primero del treemap
    while (recorrer!=NULL){ //hasta que sea nulo
        juegos=recorrer->value;
        juegolista=firstList(listajuegos);
        while (juegolista!=NULL){   //se recorre la lista, si el precio del juego es igual al que se encuentra en la lista, se mete en listaux
            if (juegolista->precio==juegos->precio)       //lsitaux se utiliza para guardar el orden y poder imprimirlo de menor o de mayor orden
                pushBack(listaux,juegolista);
            juegolista=nextList(listajuegos);
        }
        recorrer=nextTreeMap(map);  //se sigue para el siguiente
    }
    if (opcion==1){   //se imprime nomalmente
        juegolista=firstList(listaux);
        while (juegolista!=NULL){
            printf("%s,%s,%d,%d\n",juegolista->nombre,juegolista->ano_salida,juegolista->valoracion,juegolista->precio);
            juegolista=nextList(listaux);
        }
    }
    if (opcion==2){  //se empieza del final hasta el principio (mayor a menor)
        juegolista=lastList(listaux);
        while (juegolista!=NULL){
            printf("%s,%s,%d,%d\n",juegolista->nombre,juegolista->ano_salida,juegolista->valoracion,juegolista->precio);
            juegolista=prevList(listaux);
        }
    }
}
void juegos_valoracion(List *listajuego,TreeMap *mapval){
    videojuego *juegos=(videojuego*)malloc(sizeof(videojuego));
    videojuego *juegolista=(videojuego*)malloc(sizeof(videojuego));
    int valoracion;
    printf("ingrese valoracion: ");
    scanf("%d",&valoracion);
    Pairt *recorrer=firstTreeMap(mapval);
    while (recorrer!=NULL){   //se recorre el treemap
        juegos=recorrer->value;
        if (juegos->valoracion>valoracion){  //si la valoración del treemap es mayor al ingresado...
            while (recorrer!=NULL){   // se usa la lista de juegos (por si se repite la valoración) y al terminar se rompe el while
                juegos=recorrer->value;
                juegolista=firstList(listajuego);
                while (juegolista!=NULL){   //se recorre la lista
                    if (juegolista->valoracion==juegos->valoracion)  //si la valoracion de juegoslista es igual al del treemap, de imprime
                        printf("%s,%s,%d,%d\n",juegolista->nombre,juegolista->ano_salida,juegolista->valoracion,juegolista->precio);
                    juegolista=nextList(listajuego);
                }
                recorrer=nextTreeMap(mapval);
            }
            break;
        }
        recorrer=nextTreeMap(mapval);  //si esque todavia el juego->valoracion es mayor a valoracion
    }
}
void juegos_valoracion_ano(List *listajuego){
    char ano[50],anocomp[50],*extraida;
    int mayorval=0;
    extraida=(char*)malloc(50*sizeof(char*));  //variable donde se almacena el año solamente y no el resto de la fecha
    videojuego *juegos=(videojuego*)malloc(sizeof(videojuego));
    printf("ingrese año a buscar: ");
    scanf("%s",ano);
    juegos=firstList(listajuego);
    while (juegos!=NULL){
        if (juegos->valoracion>mayorval){
            extraida=comparacion_ano(juegos->ano_salida);  //se utiliza la función para sacar solamente el año
            if (strcmp(extraida,ano)==0)  //si es igual al año se va actualizando la variable mayorval, donde se va almacenando la mayor valoración
                mayorval=juegos->valoracion;
        }
        juegos=nextList(listajuego);
    }
    juegos=firstList(listajuego);
    while (juegos!=NULL){
        if (juegos->valoracion==mayorval){
            extraida=comparacion_ano(juegos->ano_salida);
            if (strcmp(extraida,ano)==0)  //se compara con la listajuegos y si la valoración coincide, se imprimirá
                printf("%s,%s,%d,%d\n",juegos->nombre,juegos->ano_salida,juegos->valoracion,juegos->precio);
        }
        juegos=nextList(listajuego);
    }
}
void buscar_juegos(List *listajuegos,HashMap *mapajuegos,TreeMap *map,TreeMap *mapval){
    int opcion,datos,valoracion,precio,op;
    char nombre[50],ano[50];
    videojuego *juegos=(videojuego*)malloc(sizeof(videojuego));
    videojuego *nuevo=(videojuego*)malloc(sizeof(videojuego));
    videojuego *juegoslista=(videojuego*)malloc(sizeof(videojuego));
    juegos=firstList(listajuegos);
    printf("imprimiendo...\n");
    while (juegos!=NULL){  //se imprime todos los juegos
        printf("%s,%s,%d,%d\n",juegos->nombre,juegos->ano_salida,juegos->valoracion,juegos->precio);
        juegos=nextList(listajuegos);
    }
    printf("---------------------------\n");
    printf("1. modificar datos\n");
    printf("2. eliminar juego\n");
    printf("pulse cualquier otro número para salir\n");
    printf("---------------------------\n");
    printf("ingrese su opción: ");
    scanf("%d",&opcion);
    if (opcion==1 || opcion==2){ //si es 1 o 2, se procederá al if, si no, la función termina (osea si se ingresa cualquier otro caracter)
        if (opcion==1){
            printf("ingrese nombre del juego a modificar: ");
            fflush(stdin);
            scanf("%50[^\n]s",nombre);
            Pair *buscar=searchMap(mapajuegos,nombre);  //se busca en el hashmap
            juegos=buscar->value;
            printf("¿cuantos datos desea modificar?: ");
            scanf("%d",&datos);
            strcpy(nuevo->nombre,juegos->nombre);
            strcpy(nuevo->ano_salida,juegos->ano_salida);  //se guarda para una nueva estructura que se ingresará una vez se modifiquen los datos
            nuevo->valoracion=juegos->valoracion;
            nuevo->precio=juegos->precio;
            for (int i=0;i<datos;i++){ //for hasta que llegue a la cantidad de datos que el usuario eligió modificar
                printf("1. nombre del juego, 2. fecha de salida, 3. valoracion, 4. precio\n");
                printf("ingrese su respuesta para modificar el dato %d: ",i+1);
                scanf("%d",&op);
                if (op==1){
                    printf("ingrese nuevo nombre: ");
                    fflush(stdin);
                    scanf("%50[^\n]s",nombre);
                    strcpy(nuevo->nombre,nombre);
                }
                if (op==2){
                    printf("ingrese nuevo año: ");
                    fflush(stdin);                                 //se ingresa en estructura "nuevo"
                    scanf("%50[^\n]s",ano);
                    strcpy(nuevo->ano_salida,ano);
                }
                if (op==3){
                    printf("ingrese nueva valoración: ");
                    scanf("%d",&valoracion);
                    nuevo->valoracion=valoracion;
                }
                if (op==4){
                    printf("ingrese nuevo precio: ");
                    scanf("%d",&precio);
                    nuevo->precio=precio;
                }
            }
        }
        if (opcion==2){
            printf("ingrese nombre del juego a eliminar: ");
            fflush(stdin);
            scanf("%50[^\n]s",nombre);                        //si esque elige eliminar
            Pair *buscar=searchMap(mapajuegos,nombre);
            juegos=buscar->value;
        }
        eraseMap(mapajuegos,juegos->nombre);
        eraseTreeMap(map,&juegos->precio);            //se elimina de todas partes
        eraseTreeMap(mapval,&juegos->valoracion);
        juegoslista=firstList(listajuegos);
        while (juegoslista!=NULL){
            if (strcmp(juegos->nombre,juegoslista->nombre)==0){
                popCurrent(listajuegos);                 //para cuando llegue al nodo que contiene el nombre del juego a eliminar
                break;                         //se termina el while
            }
            juegoslista=nextList(listajuegos);
        }
        if (opcion==1){                             //después de eliminar, si se habia elegido la opcion 1, se insertará todo nuevamente en los mapas y lista
            pushBack(listajuegos,nuevo);
            insertTreeMap(map,&nuevo->precio,nuevo);
            insertTreeMap(mapval,&nuevo->valoracion,nuevo);
            insertMap(mapajuegos,nuevo->nombre,nuevo);
        }
    }
}
char *comparacion_ano(char ano[]){               //función para unicamente extraer el año
    char anoaux[50],extraida[50]="",*extraidaux;
    extraidaux=(char*)malloc(50*sizeof(char*));
    strcpy(anoaux,ano);    //se copia la fecha ingresada en anoaux
    int cont,inicio,i,cantc;
    cont=0;    //para contar los "/ /
    inicio=0;   //para sacar la cantidad de caracteres de los caracteres antes del año
    for (i=0;i<sizeof(anoaux);i++){         //el sizeof es el tamaño de la fecha
        if (ano[i]=='/'){    //si se encuentra un "/", se aumenta el contador
            cont++;
            if (cont==2){  //cuando se llegue a los 2 "/", termina el ciclo for
                i++;      //se aumenta uno al i cuando ya se leyeron los 2 "/" ya que en teoría deberia de contar desde el 1
                break;
            }
        }
    }
    inicio=i;  //se guarda en la variable inicio
    cantc=sizeof(anoaux)-i;  //el sizeof de anoaux es el size de toda la fecha y se le resta el i (o inicio, es lo mismo)
    strncpy(extraida,ano+inicio,cantc);  //cantc son la cantidad de carácteres del año, ano+inicio es el char de la fecha completa MAS el "inicio"
    extraidaux=extraida;               //por lo cual con strncpy paso el nuevo char, osea el puro año y lo meto en "extraidaux"
    return extraidaux;   //se retorna el año solo
}
void exportar_propiedades(List *listajuegos){
    char archivo[50];
    videojuego *juegos=(videojuego*)malloc(sizeof(videojuego));
    FILE *fpt;
    printf("seleccione nombre del archivo a exportar: ");         //se lee el archivo y se abre para imprimir los datos nuevos
    fflush(stdin);
    scanf("%50[^\n]s",archivo);
    fpt=fopen(archivo,"w+");
    fprintf(fpt,"Nombre,año de salida,valoracion,precio\n");
    juegos=firstList(listajuegos);
    while (juegos!=NULL){
        fprintf(fpt,"%s,",juegos->nombre);
        fprintf(fpt,"%s,",juegos->ano_salida);                 //se reorre la lista de juegos
        fprintf(fpt,"%d,",juegos->valoracion);
        fprintf(fpt,"%d\n",juegos->precio);
        juegos=nextList(listajuegos);
    }
    fclose(fpt);      //se cierra el archivo, osea queda exportrado
}
