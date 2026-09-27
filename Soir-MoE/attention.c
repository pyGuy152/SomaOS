#include <stdio.h>
#include <math.h>

#define T 3 //tokens
#define D 2 //dimensions

void printf_matrix(int rows, int cols, float M[rows][cols]);

int main(void) {

    float X[T][D] = { //token representations
        {1.0f, 2.0f},
        {3.0f, 4.0f},
        {5.0f, 6.0f}
    };

    float W_Q[2][2] = { //query weights
        {0.1f, 0.0f},
        {0.0f, 0.1f}
    };

    float W_K[2][2] = {
        {0.1f, 0.0f},
        {0.0f, 0.1f}
    };

    float W_V[2][2] = {
        {1.0f, 0.0f},
        {2.0f, 1.0f}
    };

    float Q[3][2]; //query vectors
    float K[3][2]; //key vectors
    float V[3][2]; //value vectors; information each token gives via attention
    float S[3][3]; //attention score matrix; QK^T
    float S_scaled[3][3]; //scaled attention scores; later masked S
    float A[3][3]; //matrix of attention weights
    float O[3][2]; //attention output/context vectors; weighted sum of the value vectors



    for (size_t i = 0; i < sizeof(X) / sizeof(X[0]); i++) { //looks at rows of X
        for (size_t j = 0; j < sizeof(W_Q[0]) / sizeof(W_Q[0][0]); j++) { //looks at columns of W_Q
            float sum = 0;

            for (size_t k = 0; k < sizeof(X[0]) / sizeof(X[0][0]); k++) { //runs through columns of X
                float  value = X[i][k] * W_Q[k][j];                       //necessary to traverse X columns, W_Q rows
                sum += value;
            }
            Q[i][j] = sum;
        }
    }

    for (size_t i = 0; i < sizeof(X) / sizeof(X[0]); i++) { //K
        for (size_t j = 0; j < sizeof(W_K[0]) / sizeof(W_K[0][0]); j++) {
            float sum = 0;

            for (size_t k = 0; k < sizeof(X[0]) / sizeof(X[0][0]); k++) {
                float  value = X[i][k] * W_K[k][j];
                sum += value;
            }
            K[i][j] = sum;
        }
    }

    for (size_t i = 0; i < sizeof(X) / sizeof(X[0]); i++) { //V
        for (size_t j = 0; j < sizeof(W_V[0]) / sizeof(W_V[0][0]); j++) {
            float sum = 0;

            for (size_t k = 0; k < sizeof(X[0]) / sizeof(X[0][0]); k++) {
                float  value = X[i][k] * W_V[k][j];
                sum += value;
            }
            V[i][j] = sum;
        }
    }

    for (size_t i = 0; i < sizeof(Q) / sizeof(Q[0]); i++) { //S
        for (size_t j = 0; j < sizeof(K) / sizeof(K[0]); j++) {
            float sum = 0;

            for (size_t k = 0; k < sizeof(Q[0]) / sizeof(Q[0][0]); k++) {
                float  value = Q[i][k] * K[j][k];
                sum += value;
            }
            S[i][j] = sum;
        }
    }

    for (size_t i = 0; i < sizeof(S) / sizeof(S[0]); i++) { //A
        for (size_t j = 0; j < sizeof(S[0]) / sizeof(S[0][0]); j++) {
            float d_k = sizeof(Q[0]) / sizeof(Q[0][0]); //number of values in each Q/K vector; columns of Q/K, not S
            S_scaled[i][j] = S[i][j] / sqrtf(d_k);
        }
    }

    for (size_t i = 0; i < sizeof(S_scaled) / sizeof(S_scaled[0]); i++) { //makes sure token can only see itself and previous tokens...
        for (size_t j = 0; j < sizeof(S_scaled[0]) / sizeof(S_scaled[0][0]); j++) { //...lower left triangle of matrix; masking
            if (j > i) {
                S_scaled[i][j] = -INFINITY;
            }
        }
    }

    for (size_t i = 0; i < sizeof(S_scaled) / sizeof(S_scaled[0]); i++) { //softmax
        float max = S_scaled[i][0];
        for (size_t j = 0; j < sizeof(S_scaled[0]) / sizeof(S_scaled[0][0]); j++) {
            if (S_scaled[i][j] > max) {
                max = S_scaled[i][j];
            }
        }

        float sum = 0;
        for (size_t j = 0; j < sizeof(S_scaled[0]) / sizeof(S_scaled[0][0]); j++) {
            //score = S_scaled[i][j];

            float exp_pos = expf(S_scaled[i][j] - max);
            sum += exp_pos;

            A[i][j] = exp_pos; //stores exp_pos in A temporarily
        }

        for (size_t j = 0; j < sizeof(S_scaled[0]) / sizeof(S_scaled[0][0]); j++) {
            float exp_pos = A[i][j];
            A[i][j] = exp_pos / sum;
        }
    }

    for (size_t i = 0; i < sizeof(A) / sizeof(A[0]); i++) {
        for (size_t j = 0; j < sizeof(V[0]) / sizeof(V[0][0]); j++) {
            float sum = 0;

            for (size_t k = 0; k < sizeof(A[0]) / sizeof(A[0][0]); k++) {
                float  value = A[i][k] * V[k][j];
                sum += value;
            }
            O[i][j] = sum;
        }
    }

    printf_matrix(
        sizeof(A) / sizeof(A[0]),
        sizeof(A[0]) / sizeof(A[0][0]),
        A
    );

    printf("\n");

    printf_matrix(
        sizeof(O) / sizeof(O[0]),
        sizeof(O[0]) / sizeof(O[0][0]),
        O
    );

    return 0;
}

void printf_matrix(int rows, int cols, float M[rows][cols]) {
    for (size_t i = 0; i < (size_t)rows; i++) {
        for (size_t j = 0; j < (size_t)cols; j++) {
            printf("%f  ", M[i][j]);
        }
        printf("\n");
    }
}
