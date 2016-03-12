#pragma once

#include "GraphicsComponent.h"
#include "LTimer.h"
#include <string>

#define SCREEN_W 1600
#define SCREEN_H 900

struct Camera {
	double angle;
	double zoom;
	Vec2 target;
	bool locked;
};

class GraphicsSystem
{
public:
	GraphicsSystem();
	~GraphicsSystem();

	SDL_Texture* loadTexture(const std::string filename, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);
	void loadBackground(const std::string filename, int layer, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

	void update(Message &postman, GraphicsComponent gComp, double dt);
	void setCameraTarget(Vec2 t);
	void setCameraZoom(double zoom);
	void setCameraAngle(double angle);
	void setCamera(Vec2 t, double z, double a);
	void lockCamera(bool l = true);

	double getFPS();
	double getZoom(){ return m_camera.zoom; }

	void initFrame();
	void endFrame();

protected:

	double m_avgFPS;
	LTimer m_fpsTimer;
	int m_countedFrames;

	SDL_Renderer *m_renderer;
	SDL_Window *m_window;
	Camera m_camera;
	bool m_frameStarted;
	SDL_Texture* m_defaultHalo;
	SDL_Texture* m_backgrounds[4]; // 4 layers de background
	Vec2 m_backgroundSize[4];
};

