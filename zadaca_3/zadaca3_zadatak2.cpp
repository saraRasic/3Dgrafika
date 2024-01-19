#include <iostream>
#include <cmath>

using namespace std;

struct Point {
    int x, y;
    int r, g, b;
};

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void gouraudInterpolation(const Point& p0, const Point& p1, const Point& p2, float alpha, float beta, float gamma, Point& result) {
    result.r = lerp(lerp(p0.r, p1.r, alpha), p2.r, gamma);
    result.g = lerp(lerp(p0.g, p1.g, alpha), p2.g, gamma);
    result.b = lerp(lerp(p0.b, p1.b, alpha), p2.b, gamma);
}

void draw_triangle_2d_gouraud(Point p0, Point p1, Point p2, int image[][512]) {
    if (p0.y > p1.y) swap(p0, p1);
    if (p0.y > p2.y) swap(p0, p2);
    if (p1.y > p2.y) swap(p1, p2);

    int total_height = p2.y - p0.y;

    for (int y = p0.y; y <= p1.y; y++) {
        float alpha = (float)(y - p0.y) / total_height;
        float beta = (float)(y - p0.y) / (p1.y - p0.y);
        Point left, right;
        gouraudInterpolation(p0, p2, p1, alpha, beta, 1.0f - beta, left);
        gouraudInterpolation(p0, p2, p1, alpha, beta, 1.0f - beta, right);

        if (left.x > right.x) swap(left, right);

        int x_start = max(0, min(511, left.x));
        int x_end = max(0, min(511, right.x));

        for (int x = x_start; x <= x_end; x++) {
            float gamma = (float)(x - left.x) / (right.x - left.x);
            Point current;
            gouraudInterpolation(left, right, left, gamma, 1.0f - gamma, 0.0f, current);

            image[x][y] = (current.r << 16) | (current.g << 8) | current.b;
        }
    }

    for (int y = p1.y + 1; y <= p2.y; y++) {
        float alpha = (float)(y - p0.y) / total_height;
        float beta = (float)(y - p1.y) / (p2.y - p1.y);
        Point left, right;
        gouraudInterpolation(p0, p2, p1, alpha, beta, 1.0f - beta, left);
        gouraudInterpolation(p0, p2, p1, alpha, beta, 1.0f - beta, right);

        if (left.x > right.x) swap(left, right);

        int x_start = max(0, min(511, left.x));
        int x_end = max(0, min(511, right.x));

        for (int x = x_start; x <= x_end; x++) {
            float gamma = (float)(x - left.x) / (right.x - left.x);
            Point current;
            gouraudInterpolation(left, right, left, gamma, 1.0f - gamma, 0.0f, current);

            image[x][y] = (current.r << 16) | (current.g << 8) | current.b;
        }
    }
}

int main() {
    const int width = 512;
    const int height = 512;
    int image[width][height] = {0};

    Point p0 = {100, 100, 255, 0, 0};
    Point p1 = {400, 500, 0, 255, 0};
    Point p2 = {700, 100, 0, 0, 255};
    draw_triangle_2d_gouraud(p0, p1, p2, image);

    Point p3 = {200, 300, 255, 0, 255};
    Point p4 = {500, 300, 0, 255, 255};
    Point p5 = {350, 100, 255, 255, 0};
    draw_triangle_2d_gouraud(p3, p4, p5, image);
    
    return 0;
}