/*
 * This is the file in which you will implement the functionality required by
 * the assignment.  Make sure to add your name and @oregonstate.edu email
 * address below:
 *
 * Name: Adam Snebold
 * Email: snebolda@oregonstate.edu
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "products.h"
#include "dynarray.h"

/*
 * This function should allocate and initialize a single product struct with
 * name, inventory, and price data.
 *
 * Params:
 *   name - a string containing the product's name.  Note that you'll have to
 *     make a copy of this string to store in the product struct instead of
 *     storing this string directly.
 *   inventory - an integer for how many of this product in the store
 *   price - the product's price
 *
 * Return:
 *   Should return a newly-allocated product structure whose fields are
 *   initialized with the values provided.
 */
struct product* create_product(char* name, int inventory, float price) {
    struct product* new_product = malloc(sizeof(struct product));
    if (!new_product) return NULL;

    // Allocate memory for the name and copy it
    new_product->name = malloc(strlen(name) + 1);
    if (!new_product->name) {
        free(new_product);
        return NULL;
    }

    strcpy(new_product->name, name);
    new_product->inventory = inventory;
    new_product->price = price;

    return new_product;
}

/*
 * This function should free all memory allocated by create_product() for a
 * single product.  This function must free all relevant memory and cannot
 * result in a memory leak.
 *
 * Params:
 *   product - a pointer to the product whose memory should be freed.  This
 *     function must free any memory allocated to the fields of this struct
 *     as well as memory allocated for the struct itself.
 */
void free_product(struct product* product) {
    if (!product) return;

    // Free allocated name and struct itself
    free(product->name);
    free(product);
}

/*
 * This function should create a struct product for each product represented
 * in the information provided in the function arguments, and it should store
 * those products in a dynamic array (i.e. struct dynarray) allocated by this
 * function.
 *
 * Params:
 *   num_products - the number of products to be stored in the newly allocated
 *     dynamic array.
 *   names - an array of product names.
 *   inventory - an array of product inventories.
 *   prices - an array of product prices.
 *
 * Return:
 *   Should return a pointer to the newly allocated dynamic array containing
 *   newly-created product structs.
 */
struct dynarray* create_product_array(int num_products, char** names, int* inventory, float* prices) {
    struct dynarray* products = dynarray_create();
    if (!products) return NULL;

    for (int i = 0; i < num_products; i++) {
        struct product* new_product = create_product(names[i], inventory[i], prices[i]);
        if (!new_product) continue;
        dynarray_insert(products, -1, new_product); // Insert at end of array
    }

    return products;
}

/*
 * This function should free all of the memory allocated to a dynamic array of
 * product structs, including the memory allocated to the array itself as
 * well as any memory allocated to the individual product structs.
 *
 * Params:
 *   products - a pointer to the dynamic array of product structs whose memory
 *     is to be freed.
 */
void free_product_array(struct dynarray* products) {
    if (!products) return;

    int length = dynarray_length(products);
    for (int i = 0; i < length; i++) {
        free_product(dynarray_get(products, i)); // Free each product
    }

    dynarray_free(products); // Free the dynamic array structure
}

/*
 * This function should print the name, inventory, and price of products in an
 * array, one product per line.
 *
 * Params:
 *   products - the dynamic array of products to be printed.
 */
void print_products(struct dynarray* products) {
    if (!products) return;

    int length = dynarray_length(products);
    for (int i = 0; i < length; i++) {
        struct product* p = dynarray_get(products, i);
        printf("Name: %s, Inventory: %d, Price: %.2f\n", p->name, p->inventory, p->price);
    }
}

/*
 * This function should return a pointer to the product in a given array with
 * the highest price.
 *
 * Params:
 *   products - the array from which to find the product with the highest price.
 *
 * Return:
 *   Should return a pointer to the product in the array with the highest price.
 */
struct product* find_max_price(struct dynarray* products) {
    if (!products || dynarray_length(products) == 0) return NULL;

    struct product* max_product = dynarray_get(products, 0);
    for (int i = 1; i < dynarray_length(products); i++) {
        struct product* p = dynarray_get(products, i);
        if (p->price > max_product->price) {
            max_product = p;
        }
    }

    return max_product;
}

/*
 * This function should return a pointer to the product in a given array with
 * the largest investment, defined as the product of inventory and price.
 *
 * Params:
 *   products - the array from which to find the product with the highest investment.
 *
 * Return:
 *   Should return a pointer to the product in the array with the highest investment.
 */
struct product* find_max_investment(struct dynarray* products) {
    if (!products || dynarray_length(products) == 0) return NULL;

    struct product* max_product = dynarray_get(products, 0);
    float max_investment = max_product->price * max_product->inventory;

    for (int i = 1; i < dynarray_length(products); i++) {
        struct product* p = dynarray_get(products, i);
        float investment = p->price * p->inventory;
        if (investment > max_investment) {
            max_product = p;
            max_investment = investment;
        }
    }

    return max_product;
}

/*
 * This function should sort the products stored in a dynamic array by
 * ascending inventory (i.e. lowest inventory at the beginning of the array).
 * You may implement any sorting algorithm you want.
 *
 * Params:
 *   products - the dynamic array of products to be sorted.
 */
void sort_by_inventory(struct dynarray* products) {
    if (!products || dynarray_length(products) <= 1) return;

    int length = dynarray_length(products);

    for (int i = 0; i < length - 1; i++) {
        int min_index = i;

        for (int j = i + 1; j < length; j++) {
            struct product* p1 = dynarray_get(products, min_index);
            struct product* p2 = dynarray_get(products, j);

            // If the current product has a lower inventory, update min_index
            if (p2->inventory < p1->inventory) {
                min_index = j;
            }
        }

        // Swap products if needed
        if (min_index != i) {
            struct product* temp = dynarray_get(products, i);
            dynarray_set(products, i, dynarray_get(products, min_index));
            dynarray_set(products, min_index, temp);
        }
    }
}                         
