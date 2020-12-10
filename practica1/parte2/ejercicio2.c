#include<stdio.h>
#include<stdlib.h>
int main() {
int i, j, h;
double column_sum[1000];
double b[1000][1000];
for (i = 0; i < 1000; i++) {
for (j = 0; j < 1000; j++) {
b[j][i] = 1.0;
}
}
for (h = 0; h < 50; h++) {
for (i = 0; i < 1000; i++) {
column_sum[i] = 0.0;
for (j = 0; j < 1000; j++) {
column_sum[i] += b[j][i];
}
}
}
}
