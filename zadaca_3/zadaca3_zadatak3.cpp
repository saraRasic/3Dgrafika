#include <iostream>
#include <cmath>

using namespace std;

struct Point {
    int x, y, z;
    int r, g, b;
};

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void draw_triangle(Point p0, Point p1, Point p2, int color, int image[][512]) {
    if (p0.y > p1.y) swap(p0, p1);
    if (p0.y > p2.y) swap(p0, p2);
    if (p1.y > p2.y) swap(p1, p2);

    int total_height = p2.y - p0.y;

    for (int y = p0.y; y <= p1.y; y++) {
        float alpha = (float)(y - p0.y) / total_height;
        float beta = (float)(y - p0.y) / (p1.y - p0.y);
        int x_start = lerp(p0.x, p2.x, alpha);
        int x_end = lerp(p0.x, p1.x, beta);

        x_start = max(0, min(511, x_start));
        x_end = max(0, min(511, x_end));

        for (int x = x_start; x <= x_end; x++) {
            image[x][y] = color;
        }
    }

    for (int y = p1.y + 1; y <= p2.y; y++) {
        float alpha = (float)(y - p0.y) / total_height;
        float beta = (float)(y - p1.y) / (p2.y - p1.y);
        int x_start = lerp(p0.x, p2.x, alpha);
        int x_end = lerp(p1.x, p2.x, beta);

        x_start = max(0, min(511, x_start));
        x_end = max(0, min(511, x_end));

        for (int x = x_start; x <= x_end; x++) {
            image[x][y] = color;
        }
    }
}

int main() {
    const int width = 512;
    const int height = 512;
    int image[width][height] = {0};

    Point p0 = {-48, -10, 82, 0, 0, 255};
    Point p1 = {29, -15, 44, 0, 0, 255};
    Point p2 = {13, 34, 114, 0, 0, 255};

    draw_triangle(p0, p1, p2, (255 << 16) | (0 << 8) | 0, image);

    return 0;
}