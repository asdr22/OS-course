#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 
#include "db_warehouse.h" 
#include "concurrent.h"


pthread_mutex_t w_mutex; /* mutex global de escritura */
pthread_mutex_t r_mutex; /* mutex global de lectura */
pthread_cond_t condition; /* variable condicional global */
int READERS = 0; /* contador de lectores globales  */

/* struct de producto */
typedef struct {
	pthread_mutex_t p_w_mutex; /* mutex global de escritura */
	pthread_mutex_t p_r_mutex; /* mutex global de lectura */
	int p_readers; /* contador de lectores locales  */
} pdata;

int DEBUG = 0;

void 
__DEBUG(char* msg, int nth) {
	if(DEBUG !=1) {
		return;
	}

	printf("%s::%d\n", msg, READERS);
}



void 
GLOBAL_LOCK() {
	pthread_mutex_lock(&w_mutex); /* Bloquea escrituras */
	__DEBUG("->", 1);
}

void 
GLOBAL_UNLOCK() {
	__DEBUG("<-", 1);
	pthread_mutex_unlock(&w_mutex); /* Desbloque escrituras */
}

int concurrent_init(){
	/* Inicializa todas las variables globales que manejan concurrencia */
	pthread_mutex_init(&w_mutex, NULL);
	pthread_mutex_init(&r_mutex, NULL);
	pthread_cond_init(&condition, NULL);
	__DEBUG("DB INITIATED\n", 1);
	return db_warehouse_init();
 
}

int concurrent_destroy(){
	/* Destruyen todas las variables globales que manejan concurrencia */
	pthread_mutex_destroy(&w_mutex);
	pthread_mutex_destroy(&r_mutex);
	pthread_cond_destroy(&condition);
	__DEBUG("DB DESTROYED", 1);
	return db_warehouse_destroy(); 
}

int concurrent_create_product(char *product_name){

	GLOBAL_LOCK();
	__DEBUG("->", 1);

	pthread_mutex_lock(&w_mutex); /* Bloquea escrituras */
	printf("	-> %d\n", READERS);

	int ret = db_warehouse_exists_product(product_name);	
	if (ret == 0){
		ret = db_warehouse_create_product(product_name);
		if (ret == 0){
			/* 
			 * Generate internal data to set
			 */ 
			int size = sizeof(pdata);
			pdata *data = (pdata*)malloc(size); /* Malloc de la estructura */
			pthread_mutex_init(&data->p_w_mutex, NULL); /* init de mutexes */
			pthread_mutex_init(&data->p_r_mutex, NULL);
			data->p_readers = 0;
			__DEBUG("PRODUCT CREATED", 2);
			ret = db_warehouse_set_internal_data(product_name, data, size);
		}
	}

	__DEBUG("<-", 1);
	GLOBAL_UNLOCK();

	printf("	<- %d\n", READERS);
	pthread_mutex_unlock(&w_mutex); /* Desbloque escrituras */

	return ret;
}

int concurrent_get_num_products(int *num_products){
	/* MAX_READERS*/
	
	int num_products_aux = 0;
	


	pthread_mutex_lock(&r_mutex); /* Operaciones atomicas */
	while(READERS >= MAX_READERS) { /* Si hay mas lectores que el maximo, se espera */
		pthread_cond_wait(&condition, &r_mutex);
	}
	++READERS;
	if(READERS==1) { /* Si es el primer lector, bloquea escritores */
		pthread_mutex_lock(&w_mutex); /* Prioridad de lectores sobre escritores */
		__DEBUG("->", 1);
		__DEBUG("WRITERS LOCKED", 2);
	}

	pthread_mutex_unlock(&r_mutex);	 /* Operaciones atomicas */
	
	// Obtain number of products from DB using the given library

	__DEBUG("=>", 1);
	int ret = db_warehouse_get_num_products(&num_products_aux);
	__DEBUG("<=", 1);

	*num_products = num_products_aux;
	__DEBUG("NUM PRODUCTS CALLED\n", 1);



	pthread_mutex_lock(&r_mutex); /* Operaciones atomicas */
	--READERS;

	if(READERS==0) {
		__DEBUG(" %d\n", 1);
		pthread_mutex_unlock(&w_mutex);
		__DEBUG("WRITERS UNLOCKED", 1);
	}
	pthread_mutex_unlock(&r_mutex); /* Operaciones atomicas */
	pthread_cond_signal(&condition);

	return ret;
}

int concurrent_delete_product(char *product_name){

	int ret, size;
	void *st_int;
	

 	pthread_mutex_lock(&w_mutex); /* Bloquea escrituras */

	__DEBUG("->", 1);

	printf("	-> %d\n", READERS);



	// Read internal data for reset
	ret = db_warehouse_get_internal_data(product_name, &st_int, &size);


	if (ret == 0){

		pthread_mutex_destroy(&((pdata *)st_int)->p_w_mutex); /* Destroy de mutexes */
		pthread_mutex_destroy(&((pdata *)st_int)->p_r_mutex);
		ret = db_warehouse_set_internal_data(product_name, st_int, size); 


		st_int = NULL;
		size = 0;
		ret = db_warehouse_set_internal_data(product_name, st_int, size); 
		free(st_int); 
	
	}
	// Delete product from DB using the given library
	ret = db_warehouse_delete_product(product_name);

	__DEBUG("<-", 1);
	/*free(st_int);*/

	printf("	<- %d\n", READERS);

 	pthread_mutex_unlock(&w_mutex); /* Desloquea escrituras */
	return ret;
}

int concurrent_increment_stock(char *product_name, int stock, int *updated_stock){
	int ret, size;
	void *st_int;

	int stock_aux=0;
	
	/*
	 *  Complete 	
	 */

	pthread_mutex_lock(&r_mutex); /* Operaciones atomicas */
	while(READERS >= MAX_READERS) { /* Si hay mas lectores que el maximo, se espera */
		pthread_cond_wait(&condition, &r_mutex);
	}
	++READERS;
	if(READERS==1) { /* Si es el primer lector, bloquea escritores */
		pthread_mutex_lock(&w_mutex); /* Prioridad de lectores sobre escritores */
		__DEBUG("->", 1);
		__DEBUG("WRITERS LOCKED", 2);
	}

	__DEBUG("NOW WE HAVE CONCURRENT READERS", 2);
	pthread_mutex_unlock(&r_mutex); /* Operaciones atomicas */


	// Obtain internal data to work with them
	ret = db_warehouse_get_internal_data(product_name, &st_int, &size);
	if (ret == 0){


		pthread_mutex_lock(&((pdata *)st_int)->p_w_mutex);
	

		// Obtain current stock of the product using the given library
		ret = db_warehouse_get_stock(product_name, &stock_aux);

		// Increment stock
		stock_aux += stock;

		// Update stock of the product using the given library
		ret = db_warehouse_update_stock(product_name, stock_aux);
		*updated_stock = stock_aux;

		pthread_mutex_unlock(&((pdata *)st_int)->p_w_mutex);

		
	}


	pthread_mutex_lock(&r_mutex); /* Operaciones atomicas */
	--READERS;
	/* NUMPRODUCTS??!"?·!"·?!"· */

	if(READERS==0) {
		__DEBUG("<-", 1);
		pthread_mutex_unlock(&w_mutex);
		__DEBUG("WRITERS UNLOCKED", 2);
	}
	pthread_mutex_unlock(&r_mutex);
	pthread_cond_signal(&condition); /* Operaciones atomicas */


	return ret;
}

int concurrent_decrement_stock(char *product_name, int stock, int *updated_stock){
	int ret, size;
	void *st_int;

	int stock_aux=0;
	
	/*
	 *  Complete 	
	 */

	pthread_mutex_lock(&r_mutex); /* Operaciones atomicas */
	while(READERS >= MAX_READERS) { /* Si hay mas lectores que el maximo, se espera */
		pthread_cond_wait(&condition, &r_mutex);
	}
	++READERS;
	if(READERS==1) { /* Si es el primer lector, bloquea escritores */
		pthread_mutex_lock(&w_mutex); /* Prioridad de lectores sobre escritores */
		__DEBUG("->", 1);
		__DEBUG("WRITERS LOCKED", 2);
	}

	__DEBUG("NOW WE HAVE CONCURRENT READERS", 2);
	pthread_mutex_unlock(&r_mutex); /* Operaciones atomicas */

	// Obtain internal data to work with them
	ret = db_warehouse_get_internal_data(product_name, &st_int, &size);
	if (ret == 0){ 


		pthread_mutex_lock(&((pdata *)st_int)->p_w_mutex);
	

		// Obtain current stock of the product using the given library
		ret = db_warehouse_get_stock(product_name, &stock_aux);

		// Increment stock
		stock_aux -= stock;

		// Update stock of the product using the given library
		ret = db_warehouse_update_stock(product_name, stock_aux);
		*updated_stock = stock_aux;

		pthread_mutex_unlock(&((pdata *)st_int)->p_w_mutex);

		
	}

	pthread_mutex_lock(&r_mutex); /* Operaciones atomicas */
	--READERS;
	/* NUMPRODUCTS??!"?·!"·?!"· */

	if(READERS==0) {
		__DEBUG("<-", 1);
		pthread_mutex_unlock(&w_mutex);
		__DEBUG("WRITERS UNLOCKED", 1);
	}
	pthread_mutex_unlock(&r_mutex);
	pthread_cond_signal(&condition); /* Operaciones atomicas */


	return ret;
}

int concurrent_get_stock(char *product_name, int *stock){
    int stock_aux=0;
	int ret, size;
	void *st_int;

	/*
	 *  Complete 	
	 */

	pthread_mutex_lock(&r_mutex); /* Operaciones atomicas */
	while(READERS >= MAX_READERS) { /* Si hay mas lectores que el maximo, se espera */
		pthread_cond_wait(&condition, &r_mutex);
	}
	++READERS;
	if(READERS==1) { /* Si es el primer lector, bloquea escritores */
		pthread_mutex_lock(&w_mutex); /* Prioridad de lectores sobre escritores */
		__DEBUG("->", 1);
		__DEBUG("WRITERS LOCKED", 2);
	}
	__DEBUG("NOW WE HAVE %d CONCURRENT READERS", 1);
	pthread_mutex_unlock(&r_mutex); /* Operaciones atomicas */
	




	// Obtain internal data to work with them
	ret = db_warehouse_get_internal_data(product_name, &st_int, &size);

	if (ret == 0){ /* Controlar escrituras a nivel local */
		pdata *reference = (pdata*)st_int;

		pthread_mutex_lock(&reference->p_r_mutex); /* Operaciones atomicas */
		reference->p_readers++;
		if(reference->p_readers==1) { /* bloqueamos escrituras*/
			pthread_mutex_lock(&reference->p_w_mutex);
		}
		pthread_mutex_unlock(&reference->p_r_mutex); /* Operaciones atomicas */


		// Obtain current stock from the product using the given library
		ret = db_warehouse_get_stock(product_name, &stock_aux);
		*stock = stock_aux;

		
		pthread_mutex_lock(&reference->p_r_mutex); /* Operaciones atomicas */
		reference->p_readers--;
		if(reference->p_readers==0) {
			pthread_mutex_unlock(&reference->p_w_mutex); 
		}
		pthread_mutex_unlock(&reference->p_r_mutex); /* Operaciones atomicas */
	}



	pthread_mutex_lock(&r_mutex); /* Operaciones atomicas */
	--READERS;
	/* NUMPRODUCTS??!"?·!"·?!"· */
	if(READERS==0) {
		__DEBUG("<-", 1);    
		pthread_mutex_unlock(&w_mutex);
		__DEBUG("WRITERS UNLOCKED", 2);
	}
	pthread_mutex_unlock(&r_mutex);
	pthread_cond_signal(&condition);  /* Operaciones atomicas */
    return ret;
}
