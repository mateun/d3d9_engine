#include <d3d9_engine.h>
#include <stdio.h>

int main(int argc, char** args) {
    printf("in testgame\n");
    GraphicsDevice gd(800, 600, false);
	SceneSimpleScreenSpace scene;
	std::unique_ptr<SceneNode> quadNode = std::make_unique<QuadNode>(D3DXVECTOR3(100, 200, 0), 200, 300, gd.getD3DDevice());
	scene.addNode(std::move(quadNode));

	quadNode = std::make_unique<QuadNode>(D3DXVECTOR3(400, 400, 0), 200, 50, gd.getD3DDevice());
	scene.addNode(std::move(quadNode));

	SceneSimple scene3d;

	std::vector<VertexPosColor> vertices;
	vertices.push_back(VertexPosColor(0, .5f, 2, 0xFFFFFFFF));
	vertices.push_back(VertexPosColor(.5f, -.5f, 2, 0xFFFFFFFF));
	vertices.push_back(VertexPosColor(-.5f, -.5f, 2, 0xFFFFFFFF));

	std::unique_ptr<SceneNode> meshNode = std::make_unique<MeshNode>(gd, D3DXVECTOR3(0, 0, 2), std::move(vertices));
	scene3d.addNode(std::move(meshNode));

    printf("start game loop\n");

    while(1) {
        if (gd.PollEvent() == EVT_CLOSE) {
            printf("window closed\n");
            break;
        }

		gd.Clear();
		gd.DrawText("render time(ms): ", 10, 10);
		scene.Render(gd);
		scene3d.Render(gd);
		gd.Render();
        
    }

    return 0;
}