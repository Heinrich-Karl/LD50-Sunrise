struct player {
	float x;
	float y;
	float rot;
	signed health : 8;//Max health: 127
	float h_width;
	float h_height;
	weapon sword;
	int tex;
	float speed;
	int f_dead : 1;
	ef_video_element_t* e;
} player;

void init_player( void );
void end_player( void );
void update_player( ef_video_scene_t* scn );
void draw_player( ef_video_scene_t* scn );
void update_camera( ef_video_scene_t* scn );

void init_player( void ){
	player.x = 0.0f;
	player.y = 0.0f;
	player.rot = radian( 90.0 );
	player.health = 100;
	player.h_width = 0.5f;
	player.h_height = 0.5f;
	player.sword.p = &w_sword;
	player.sword.f_vis = 0;
	player.sword.x = player.x;
	player.sword.y = player.y;
	player.sword.rot = player.rot;
	player.sword.e = ef_video_create_element( player.x, player.y, TEX_SWORD, NULL, NULL );
	player.sword.e->sx = 0.25f;
	player.tex = TEX_SC_IDLE_0;
	player.speed = 7.5f;
	player.f_dead = 0;
	player.e = ef_video_create_element( player.x, player.y, player.tex, NULL, NULL );
	ef_video_animation_t* ap = (ef_video_animation_t*) malloc( sizeof( ef_video_animation_t ) );
	if( ap != NULL ){
		ap->frame = TEX_SC_IDLE_0;
		ap->base = TEX_SC_IDLE_0;
		ap->max = 9;
		ap->fps = 12.0f;
		player.e->ap = ap;
	}
	//player.e->sy = 2.17f;
	//player.e->sy = 2.821f;
	player.e->sy = 2.4063f;
	player.e->sx = 1.3f;
}

void end_player( void ){
	ef_video_delete_element( &(player.sword.e) );
	if( player.e->ap != NULL )
		free( player.e->ap );
	ef_video_delete_element( &(player.e) );
}

void update_player( ef_video_scene_t* scn ){
	if( !player.f_dead ){
		float cur_x;
		float cur_y;
		cursor_to_pos( scn, input_buffer.cursor_x, input_buffer.cursor_y, &cur_x, &cur_y );
		player.y += input_buffer.up * ef_system->delta_time * player.speed;
		player.y -= input_buffer.down * ef_system->delta_time * player.speed;
		player.x += input_buffer.right * ef_system->delta_time * player.speed;
		player.x -= input_buffer.left * ef_system->delta_time * player.speed;
		if( player.x > right_bound )
			player.x = right_bound;
		else if( player.x < left_bound )
			player.x = left_bound;
		if( player.y < bottom_bound )
			player.y = bottom_bound;
		if( input_buffer.attack ){
			//player.sword.tri_a = cur_x;
			//player.sword.tri_o = cur_y;
			atk_weapon( (entity*) &player, &(player.sword) );
		}
		player.rot = calc_target_rot( player.x, player.y, cur_x, cur_y, player.rot );
	}else if( input_buffer.restart ){
		finished = 0;
		player.x = 0.0f;
		player.y = 0.0f;
		player.health = 100;
		player.f_dead = 0;
		player.e->texture = TEX_SC_IDLE_0;
		if( player.e->ap != NULL ){
			player.e->ap->max = 9;
			player.e->ap->frame = TEX_SC_IDLE_0;
			player.e->ap->base = TEX_SC_IDLE_0;
		}
		unload_level_enemies( level_enemies );
		level_index = 0;
	}
	update_weapon( (entity*) &player, &(player.sword) );
	if( player.sword.f_vis )
		for( int i = 0; i < MAX_ENEMIES; i++ ){
			weapon* w = &(player.sword);
			enemy* en = enemies[i];
			if( en == NULL )
				break;
			if( !en->f_dead && is_collision( 
				w->x, w->y, w->p->h_width, w->p->h_height,
				en->x, en->y, en->h_width, en->h_height 
			) ){
				hurt_enemy( en, w->p->damage );
				w->f_vis = 0;
				break;
			}else if( en->spell.f_vis && is_collision( 
				w->x, w->y, w->p->h_width, w->p->h_height,
				en->spell.x, en->spell.y, en->spell.p->h_width, en->spell.p->h_height 
			) ){
				en->spell.f_vis = 0;
				w->f_vis = 0;
				break;
			}
		}
}

void draw_player( ef_video_scene_t* scn ){
	player.e->x = player.x;
	player.e->y = player.y;
	player.e->rot = player.rot;
	ef_video_draw_element( scn, player.e );
	draw_weapon( scn, &(player.sword) );
}

void hurt_player( float damage ){
	if( !player.f_dead ){
		player.health -= damage;
		if( player.health <= 0 ){
			if( player.e->ap != NULL ){
				player.e->ap->max = 0;
				player.e->ap->frame = TEX_SC_DEAD_0;
				player.e->ap->base = TEX_SC_DEAD_0;
			}
			player.f_dead = 1;
			printf( "You are dead\n" );
		}
	}
}

void update_camera( ef_video_scene_t* scn ){
	const float tolerance = 0.1f;
	const float speed = 0.01f;
	float x = scn->view_scl * -player.x;//scn->view_x;
	float y = scn->view_scl * -player.y;//scn->view_y;
	/*
	if( scn->view_x < player.x - tolerance ){
		x -= speed * pow( 2, abs( x - player.x ) ) * ef_system->delta_time;
	}else if( scn->view_x > player.x + tolerance ){
		x += speed * pow( 2, abs( x - player.x ) ) * ef_system->delta_time;
	}
	*/
	ef_video_update_scene_view_pos( scn, x, y );
}
