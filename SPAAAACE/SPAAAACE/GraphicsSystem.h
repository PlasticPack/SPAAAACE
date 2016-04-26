#pragma once

#include "GraphicsComponent.h"
#include "LTimer.h"
#include <string>
#include <SDL_ttf.h>

#define SCREEN_W 1920
#define SCREEN_H 1080

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

	static void update(Message &postman, std::string id, GraphicsComponent gComp, double dt);
	static void setCameraTarget(Vec2 t);
	static void setCameraZoom(double zoom);
	static void setCameraAngle(double angle);
	static void setCamera(Vec2 t, double z, double a);
	static void lockCamera(bool l = true);

	static void reset();

	static double getFPS();
	static double getCameraZoom(){ return m_camera.zoom; }
	static double getCameraAngle(){ return m_camera.angle; }
	static void clearBackgrounds() { 
		for (int i = 0; i < 4; i++){
			if (m_backgrounds[i] != NULL){
				SDL_DestroyTexture(m_backgrounds[i]);
				m_backgrounds[i] = NULL;
				m_backgroundSize[i] = Vec2(0, 0);
			}
		}
	};

	static bool backgroundExists() {
		bool e = false;
		for (int i = 0; i < 4; i++){
			if (m_backgrounds[i] != NULL){
				e = true;
				i = 4;
			}
		}

		return e;
	}

	static void initFrame();
	static void endFrame(Message &postman);

	static void setFont(std::string f, int s, SDL_Color c = {255, 255, 255});
	static void setTextColor(SDL_Color c);
	static void print(std::string text);
	static void printAt(std::string text, int x, int y, int d_w = 0, int d_h = 0);

	static bool isTextLeft() { return m_textQueue.size(); }

protected:
	GraphicsSystem();
	~GraphicsSystem();
	static SDL_Window *m_window;
	static SDL_Renderer *m_renderer;
	static TTF_Font* m_currentFont;
	static Camera m_camera;
	static bool m_frameStarted;
	static double m_avgFPS;
	static LTimer m_fpsTimer;
	static int m_countedFrames;
	static SDL_Texture* m_backgrounds[4]; // 4 layers de background
	static Vec2 m_backgroundSize[4];
	static bool m_initialized;
	static SDL_Texture* m_currentTextTexture;
	static SDL_Color m_textColor;
	static std::string m_currentText;
	static std::map<std::string, SDL_Texture*> m_texts;
	static std::vector<std::string> m_textQueue;
	static LTimer m_textTimer;
};

