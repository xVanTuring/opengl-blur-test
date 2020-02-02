#pragma once
#define RECT_SIZE sizeof(float) * 12
#define BLUR_RECT_SIZE sizeof(float) * 24
float *rectWithSize(float width, float height)
{
    float *vertices = new float[2 * 6];
    int stride = 2;
    vertices[stride * 0] = 0;
    vertices[stride * 0 + 1] = 0;

    vertices[stride * 1] = width;
    vertices[stride * 1 + 1] = 0;

    vertices[stride * 2] = width;
    vertices[stride * 2 + 1] = height;

    vertices[stride * 3] = 0;
    vertices[stride * 3 + 1] = 0;

    vertices[stride * 4] = 0;
    vertices[stride * 4 + 1] = height;

    vertices[stride * 5] = width;
    vertices[stride * 5 + 1] = height;
    return vertices;
}
float *blurRectWithSize(float width, float height, float SCREEN_WIDTH, float SCREEN_HEIGHT)
{
    float startU = 0;
    float endU = 0;

    float startV = 0;
    float endV = 0;
    float *vertices = new float[4 * 6];
    int stride = 4;
    vertices[stride * 0 + 0] = 0;
    vertices[stride * 0 + 1] = 0;
    vertices[stride * 0 + 2] = startU;
    vertices[stride * 0 + 3] = startV;

    vertices[stride * 1 + 0] = width;
    vertices[stride * 1 + 1] = 0;
    vertices[stride * 1 + 2] = endU;
    vertices[stride * 1 + 3] = startV;

    vertices[stride * 2 + 0] = width;
    vertices[stride * 2 + 1] = height;
    vertices[stride * 2 + 2] = endU;
    vertices[stride * 2 + 3] = endV;

    vertices[stride * 3 + 0] = 0;
    vertices[stride * 3 + 1] = 0;
    vertices[stride * 3 + 2] = startU;
    vertices[stride * 3 + 3] = startV;

    vertices[stride * 4 + 0] = 0;
    vertices[stride * 4 + 1] = height;
    vertices[stride * 4 + 2] = startU;
    vertices[stride * 4 + 3] = endV;

    vertices[stride * 5 + 0] = width;
    vertices[stride * 5 + 1] = height;
    vertices[stride * 5 + 2] = endU;
    vertices[stride * 5 + 3] = endV;

    return vertices;
}