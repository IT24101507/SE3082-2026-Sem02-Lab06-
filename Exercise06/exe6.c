#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000
#define STRIP 8   

int main() {
    double *A = malloc(N * sizeof(double));
    double *B = malloc(N * sizeof(double));
    double *C = malloc(N * sizeof(double));

    for (int i = 0; i < N; i++) {
        A[i] = i * 1.0;
        B[i] = (N - i) * 1.0;
    }

    double tstart, tstop, tcalc;
    tstart = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int strip_start = 0; strip_start < N; strip_start += STRIP) {
        int strip_end = strip_start + STRIP;
        if (strip_end > N) strip_end = N;

    
        #pragma omp simd
        for (int i = strip_start; i < strip_end; i++) {
            C[i] = A[i] * B[i];
        }
    }

    tstop = omp_get_wtime();
    tcalc = tstop - tstart;

    
    int ok = 1;
    for (int i = 0; i < N; i++) {
        if (C[i] != A[i] * B[i]) { ok = 0; break; }
    }

    printf("Correct: %s\n", ok ? "yes" : "NO");
    printf("Time: %f seconds\n", tcalc);
    printf("Sample: C[100] = %f (expected %f)\n", C[100], A[100]*B[100]);

    free(A); free(B); free(C);
    return 0;
}