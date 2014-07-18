#ifndef _DB_WAREHOUSE_
#define _DB_WAREHOUSE_

/*
 * Funcion que inicializa la base de datos. Usar solo una vez al inicio.
 * Entrada:
 * Salida:
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int db_warehouse_init();       

/*
 * Funcion que elimina los recusos utilizados de la base de datos. 
 * Usar solo una vez al finalizar.
 * Entrada:
 * Salida:
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int db_warehouse_destroy();

/*
 * Funcion que crea un nuevo producto en el primer registro disponible 
 * del almacen. No controla la existencia de otros productos con el mismo nombre.
 * Entrada: nombre del producto
 * Salida: 
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int db_warehouse_create_product(char *product_name);

/*
 * Funcion que devuelve el numero de productos activos en el almacen.
 * Salida: numero de de productos activos en el almacen.
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int db_warehouse_get_num_products(int *num_products);

/*
 * Funcion que elimina el primer producto que encuentre con el mismo 
 * nombre que el solicitado.
 * Entrada: nombre del producto
 * Salida:
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int db_warehouse_delete_product(char *product_name);

/*
 * Función que indica si existe un producto.
 * Entrada: nombre del producto a buscar.
 * Salida:
 * Valor de retorno: 1 -> existe, 0 -> No existe
 */
int db_warehouse_exists_product(char *product_name);

/*
 * Funcion que actualiza el stock de un producto
 * Entrada: nombre del producto y stock del producto
 * Salida:
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int db_warehouse_update_stock(char *product_name, int stock);

/*
 * Funcion que devuelve el stock de un producto
 * Entrada: nombre del producto
 * Salida: stock del producto
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int db_warehouse_get_stock(char *product_name, int *stock);

/*
 * Funcion que permite asociar una serie de datos a un producto concreto. 
 * Se puede utilizar para guardar con el producto datos relacionados con 
 * la sincronizacion de procesos ligeros.
 * Entrada: nombre ddel producto, puntero a los datos y tamaño de los mismos
 * Salida:
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int db_warehouse_set_internal_data(char *product_name, void *ptr, int size);

/*
 * Funcion que devuelve datos externos asociados a un producto.
 * Entrada: nombre del producto, 
 * Salida: puntero a los datos y tamaño de los mismos
 * Valor de retorno: 0 -> ok, -1 -> error
 */
int db_warehouse_get_internal_data(char *product_name, void **ptr, int *size);

#endif //_DB_WAREHOUSE_
