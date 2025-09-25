#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double x;
    double y;
} Point;

void monotone_cubic_interpolation(Point* data, int n, double* x_values, double* y_values, int m) {
    double* h = (double*)malloc((n - 1) * sizeof(double));
    double* delta = (double*)malloc((n - 1) * sizeof(double));
    double* slopes = (double*)malloc((n - 1) * sizeof(double));
    
    // Calculate h and delta
    for (int i = 0; i < n - 1; i++) {
        h[i] = data[i + 1].x - data[i].x;
        delta[i] = (data[i + 1].y - data[i].y) / h[i];
    }

    // Calculate slopes
    for (int i = 1; i < n - 1; i++) {
        if (delta[i - 1] * delta[i] <= 0) {
            slopes[i] = 0;
        } else {
            slopes[i] = (delta[i - 1] + delta[i]) / 2;
        }
    }
    slopes[0] = delta[0];
    slopes[n - 2] = delta[n - 2];

    // Interpolating
    for (int j = 0; j < m; j++) {
        // Find the right interval
        int i = 0;
        while (i < n - 1 && x_values[j] > data[i + 1].x) {
            i++;
        }
        
        if (i == 0) {
            y_values[j] = data[0].y; // Use first point if x_value is out of bounds
        } else if (i == n - 1) {
            y_values[j] = data[n - 1].y; // Use last point if x_value is out of bounds
        } else {
            double h_i = data[i + 1].x - data[i].x;
            double a = (data[i + 1].y - data[i].y) / h_i - h_i * (slopes[i] + 2 * slopes[i + 1]) / 3;
            double b = slopes[i];
            double c = data[i].y;
            double x_diff = x_values[j] - data[i].x;
            y_values[j] = c + b * x_diff + a * x_diff * x_diff * (x_diff - h_i) / (h_i * h_i);
        }
    }

    free(h);
    free(delta);
    free(slopes);
}

int main() {
    // Sample data
    Point data[] = {
        {0, 0},
        {10, 1},
        {20, 4},
        {30, 9},
        {40, 16},
        {50, 25}
    };
    int n = sizeof(data) / sizeof(data[0]);

    // Points to interpolate
    double x_values[] = {5, 15, 25, 35, 45};
    int m = sizeof(x_values) / sizeof(x_values[0]);
    double* y_values = (double*)malloc(m * sizeof(double));

    // Perform interpolation
    monotone_cubic_interpolation(data, n, x_values, y_values, m);

    // Print results
    for (int i = 0; i < m; i++) {
        printf("x: %.2f, interpolated y: %.4f\n", x_values[i], y_values[i]);
    }

    free(y_values);
    return 0;
}
