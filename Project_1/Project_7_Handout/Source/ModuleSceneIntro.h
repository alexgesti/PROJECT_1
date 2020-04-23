#ifndef __MODULE_SCENE_INTRO_H__
#define __MODULE_SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleSceneIntro : public Module
{
public:
	//Constructor
	ModuleSceneIntro(bool startEnabled);

	//Destructor
	~ModuleSceneIntro();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	update_status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	update_status PostUpdate() override;

	// Disables some textures
	bool CleanUp();

	// Resets All Stats
	void Reset();

public:
	// The sprite section for the background
	SDL_Rect background;
	SDL_Rect background2;
	SDL_Rect blackScreen;

	// The title sprite sheet loaded into an SDL_Texture
	SDL_Texture* Texture = nullptr;
	SDL_Texture* Texture2 = nullptr;

	// A set of animations
	Animation Nick;
	Animation Tom;
	Animation Title;
	Animation Logo;
	Animation Toaplan;
	Animation AllRight;
	Animation Credit;
	Animation Number;
	Animation StartName;
	
	// Nick&Tom X position
	int PosX = -32;

	// Coins and Sound
	int coins = 0;
	int CoinFx = 0;

	// Start
	bool CanStart = false;
};

#endif
