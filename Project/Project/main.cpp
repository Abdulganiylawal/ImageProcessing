#include <iostream>
#include <vector>
#include "image.h"

using namespace std;

void gaussianBlur(const string&);
void edgeDetection(const string&);

int main()
{
    int choice;
    cout << "What operation do you want to perform?\n1. Perform Gaussian Blur\n2. Perform Edge Detection\n" << endl;
    cout << "Enter a Choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            gaussianBlur("Lenna_(test_image).png");
            break;
        case 2:
            edgeDetection("Lenna_(test_image).png");
            break;
        default:
            cout << "Invalid choice!";
            break;
    }

    return 0;
}

void gaussianBlur(const string& image)
{
    GrayscaleImage I;
    I.Load(image);
    int h = I.GetHeight();
    int w = I.GetWidth();
    GrayscaleImage o(w, h);
    const int radius = 3;
    const float sd = 2.0f;
    const int size = 2 * radius + 1;

    vector<vector<float> > Kernel(size, vector<float>(size));

    const float firstPart = 1 / (2 * 3.147 * sd * sd);

    for (int i = -radius; i <= radius; i++)
    {
        for (int j = -radius; j <= radius; j++)
        {
            const float secondPart = -(j * j + i * i) / (2 * sd * sd);
            Kernel[i + radius][j + radius] = firstPart * exp(secondPart);
        }
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            float sum = 0.0f;
            float weightSum = 0.0f;
            for (int i = -radius; i <= radius; i++)
            {
                for (int j = -radius; j <= radius; j++)
                {
                    float gaussianValue = Kernel[i + radius][j + radius];
                    sum += gaussianValue * I.Get(x + j, y + i);
                    weightSum += gaussianValue;
                }
            }
            o(x, y) = sum / weightSum;
        }
    }
    o.Save("image.png");
}

void edgeDetection(const string& image)
{
    GrayscaleImage I;
    I.Load(image);
    int h = I.GetHeight();
    int w = I.GetWidth();
    GrayscaleImage o(w, h);

    for (int y = 1; y < h - 1; y++)
    {
        for (int x = 1; x < w - 1; x++)
        {
            int Gx = I.Get(x + 1, y - 1) - I.Get(x - 1, y - 1)
                + 2 * I.Get(x + 1, y) - 2 * I.Get(x - 1, y)
                + I.Get(x + 1, y + 1) - I.Get(x - 1, y + 1);

            int Gy = I.Get(x - 1, y + 1) - I.Get(x - 1, y - 1)
                + 2 * I.Get(x, y + 1) - 2 * I.Get(x, y - 1)
                + I.Get(x + 1, y + 1) - I.Get(x + 1, y - 1);
            o(x, y) = sqrt(Gx * Gx + Gy * Gy);
        }
    }
    o.Save("Out.png");
}
