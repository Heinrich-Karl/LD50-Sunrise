#define MAX_ENEMIES 25

typedef struct enemy {
	float x;
	float y;
	float rot;
	signed health : 8;// Max health: 127
	float h_width;
	float h_height;
	weapon spell;
	int index;
	float speed;
	int f_vip : 1;
	int f_dead : 1;
	ef_video_element_t* e;
} enemy;

enemy* enemies[MAX_ENEMIES];

void init_enemy( void );
void end_enemy( void );
enemy* make_enemy( float x, float y );
void break_enemy( enemy* en );
void hurt_enemy( enemy* en, float damage );
int add_enemies( enemy* en );
enemy* remove_enemies( int i );
void update_enemies( entity* plyr );
void draw_enemies( ef_video_scene_t* scn );

void init_enemy( void ){
	for( int i = 0; i < MAX_ENEMIES; i++ )
		enemies[i] = NULL;
}

void end_enemy( void ){}

enemy* make_enemy( float x, float y ){
	enemy* en = (enemy*) malloc( sizeof( enemy ) );
	ef_video_animation_t* ap = (ef_video_animation_t*) malloc( sizeof( ef_video_animation_t ) );
	if( en != NULL && ap != NULL ){
		//en->index
		en->x = x;
		en->y = y;
		en->rot = radian( 270.0f );
		en->health = 100;
		en->h_width = 0.625f;
		en->h_height = 0.625f;
		en->spell.p = &w_spell;
		en->spell.f_vis = 0;
		en->spell.e = ef_video_create_element( en->x, en->y, TEX_SPELL_0, NULL, NULL );
		en->spell.e->ap = ap;
		en->spell.e->ap->frame = TEX_SPELL_0;
		en->spell.e->ap->base = TEX_SPELL_0;
		en->spell.e->ap->max = 5;
		en->spell.e->ap->fps = 12.0f;
		en->spell.e->sx = 0.5f;
		en->spell.e->sy = 0.5f;
		en->speed = 2.0f;
		en->f_vip = 0;
		en->f_dead = 0;
		en->e = ef_video_create_element( en->x, en->y, TEX_MC_IDLE_0, NULL, NULL );
		en->e->sy = 2.0f;
	}
	return en;
}

void break_enemy( enemy* en ){
	if( en != NULL ){
		free( en->spell.e->ap );
		if( en->spell.e != NULL )
			ef_video_delete_element( &(en->spell.e) );
		if( en->e != NULL )
			ef_video_delete_element( &(en->e) );
		free( en );
	}
}

void hurt_enemy( enemy* en, float damage ){
	if( !en->f_dead ){
		en->health -= damage;
		if( en->health <= 0 ){
			en->f_dead = 1;
			en->e->texture = TEX_MC_DEAD_0;
			en->e->sx = 6.7875f;
			en->e->sy = 4.1f;
		}
	}
}

int add_enemies( enemy* en ){
	for( int i = 0; i < MAX_ENEMIES; i++ ){
		if( enemies[i] == NULL ){
			enemies[i] = en;
			return i;
		}
	}
	return -1;//Out of room
}

enemy* remove_enemies( int i ){
	if( i >= 0 && i < MAX_ENEMIES && enemies[i] != NULL ){
		enemy* en = enemies[i];
		enemies[i] = NULL;
		return en;
	}
	return NULL;
}

void update_enemies( entity* plyr ){
	const float approch_range = 3.0f;
	const float retreat_range = 2.0f;
	const float fire_range = 8.0f;
	const float antisocial_range = 0.5f;
	for( int i = 0; i < MAX_ENEMIES; i++ ){
		enemy* en = enemies[i];
		if( en == NULL )
			break;
		update_weapon( (entity*) en, &(en->spell) );
		if( en->spell.f_vis ){
			weapon* w = &(en->spell);
			if( is_collision( 
				w->x, w->y, w->p->h_width, w->p->h_height,
				plyr->x, plyr->y, plyr->h_width, plyr->h_height 
			) ){
				w->f_vis = 0;
				hurt_player( w->p->damage );
			}else if( !en->f_dead ){
				if( w->x < plyr->x )
					w->x += w->p->speed * ef_system->delta_time;
				else
					w->x -= w->p->speed * ef_system->delta_time;
				if( w->y < plyr->y )
					w->y += w->p->speed * ef_system->delta_time;
				else
					w->y -= w->p->speed * ef_system->delta_time;
			}
		}
		if( en->f_dead )
			continue;
		en->rot = calc_target_rot( en->x, en->y, plyr->x, plyr->y, en->rot );
		if( 0 && i != 0 && !enemies[i - 1]->f_dead ){
			if( en->x <= enemies[i - 1]->x + antisocial_range )
				en->x -= en->speed * 0.75f * ef_system->delta_time;
			else if( en->x >= enemies[i - 1]->x - antisocial_range )
				en->x += en->speed * 0.75f * ef_system->delta_time;
		}
		if( i + 1 < MAX_ENEMIES && enemies[i + 1] != NULL && !enemies[i + 1]->f_dead ){
			if( en->x <= enemies[i + 1]->x + antisocial_range )
				en->x -= en->speed * ef_system->delta_time;
			else if( en->x >= enemies[i + 1]->x - antisocial_range )
				en->x += en->speed * ef_system->delta_time;
		}
		float dist = sqrt( pow( en->x - plyr->x, 2 ) + pow( en->y - plyr->y, 2 ) );
		int a = 0;
		if( dist > approch_range )
			a = 1;
		else if( dist < retreat_range )
			a = -1;
		if( a ){
			if( en->x < plyr->x )
				en->x += a * en->speed * ef_system->delta_time;
			else
				en->x -= a * en->speed * ef_system->delta_time;
			if( en->y < plyr->y )
				en->y += a * en->speed * ef_system->delta_time;
			else
				en->y -= a * en->speed * ef_system->delta_time;
		}
		if( dist < fire_range )
			atk_weapon( (entity*) en, &(en->spell) );
	}
}

void draw_enemies( ef_video_scene_t* scn ){
	for( int i = 0; i < MAX_ENEMIES; i++ ){ //Stops when first NULL cell is reached
		enemy* en = enemies[i];
		if( en == NULL ){
			break;
		}else{
			en->e->x = en->x;
			en->e->y = en->y;
			en->e->rot = en->rot;
			ef_video_draw_element( scn, en->e );
			draw_weapon( scn, &(en->spell) );
		}
	}
}

