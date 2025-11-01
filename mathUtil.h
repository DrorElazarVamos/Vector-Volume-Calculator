#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define PI 3.1415
// --- Data Structures ---

typedef struct {
    double direction[3];
    double magnitude;
} vector;

typedef struct {
    vector *vectors; // Dynamic array of vectors
    size_t count;    // Number of vectors in the array
} VectorList;


// --- Function Prototypes ---

/**
 * @brief calculate the scalaric product of 2 given vectors
 * @param v1 vector 1
 * @param v2 vector 2
 * @return scalaric product of the 2 vectors
 */
double scalaricProduct(vector v1, vector v2);

/**
 * @brief calculate the cross product of 2 given vectors (the orthogonal vector).
 * @param v1 vector 1
 * @param v2 vector 2
 * @return cross product of the 2 vectors
 */
vector crossProduct(vector v1, vector v2);

/**
 * @brief calculate the volume of a parallelepiped (Scalar Triple Product).
 * @param vectors[] the three direction vectors of the parallelepiped
 * @param k is the constant for the shape
 * @return volume
 */
double volumeParallelepiped(vector vectors[], double k);

/**
 * @brief Frees the dynamically allocated memory used by the VectorList.
 * @param list The VectorList to clean up.
 */
void free_vector_list(VectorList *list);

#endif // MATH_UTIL_H