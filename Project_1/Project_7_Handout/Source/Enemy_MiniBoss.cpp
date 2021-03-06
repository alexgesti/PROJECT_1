#include "Enemy_MiniBoss.h"

#include "Application.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"

Enemy_MiniBoss::Enemy_MiniBoss(float x, float y) : Enemy(x, y)
{
	walkRAnim.PushBack({ 5, 7, 23, 25 });
	walkRAnim.PushBack({ 38, 7, 23, 25 });
	walkRAnim.speed = 0.2f;

	walkLAnim.PushBack({ 196, 39, 23, 25 });
	walkLAnim.PushBack({ 163, 39, 23, 25 });
	walkLAnim.speed = 0.2f;

	downRAnim.PushBack({ 200, 11, 16,19 });

	downLAnim.PushBack({ 8, 43, 16, 19 });

	nothing.PushBack({ 0, 0, 0, 0 });

	Ldead.PushBack({ 5, 39, 23, 25 });
	Ldead.PushBack({ 38, 39, 23, 25 });
	Ldead.speed = 0.2f;

	Rdead.PushBack({ 197, 7, 23, 25 });
	Rdead.PushBack({ 166, 7, 23, 25 });
	Rdead.speed = 0.2f;

	vistard = true;

	collider = App->collisions->AddCollider({ 0, 0, 22, 25 }, Collider::Type::ENEMY);
	cfs = App->collisions->AddCollider({ 0, 0, 22, 24 }, Collider::Type::FISICSNOW, (Module*)App->enemies);

	miniboss = true;
}

void Enemy_MiniBoss::Update()
{
	//Admin. Collider
	if (cfs != nullptr && candelete == false && InitialD == false) {
		cfs->SetPos(position.x + 4, position.y + 3);
		balldash->SetPos(-600, -600);
		if (cout < 8) {
			wall1->SetPos(-400, -400);
			wall2->SetPos(-400, -400);
			if (cout <= 0) {
				collider->SetPos(position.x + 4, position.y + 2);
			}
		}
	}
	if (cout > 0) {
		collider->SetPos(-600, -600);
		if (InitialD == true) {
			cfs->SetPos(-300, -300);
			balldash->SetPos(position.x + 4, position.y + 3);
		}
	}
	if (InitialD == true) {
		countdown++;
		if (countdown >= 5) {
			wall1->SetPos(-400, -400);
			wall2->SetPos(-400, -400);
		}
	}

	position.x = spawnPos.x + position.x + speedx;

	if (cout <= 0) {
		if (gravity == true) {
			speedx = 0;
			position.y = spawnPos.y + position.y + speedy;
			if (vistard == true) {
				if (currentAnim != &downLAnim)
				{
					downLAnim.Reset();
					currentAnim = &downLAnim;
				}
			}
			else {
				if (currentAnim != &downRAnim)
				{
					downRAnim.Reset();
					currentAnim = &downRAnim;
				}
			}
		}
		else if (vistard == true) {
			speedx = (-2);
			if (currentAnim != &walkLAnim)
			{
				walkLAnim.Reset();
				currentAnim = &walkLAnim;
			}
		}
		else if (vistard == false) {
			speedx = 2;
			if (currentAnim != &walkRAnim)
			{
				walkRAnim.Reset();
				currentAnim = &walkRAnim;
			}
		}

		if (hitwallL == true) {
			vistard = false;
			random = (rand() % 4 + 1) * 100;
			hitwallL = false;
		}
		else if (hitwallR == true) {
			vistard = true;
			random = (rand() % 4 + 1) * 100;
			hitwallR = false;
		}
	}

	else if (cout > 0 && cout < 8) {
		speedx = 0;
		if (gravity == true) {
			position.y = spawnPos.y + position.y + speedy;
		}

		if (vistard == true) {
			if (currentAnim != &Ldead)
			{
				Ldead.Reset();
				currentAnim = &Ldead;
			}
		}
		else {
			if (currentAnim != &Rdead)
			{
				Rdead.Reset();
				currentAnim = &Rdead;
			}
		}
	}

	else if (cout >= 8) {
		speedx = 0;
		if (currentAnim != &nothing)
		{
			nothing.Reset();
			currentAnim = &nothing;
		}

		if (gravity == true) {
			position.y = spawnPos.y + position.y + (speedy * 2);
		}

		if (InitialD == false) {
			if (hitwallL == true) {
				push = false;
				if (vistard == true) {
					speedx = 0;
				}
				vistard = false;
				hitwallL = false;
			}
			else if (hitwallR == true) {
				push = false;
				if (vistard == false) {
					speedx = 0;
				}
				vistard = true;
				hitwallR = false;
			}
			else {
				if (push == true) {
					if (App->player->vista == true) {
						speedx = (-1);		//Misma velocidad que el player
						vistard = true;
					}
					if (App->player->vista == false) {
						speedx = 1;		//Misma velocidad que el player
						vistard = false;
					}
				}
			}
			if (up == true) {
				if (vistard == true) {
					speedx = (-2);
				}
				if (vistard == false) {
					speedx = 2;
				}
				up = false;
			}
		}

		else if (InitialD == true) {
			if (vistard == false) {
				speedx = 2;
			}
			if (vistard == true) {
				speedx = (-2);
			}

			if (hitwallL == true) {
				vistard = false;
				hitwallL = false;
			}
			else if (hitwallR == true) {
				vistard = true;
				hitwallR = false;
			}
		}
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}