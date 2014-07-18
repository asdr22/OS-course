#include <stdio.h>
#include <pthread.h>
#include "concurrent.h"


void *Thread2 (void *arg) {
	int stock = 100, updated_stock = 0;
	int num_products;
	char *product_name= "product1";

	printf ("%s\n", (char *)arg);
	concurrent_get_num_products(&num_products);
	concurrent_create_product(product_name);	
	concurrent_increment_stock(product_name, stock, &updated_stock);	
	concurrent_increment_stock(product_name, stock, &updated_stock);	
	concurrent_increment_stock(product_name, stock, &updated_stock);	
	concurrent_increment_stock(product_name, stock, &updated_stock);	
	concurrent_get_stock(product_name, &stock);

	pthread_exit (NULL);
} /* Fin de Hilo */



void *Thread1 (void *arg) {
	int stock = 100, updated_stock = 0;
	char *product_name= "product1";
	int num_products;

	printf ("%s\n", (char *)arg);
	concurrent_get_num_products(&num_products);
	concurrent_create_product(product_name);
	concurrent_increment_stock(product_name, stock, &updated_stock);
	concurrent_decrement_stock(product_name, stock, &updated_stock);	
	concurrent_get_stock(product_name, &stock);

	pthread_exit (NULL);
} /* End thread */




#define MAX 20
int main(){
	int i = 0;
	pthread_t th1[MAX];
	pthread_t th2;
	void *st1;

	concurrent_init();
	
	for (i=0;i<MAX;i++)
		pthread_create (&th1[i], NULL, Thread1, "Thread 1");
		pthread_create (&th2, NULL, Thread2, "Thread 2");
	for (i=0;i<MAX;i++)
		pthread_join (th1[i], (void **) &st1);
		pthread_join (th2, (void **) &st1);
	printf ("Thread return: %d\n", (int)((long)st1));	

	concurrent_destroy();

	return 0;
}
