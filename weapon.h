typedef struct w_param {
	float damage;
	float range;
	float speed;
	float cooldown;
	float lifetime;
	float h_width;
	float h_height;
} w_param;

typedef struct weapon {
	const w_param* p;
	float x;
	float y;
	float rot;
	int f_vis : 1;
	float tri_o;
	float tri_a;
	float time_active;
	float time_inactive;
	ef_video_element_t* e;
} weapon;

typedef struct entity {
	float x;
	float y;
	float rot;
	signed health : 8;//Max health: 127
	float h_height;
	float h_width;
} entity;

const w_param w_sword = {
	52.0f,//damage
	2.5f,//range
	10.f,//speed
	0.25f,//cooldown
	0.27f,//lifetime
	2.0f,//h_width
	2.0f//h_height
};

const w_param w_spell = {
	10.0f,//damage
	2.5f,//range
	5.5f,//speed
	0.1f,//cooldown
	4.0f,//lifetime
	1.0f,//h_width
	1.0f//h_height
};

void update_weapon( entity* user, weapon* w );
void atk_weapon( entity* user, weapon* w );
void draw_weapon( ef_video_scene_t* scn, weapon* w );

void update_weapon( entity* user, weapon* w ){
	if( w->f_vis ){
		if( w->time_active > w->p->lifetime ){
			w->f_vis = 0;
			return;
		}
		w->y += w->tri_o;
		w->x += w->tri_a;
		w->time_active += ef_system->delta_time;
	}else{
		w->time_inactive += ef_system->delta_time;
	}
}

void atk_weapon( entity* user, weapon* w ){
	if( w->time_inactive >= w->p->cooldown ){
		w->time_active = 0.0f;
		w->time_inactive = 0.0f;
		w->x = user->x;
		w->y = user->y;
		w->rot = user->rot;
		w->f_vis = 1;
		float h = w->p->speed * ef_system->delta_time;
		float f = sin( w->rot );
		float o = f * h;
		float g = cos( w->rot );
		float a = g * h;
		w->tri_o = o;
		w->tri_a = a;
	}
}

void draw_weapon( ef_video_scene_t* scn, weapon* w ){
	if( w->f_vis ){
		w->e->x = w->x;
		w->e->y = w->y;
		w->e->rot = w->rot;
		ef_video_draw_element( scn, w->e );
	}
}
