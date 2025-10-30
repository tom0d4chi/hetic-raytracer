#include <iostream>
#include "Color.hpp"
#include "Image.hpp"

using namespace std;


int main()
{    
    Color red(1, 0, 0);
    Color green(0, 1, 0);
    Color black;

    cout << "Red : " << red << std::endl;
    cout << "Green : " << green << std::endl;
    cout << "Black : " << black << std::endl;

    Color yellow = red + green;

    cout << "Yellow : " << yellow << std::endl;

    // Create an image in memory, and fill it with yellow
    Image image(512, 512, yellow);

    // Make a red square on the top left of the image
    for (int y = 0; y < 100; y++) {
      for (int x = 0; x < 100; x++) {
        image.SetPixel(x, y, Color(1, 0, 0));
      }
    }

    image.WriteFile("test.png");
}
