#pragma once

#include "GraphicsComponent.h"
#include "LTimer.h"
#include <string>

#define SCREEN_W 1200
#define SCREEN_H 900

struct Camera {
	double angle;
	double zoom;
	Vec2 target;
	bool locked;
};

class GraphicsComponent;

class GraphicsSystem
{
public:
	static void init();
	static void close();
	static SDL_Texture* loadTexture(const std::string filename, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);
	static void loadBackground(const std::string filename, int layer, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

	static void update(Message &postman, GraphicsComponent gComp, double dt);
	static void setCameraTarget(Vec2 t);
	static void setCameraZoom(double zoom);
	static void setCameraAngle(double angle);
	static void setCamera(Vec2 t, double z, double a);
	static void lockCamera(bool l = true);

	static double getFPS();
	static double getZoom(){ return m_camera.zoom; }

	static void initFrame();
	static void endFrame();

protected:
	GraphicsSystem();
	~GraphicsSystem();
	static SDL_Window *m_window;
	static SDL_Renderer *m_renderer;
	static Camera m_camera;
	static bool m_frameStarted;
	static double m_avgFPS;
	static LTimer m_fpsTimer;
	static int m_countedFrames;
	static SDL_Texture* m_backgrounds[4]; // 4 layers de background
	static Vec2 m_backgroundSize[4];
};

