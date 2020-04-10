#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_scancode.h"


ModulePlayer::ModulePlayer()
{
	// idle animation - just one sprite
	idleRAnim.PushBack({ 16, 16, 21, 27 });
	idleLAnim.PushBack({ 283, 75, 21, 27 });
	fallRAnim.PushBack({ 16, 305, 20, 29 });
	fallLAnim.PushBack({ 546, 132, 20, 29 });

	// move upwards
	upRAnim.PushBack({ 16, 305, 20, 29 });
	upRAnim.PushBack({ 44, 307, 22, 27 });
	upRAnim.PushBack({ 74, 311, 20, 23 });
	upRAnim.PushBack({ 102, 312, 20, 22 });
	upRAnim.PushBack({ 130, 307 , 20, 27 });
	upRAnim.PushBack({ 159, 311, 20, 24 });
	upRAnim.PushBack({ 186, 311, 23, 23 });
	upRAnim.PushBack({ 217, 305, 20, 29 });
	upRAnim.loop = false;
	upRAnim.speed = 0.15f;

	upLAnim.PushBack({ 546, 132, 20, 29 });
	upLAnim.PushBack({ 516, 134, 22, 27 });
	upLAnim.PushBack({ 488, 138, 20, 23 });
	upLAnim.PushBack({ 460, 139, 20, 22 });
	upLAnim.PushBack({ 432, 134, 20, 27 });
	upLAnim.PushBack({ 404, 137, 20, 24 });
	upLAnim.PushBack({ 137, 138, 23, 23 });
	upLAnim.PushBack({ 345, 132, 20, 29 });
	upLAnim.loop = false;
	upLAnim.speed = 0.15f;

	RAnim.PushBack({ 16, 76, 18, 28});
	RAnim.PushBack({ 42, 77, 16, 27 });
	RAnim.PushBack({ 66, 76, 19, 28 });
	RAnim.PushBack({ 93, 77, 16, 27 });
	RAnim.loop = true;
	RAnim.speed = 0.1f;

	LAnim.PushBack({ 548, 79, 18, 28 });
	LAnim.PushBack({ 524, 80, 16, 27 });
	LAnim.PushBack({ 497, 79, 19, 28 });
	LAnim.PushBack({ 473, 80, 16, 27 });
	LAnim.loop = true;
	LAnim.speed = 0.1f;

}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Nick&Tom.png");
	currentAnimation = &idleRAnim;

	laserFx = App->audio->LoadFx("Assets/shot.wav"); // laser.wav por shot.wav
	// a�ad� sonidos del salto y muerte
	jumpFx = App->audio->LoadFx("Assets/jump.wav");
	deathFx = App->audio->LoadFx("Assets/death.wav");


	position.x = 150;
	position.y = 120;

	collider = App->collisions->AddCollider({ position.x, position.y, 32, 16 }, Collider::Type::PLAYER, this);

	return ret;
}

update_status ModulePlayer::Update()
{
	position.x += speed;
	if (death == false) {
		//Gravedad
		if (position.y < 120 && jump == false)//cambiarlo por colisiones
		{
			position.y += speed * 1.5;
			if (vista ==  1) {
				if (currentAnimation != &fallLAnim)
				{
					fallLAnim.Reset();
					currentAnimation = &fallLAnim;
				}
			}
			else {
				if (currentAnimation != &fallRAnim)
				{
					fallRAnim.Reset();
					currentAnimation = &fallRAnim;
				}
			}
		}

		if (App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT)
		{
			position.x -= speed;
			vista = 1;
			if (currentAnimation != &LAnim && timer >= 75)//coli-
			{
				LAnim.Reset();
				currentAnimation = &LAnim;
			}
		}

		if (App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT)
		{
			position.x += speed;
			vista = 0;
			if (currentAnimation != &RAnim && timer >= 75)// -siones
			{
				RAnim.Reset();
				currentAnimation = &RAnim;
			}
		}

		if (timer >= 75) { //bye bye timer, hola colisiones
			if (App->input->keys[SDL_SCANCODE_W] == KEY_STATE::KEY_DOWN)
			{
				timer = 0;
				high = position.y;
				jump = true;

				//  para asignar W al sonido del jump
				App->audio->PlayFx(jumpFx);
			}
		}
		timer += 1;
		if (jump == true)
		{
			if (position.y > high - 40)
			{
				position.y -= speed;
				if (vista == 1) {
					if (currentAnimation != &upLAnim)
					{
						upRAnim.Reset();
						currentAnimation = &upLAnim;
					}
				}
				else {
					if (currentAnimation != &upRAnim)
					{
						upRAnim.Reset();
						currentAnimation = &upRAnim;
					}
				}

				if (position.y <= high - 40)
				{
					jump = false;
					high = 0;
				}
			}
		}

		if (App->input->keys[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
		{
			if (vista == 1) {
				App->particles->AddParticle(App->particles->lasery, position.x - 9, position.y + 8, Collider::Type::PLAYER_SHOT);
				App->audio->PlayFx(laserFx);
			}
			else {
				App->particles->AddParticle(App->particles->laserx, position.x + 20, position.y + 8, Collider::Type::PLAYER_SHOT);
				App->audio->PlayFx(laserFx);
			}
		}
	}  

	if (App->input->keys[SDL_SCANCODE_ESCAPE] == KEY_STATE::KEY_DOWN) {
		return update_status::UPDATE_STOP;
	}

	// If no  movement detected or default floor, set the current animation back to idle
	if (App->input->keys[SDL_SCANCODE_W] == KEY_STATE::KEY_IDLE 
		&& App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_IDLE
		&& position.y >= 120  //mas colisiones
		&& vista == 0)
		currentAnimation = &idleRAnim;
	if (App->input->keys[SDL_SCANCODE_W] == KEY_STATE::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_A] == KEY_STATE::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_D] == KEY_STATE::KEY_IDLE
		&& position.y >= 120  //mas colisiones
		&& vista == 1)
		currentAnimation = &idleLAnim;
	

	collider->SetPos(position.x, position.y);

	currentAnimation->Update();

	if (destroyed)
	{
		death = true;
		destroyedCountdown--;
		if (destroyedCountdown <= 0)
			return update_status::UPDATE_STOP;
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}

	return update_status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collider && destroyed == false)
	{
		App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);

		App->audio->PlayFx(deathFx);

		destroyed = true;
	}
}