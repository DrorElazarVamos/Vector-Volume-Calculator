#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mathUtil.h"
#include "csvHandler.h"
#include "testerFile.h"

// --- Forward Declarations ---
void display_main_menu(void);
void clear_screen(void);
void pause_screen(void);
int get_user_choice(void);
void run_tester_module(void);
void run_single_calculation(void);
void display_about(void);
void vector_operation(void);
void volume_calculation(void);

// --- Main Function ---
int main() {
    int choice;
    bool running = true;

    while (running) {
        clear_screen();
        display_main_menu();
        choice = get_user_choice();

        switch (choice) {
            case 1:
                run_tester_module();
                break;
            case 2:
                run_single_calculation();
                break;
            case 3:
                display_about();
                break;
            case 4:
                printf("\nThank you for using my calculator!\n");
                running = false;
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
                pause_screen();
        }
    }

    return 0;
}

// --- UI Helper Functions ---

void display_main_menu(void) {
    printf("=============================================\n");
    printf("||   Advanced Math Calculator - Main Menu  ||\n");
    printf("=============================================\n");
    printf("|                                            |\n");
    printf("|  1. Run Automated Test Suite               |\n");
    printf("|  2. Calculations                           |\n");
    printf("|  3. About                                  |\n");
    printf("|  4. Exit                                   |\n");
    printf("|                                            |\n");
    printf("=============================================\n");
    printf("\nEnter your choice (1-4): ");
}

void clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause_screen(void) {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // Clear input buffer
    getchar(); // Wait for Enter
}

int get_user_choice(void) {
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n'); // Clear invalid input
        return -1;
    }
    while (getchar() != '\n'); // Clear remaining input
    return choice;
}

// --- Module: Automated Test Suite ---

void run_tester_module(void) {
    char filename[256];
    int test_choice;
    
    clear_screen();
    printf("=============================================\n");
    printf("|        Automated Test Suite Runner        |\n");
    printf("=============================================\n\n");
    
    printf("Enter the CSV test file path: ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        fprintf(stderr, "Error reading filename.\n");
        pause_screen();
        return;
    }
    
    size_t len = strcspn(filename, "\n");
    if (len >= sizeof(filename) - 1) {
        fprintf(stderr, "Error: Filename too long (max %zu characters)\n", sizeof(filename) - 2);
        pause_screen();
        return;
    }
    filename[len] = '\0';
    
    if (len == 0) {
        fprintf(stderr, "Error: Empty filename\n");
        pause_screen();
        return;
    }

    CsvFile *csv = csv_open(filename);
    if (csv == NULL) {
        printf("\nError: Could not open file '%s'\n", filename);
        printf("Please check:\n");
        printf("  - File exists in current directory\n");
        printf("  - File path is correct\n");
        printf("  - You have read permissions\n");
        pause_screen();
        return;
    }
    
    if (!csv_read_line(csv)) {
        printf("Error: File is empty or could not read header.\n");
        csv_close(csv);
        pause_screen();
        return;
    }

    printf("\n--- Test Options ---\n");
    printf("1. Test Parallelepiped Volume (k=1)\n");
    printf("2. Test Pyramid Volume (k=6)\n");
    printf("3. Test Cross Product\n");
    printf("4. Test Scalar Product\n");
    printf("5. Run All Tests\n");
    printf("0. Cancel and Return to Main Menu\n");
    printf("\nEnter choice (0-5): ");
    
    if (scanf("%d", &test_choice) != 1) {
        fprintf(stderr, "Invalid input.\n");
        csv_close(csv);
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    while (getchar() != '\n');

    if (test_choice == 0) {
        csv_close(csv);
        return;
    }

    printf("\n");
    switch(test_choice) {
        case 1:
            run_volume_tests(csv, volumeParallelepiped, "Parallelepiped Volume", 1.0);
            break;
        case 2:
            run_volume_tests(csv, volumeParallelepiped, "Pyramid Volume", 6.0);
            break;
        case 3:
            run_cross_product_tests(csv, crossProduct);
            break;
        case 4:
            run_scalar_product_tests(csv, scalaricProduct);
            break;
        case 5:
            run_volume_tests(csv, volumeParallelepiped, "Parallelepiped Volume", 1.0);
            run_volume_tests(csv, volumeParallelepiped, "Pyramid Volume", 6.0);
            run_cross_product_tests(csv, crossProduct);
            run_scalar_product_tests(csv, scalaricProduct);
            break;
        default:
            printf("Invalid choice.\n");
    }
    
    csv_close(csv);
    pause_screen();
}

// --- Module: Vector Operation ---
void vector_operation(void) {
    vector *vectorsArg = NULL;
    int operation;
    int vectorCount = 0;

    clear_screen();
    printf("=============================================\n");
    printf("|            Vector Operation                |\n");
    printf("=============================================\n\n");
    printf("|                                            |\n");
    printf("|  1. Cross Product                          |\n");
    printf("|  2. Scalar Product                         |\n");
    printf("|  3. Addition/Subtraction                   |\n");
    printf("|  4. Get Normal Vector                      |\n");
    printf("|  5. Check if Vector is in Shape            |\n");
    printf("|  0. Cancel                                 |\n");
    printf("|                                            |\n");
    printf("=============================================\n");
    printf("\nEnter your choice (0-5): ");

    if (scanf("%d", &operation) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    while (getchar() != '\n');

    // Determine required vector count based on operation
    switch (operation) {
        case 1: // Cross Product
        case 2: // Scalar Product
            vectorCount = 2;
            break;
        case 3: // Addition/Subtraction
            printf("\nEnter how many vectors you want to add/sub (min: 2, max: 10): ");
            if (scanf("%d", &vectorCount) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            if (vectorCount < 2 || vectorCount > 10) {
                printf("Invalid input. Must be between 2 and 10.\n");
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            break;
        case 4: // Get Normal Vector
            vectorCount = 1;
            break;
        case 5: // Check if vector is in shape
            printf("\nEnter how many vectors (3 for shape + at least 1 to check) (min: 4, max: 10): ");
            if (scanf("%d", &vectorCount) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            if (vectorCount < 4 || vectorCount > 10) {
                printf("Invalid input. Must be between 4 and 10.\n");
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            break;
        case 0:
            return;
        default:
            printf("Invalid choice.\n");
            pause_screen();
            return;
    }

    // Allocate memory for vectors
    vectorsArg = (vector*)malloc(vectorCount * sizeof(vector));
    if (vectorsArg == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        pause_screen();
        return;
    }

    // Read vectors from user
    while (getchar() != '\n'); // Clear buffer
    for (int i = 0; i < vectorCount; i++) {
        printf("Enter Vector %d (X Y Z): ", i + 1);
        if (scanf("%lf %lf %lf", &vectorsArg[i].direction[0], 
                  &vectorsArg[i].direction[1], &vectorsArg[i].direction[2]) != 3) {
            printf("Invalid input. Please enter three numbers separated by spaces.\n");
            while (getchar() != '\n');
            free(vectorsArg);
            pause_screen();
            return;
        }
        // Calculate magnitude
        vectorsArg[i].magnitude = sqrt(
            pow(vectorsArg[i].direction[0], 2) + 
            pow(vectorsArg[i].direction[1], 2) + 
            pow(vectorsArg[i].direction[2], 2)
        );
    }

    printf("\n--- Results ---\n");
    
    switch(operation) {
        case 1: { // Cross Product
            vector result = crossProduct(vectorsArg[0], vectorsArg[1]);
            printf("V1 × V2 = [%.6lf, %.6lf, %.6lf]\n", 
                   result.direction[0], result.direction[1], result.direction[2]);
            printf("Magnitude: %.6lf\n", result.magnitude);
            
            // Verify perpendicularity
            double dot_v1 = scalaricProduct(result, vectorsArg[0]);
            double dot_v2 = scalaricProduct(result, vectorsArg[1]);
            printf("Verification: V1·result = %.6lf, V2·result = %.6lf (should be ~0)\n", 
                   dot_v1, dot_v2);
            break;
        }
        case 2: { // Scalar Product
            double result = scalaricProduct(vectorsArg[0], vectorsArg[1]);
            printf("V1 · V2 = %.6lf\n", result);
            break;
        }
        case 3: { // Addition/Subtraction
            char operation_type;
            printf("Addition or Subtraction (+/-): ");
            while (getchar() != '\n'); // Clear buffer
            if (scanf("%c", &operation_type) != 1) {
                printf("Invalid input.\n");
                free(vectorsArg);
                while (getchar() != '\n');
                pause_screen();
                return;
            }
            
            vector result = {0};
            if (operation_type == '+') {
                for (int i = 0; i < vectorCount; i++) {
                    for (int j = 0; j < 3; j++) {
                        result.direction[j] += vectorsArg[i].direction[j];
                    }
                }
                printf("Sum: [%.6lf, %.6lf, %.6lf]\n", 
                       result.direction[0], result.direction[1], result.direction[2]);
            } else if (operation_type == '-') {
                // Start with first vector
                for (int j = 0; j < 3; j++) {
                    result.direction[j] = vectorsArg[0].direction[j];
                }
                // Subtract rest
                for (int i = 1; i < vectorCount; i++) {
                    for (int j = 0; j < 3; j++) {
                        result.direction[j] -= vectorsArg[i].direction[j];
                    }
                }
                printf("Difference: [%.6lf, %.6lf, %.6lf]\n", 
                       result.direction[0], result.direction[1], result.direction[2]);
            } else {
                printf("Invalid operation.\n");
                free(vectorsArg);
                pause_screen();
                return;
            }
            
            result.magnitude = sqrt(
                pow(result.direction[0], 2) + 
                pow(result.direction[1], 2) + 
                pow(result.direction[2], 2)
            );
            printf("Magnitude: %.6lf\n", result.magnitude);
            break;
        }
        case 4: { // Get Normal Vector (Unit Vector)
            if (vectorsArg[0].magnitude == 0) {
                printf("Cannot normalize zero vector.\n");
            } else {
                vector normal;
                for (int i = 0; i < 3; i++) {
                    normal.direction[i] = vectorsArg[0].direction[i] / vectorsArg[0].magnitude;
                }
                normal.magnitude = 1.0;
                printf("Normal (Unit) Vector: [%.6lf, %.6lf, %.6lf]\n", 
                       normal.direction[0], normal.direction[1], normal.direction[2]);
                printf("Magnitude: %.6lf\n", normal.magnitude);
            }
            break;
        }
        case 5: { // Check if vector is in parallelepiped
            // First 3 vectors define the shape, rest are points to check
            vector shape[3] = {vectorsArg[0], vectorsArg[1], vectorsArg[2]};
            double volume = volumeParallelepiped(shape, 1.0);
            
            printf("Shape Volume: %.6lf\n\n", volume);
            
            if (volume < 0.001) {
                printf("Warning: Vectors are coplanar (volume ~0)\n");
            }
            
            // Check each remaining vector
            for (int i = 3; i < vectorCount; i++) {
                printf("Checking Vector %d:\n", i + 1);
                
                // Create 3 sub-parallelepipeds
                vector test1[3] = {vectorsArg[i], vectorsArg[1], vectorsArg[2]};
                vector test2[3] = {vectorsArg[0], vectorsArg[i], vectorsArg[2]};
                vector test3[3] = {vectorsArg[0], vectorsArg[1], vectorsArg[i]};
                
                double vol1 = volumeParallelepiped(test1, 1.0);
                double vol2 = volumeParallelepiped(test2, 1.0);
                double vol3 = volumeParallelepiped(test3, 1.0);
                double total = vol1 + vol2 + vol3;
                
                printf("  Sub-volumes: %.6lf + %.6lf + %.6lf = %.6lf\n", 
                       vol1, vol2, vol3, total);
                
                if (fabs(total - volume) < 0.001) {
                    printf("  Result: Vector IS inside the parallelepiped\n\n");
                } else {
                    printf("  Result: Vector IS NOT inside the parallelepiped\n\n");
                }
            }
            break;
        }
        default:
            printf("Invalid choice.\n");
    }
    
    free(vectorsArg);
    while (getchar() != '\n'); // Clear buffer
    pause_screen();
}

// --- Module: Volume Calculation ---
void volume_calculation(void) {
    vector v1, v2, v3;
    
    clear_screen();
    printf("=============================================\n");
    printf("|         Volume Calculation Mode            |\n");
    printf("=============================================\n\n");
    
    printf("Enter Vector 1 (X Y Z): ");
    if (scanf("%lf %lf %lf", &v1.direction[0], &v1.direction[1], &v1.direction[2]) != 3) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    
    printf("Enter Vector 2 (X Y Z): ");
    if (scanf("%lf %lf %lf", &v2.direction[0], &v2.direction[1], &v2.direction[2]) != 3) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    
    printf("Enter Vector 3 (X Y Z): ");
    if (scanf("%lf %lf %lf", &v3.direction[0], &v3.direction[1], &v3.direction[2]) != 3) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    
    vector vectors[3] = {v1, v2, v3};
    double vol_parallelepiped = volumeParallelepiped(vectors, 1.0);
    double vol_pyramid = volumeParallelepiped(vectors, 6.0);
    
    printf("\n--- Results ---\n");
    printf("Parallelepiped Volume: %.6lf\n", vol_parallelepiped);
    printf("Pyramid Volume: %.6lf\n", vol_pyramid);
    
    if (vol_parallelepiped < 0.001) {
        printf("\nWarning: Vectors are coplanar (parallel or in same plane)\n");
    }
    
    while (getchar() != '\n');
    pause_screen();
}

// --- Module: Single Calculation ---
void run_single_calculation(void) {
    int calc_choice;
    
    clear_screen();
    printf("=============================================\n");
    printf("|            Calculation Mode                |\n");
    printf("=============================================\n\n");
    printf("|                                            |\n");
    printf("|  1. Volume of a Shape                      |\n");
    printf("|  2. Vector Operations                      |\n");
    printf("|  0. Cancel                                 |\n");
    printf("|                                            |\n");
    printf("=============================================\n");
    printf("\nEnter your choice (0-2): ");
    
    if (scanf("%d", &calc_choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        pause_screen();
        return;
    }
    while (getchar() != '\n');
    
    switch(calc_choice) {
        case 1:
            volume_calculation();
            break;
        case 2:
            vector_operation();
            break;
        case 0:
            return;
        default:
            printf("Invalid choice.\n");
            pause_screen();
    }
}

// --- Module: About ---
void display_about(void) {
    clear_screen();
    printf("=============================================\n");
    printf("|              About This Program           |\n");
    printf("=============================================\n\n");
    
    printf("Vector & Volume Calculator v1.1\n");
    printf("--------------------------------\n\n");
    
    printf("This program provides:\n");
    printf("  • Scalar (dot) product calculations\n");
    printf("  • Cross product calculations\n");
    printf("  • Vector addition and subtraction\n");
    printf("  • Vector normalization\n");
    printf("  • Point-in-parallelepiped testing\n");
    printf("  • Parallelepiped volume calculations\n");
    printf("  • Pyramid volume calculations\n");
    printf("  • Automated CSV-based testing\n\n");
    
    printf("Mathematical Operations:\n");
    printf("  Scalar Product:  V1 · V2 = x1*x2 + y1*y2 + z1*z2\n");
    printf("  Cross Product:   V1 × V2 = perpendicular vector\n");
    printf("  Volume:          |V1 · (V2 × V3)| / k\n");
    printf("                   where k=1 (parallelepiped)\n");
    printf("                         k=6 (pyramid)\n\n");
    
    printf("Modules:\n");
    printf("  • mathUtil: Core mathematical operations\n");
    printf("  • csvHandler: CSV file parsing\n");
    printf("  • testerFile: Automated testing framework\n");
    printf("  • main: User interface (this file)\n\n");
    
    printf("Recent Fixes (v1.1):\n");
    printf("  ✓ Fixed CSV parsing buffer corruption\n");
    printf("  ✓ Added input validation\n");
    printf("  ✓ Added coplanar vector detection\n");
    printf("  ✓ Improved error messages\n");
    printf("  ✓ Completed vector operations\n");
    printf("  ✓ Fixed memory management\n\n");
    
    pause_screen();
}