#include "mathUtil.h"

double scalaricProduct(vector v1, vector v2) {
    double scalarProd = 0.0; // init the product

    for (int i=0; i<3; i++){
        scalarProd += v1.direction[i]*v2.direction[i];
    } 

    return scalarProd;
}

vector crossProduct(vector v1, vector v2){
    vector result;
    // Cross product is calculated like a determinant.
    double x1 = v1.direction[0] ,y1 = v1.direction[1] ,z1 = v1.direction[2];
    double x2 = v2.direction[0] ,y2 = v2.direction[1] ,z2 = v2.direction[2];

    result.direction[0] = (y1 * z2) - (y2 * z1);
    result.direction[1] = (z1 * x2) - (x1 * z2);
    result.direction[2] = (x1 * y2) - (x2 * y1);

    result.magnitude = sqrt(
        pow(result.direction[0],2) + pow(result.direction[1],2) + pow(result.direction[2],2)
    );

    return result;
}

double volumeParallelepiped(vector vectors[],double k){
    vector v_cross = crossProduct(vectors[0], vectors[1]);
    double scalar_triple_product = scalaricProduct(v_cross, vectors[2]);
    
    return fabs(scalar_triple_product)/k;
}

void free_vector_list(VectorList *list) {
    if (list != NULL && list->vectors != NULL) {
        free(list->vectors);
        list->vectors = NULL;
        list->count = 0;
    }
}