#ifndef _SECUENCIAL_H_
#define _SECUENCIAL_H_

#include "db_warehouse.h"

/*
 *  Descripcion: funcion que inicializa los recursos utilizados en 
 *  la biblioteca, asi como la base de datos utilizada. Usar solo una 
 *  vez al inicio.
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int sequential_init();


/*
 *  Descripcion: funcion que elimina los recursos utilizados en la 
 *  biblioteca, asi como los utilizados en la base de datos. Usar solo 
 *  una vez al final del programa.
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int sequential_destroy();


/*
 *  Descripcion: funcion que crea un producto en la base de 
 *  datos. En caso de encontrar otro producto con el mismo nombre, 
 *  se considera que la creacion se ha realizado con éxito.
 *  Entrada: nombre del producto
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int sequential_create_product(char *product_name);


 /*
 *  Descripcion: funcion que consulta en la base de datos el numero 
 *  de productos activos en el almacen. 
 *  Salida: devuelve el número de productos activos en el almacen.
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int sequential_get_num_products(int *num_products);


/*
 *  Descripcion: funcion que borra un producto de la base de datos. 
 *  En caso de no encontrarse un producto con el mismo nombre, 
 *  se considera que el borrado se ha realizado con exito.
 *  Entrada: nombre del producto
 *  Valor de retorno: 0 -> ok, -1 -> error
 */
int sequential_delete_product(char *product_name);

/*
 * Descripcion: funcion que incrementa el stock de un producto y devuelve 
 * el stock actualizado. 
 * Entrada: nombre del producto y el stock a incrementar
 * Salida: devuelve el stock actualizado
 * Valor de retorno: 0 -> ok, -1 -> error (producto no encontrado, etc.)
 */
int sequential_increment_stock(char *product_name, int stock, int *updated_stock);


/*
 * Descripcion: funcion que decrementa el stock de un producto y devuelve 
 * el stock actualizado.
 * Entrada: nombre del producto y el stock a decrementar
 * Salida: devuelve el stock actualizado
 * Valor de retorno: 0 -> ok, -1 -> error (producto no encontrado, etc.)
 */
int sequential_decrement_stock(char *product_name, int stock, int *updated_stock);


/*
 * Descripcion: funcion que devuelve el stock de un producto.
 * Entrada: nombre del producto
 * Salida: devuelve el stock actualizado
 * Valor de retorno: 0 -> ok, -1 -> error (producto no encontrado, etc.)
 */
int sequential_get_stock(char *product_name, int *stock);

#endif// _CONCURRENTE_H_
