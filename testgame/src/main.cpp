#include <d3d9_engine.h>
#include <stdio.h>

int main(int argc, char** args) {
    printf("in testgame\n");
    GraphicsDevice gd(800, 600, false);

    printf("start game loop\n");

    while(1) {
        if (gd.PollEvent() == EVT_CLOSE) {
            printf("window closed\n");
            break;
        }

		gd.Clear();
		gd.Render();
        
    }

    return 0;
}