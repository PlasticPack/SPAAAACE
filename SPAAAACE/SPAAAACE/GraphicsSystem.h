#pragma once

#include "GraphicsComponent.h"
#include <string>

struct Camera {
	double angle;
	double zoom;

};

class GraphicsSystem
{
public:
	GraphicsSystem();
	~GraphicsSystem();

	SDL_Texture* loadTexture(const std::string filename);

	void update(GraphicsComponent gComp, double dt);

	void initFrame();
	void endFrame();

protected:
	SDL_Renderer *m_renderer;
	SDL_Window *m_window;
	Camera m_camera;
	bool m_frameStarted;
};

