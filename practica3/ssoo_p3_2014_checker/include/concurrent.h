#ifndef _CONCURRENT_H_
#define _CONCURRENT_H_

/*
 * Incluir el resto de "includes" necesarios
 */
#include "db_warehouse.h"
#define MAX_READERS 5

/*
 *  Descripcion: funcion que inicializa los recursos utilizados en 
 *  la biblioteca, asi como la base de datos utilizada. Usar solo una 
 *  vez al inicio.
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int concurrent_init();

/*
 *  Descripcion: funcion que elimina los recursos utilizados en la 
 *  biblioteca, asi como los utilizados en la base de datos. Usar solo 
 *  una vez al final del programa.
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int concurrent_destroy();

/*
 *  Descripcion: funcion que crea un producto en la base de 
 *  datos utilizando gestion de la concurrencia. En caso de encontrar
 *  otro producto con el mismo nombre, se considera que la creación se
 *  ha realizado con éxito.
 *  Debe mantener la coherencia de la BD. No permite realizar otras 
 *  operaciones al mismo tiempo. 
 *  Entrada: nombre del producto
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int concurrent_create_product(char *product_name);


 /*
 *  Descripcion: funcion que consulta en la base de datos el numero 
 *  de productos existente utilizando gestion de la concurrencia. 
 *  Debe mantener la coherencia de la BD. Permite realizar otras
 *  operaciones de consulta al mismo tiempo.
 *  Salida: devuelve el número de productos existentes en el almacen.
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int concurrent_get_num_products(int *num_products);


/*
 *  Descripcion: funcion que borra un producto en la base de 
 *  datos utilizando gestión de la concurrencia. En caso de no encontrarse
 *  un producto con el mismo nombre, se considera que el borrado se
 *  ha realizado con éxito.
 *  Debe mantener la coherencia de la BD. No permite realizar otras 
 *  operaciones al mismo tiempo.
 *  Entrada: nombre del producto
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int concurrent_delete_product(char *product_name);

/*
 * Descripcion: funcion que incrementa el stock de un producto y devuelve 
 * el stock actualizado. 
 * Debe permitir actualizar o leer otros productos en paralelo.
 * Entrada: nombre del producto y el stock a incrementar
 * Salida: devuelve el stock actualizado
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int concurrent_increment_stock(char *product_name, int stock, int *updated_stock);


/*
 * Descripcion: funcion que decrementa el stock de un producto y devuelve 
 * el stock actualizado
 * Debe permitir actualizar o leer otros productos en paralelo.
 * Entrada: nombre del producto y el stock a decrementar
 * Salida: devuelve el stock actualizado
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int concurrent_decrement_stock(char *product_name, int stock, int *updated_stock);


/*
 * Descripcion: funcion que devuelve el stock de un producto
 * Debe permitir actualizar o leer otros productoss en paralelo.
 * Entrada: nombre del producto
 * Salida: devuelve el stock actualizado
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int concurrent_get_stock(char *product_name, int *stock);

#endif// _CONCURRENT_H_
