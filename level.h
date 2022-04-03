const int last_level = 5;
const float level_bounds[] = {
	20.0f,
	40.0f,
	80.0f,
	120.0f,
	150.0f,
	180.0f,
	1000.0f
};
int level_en_num[] = {
	1,
	3,
	6,
	9,
	20
};
float level_en_pos_0[] = {
	0.0f,	level_bounds[0] + 10.0f
};
float level_en_pos_1[] = {
	1.0f,	level_bounds[1] + 10.0f,
	4.0f,	level_bounds[1] + 13.0f,
	2.0f,	level_bounds[1] + 16.0f,
};
float level_en_pos_2[] = {
	1.5f,	level_bounds[2] + 10.0f,
	-1.5f,	level_bounds[2] + 10.0f,
	1.5f,	level_bounds[2] + 15.0f,
	-1.5f,	level_bounds[2] + 15.0f,
	1.5f,	level_bounds[2] + 20.0f,
	-1.5f,	level_bounds[2] + 20.0f,
};
float level_en_pos_3[] = {
	1.5f,	level_bounds[3] + 10.0f,
	-1.5f,	level_bounds[3] + 14.0f,
	2.3f,	level_bounds[3] + 17.0f,
	-3.7f,	level_bounds[3] + 12.0f,
	0.5f,	level_bounds[3] + 15.0f,
	-3.5f,	level_bounds[3] + 19.0f,
	-4.0f,	level_bounds[3] + 20.0f,
	-2.0f,	level_bounds[3] + 22.0f,
	2.0f,	level_bounds[3] + 24.0f,
};
float level_en_pos_4[] = {
	1.5f,	level_bounds[4] + 10.0f,
	1.5f,	level_bounds[4] + 11.0f,
	2.3f,	level_bounds[4] + 10.0f,
	-3.0f,	level_bounds[4] + 12.0f,
	-0.5f,	level_bounds[4] + 11.5f,
	-1.5f,	level_bounds[4] + 10.5f,
	1.5f,	level_bounds[4] + 10.0f,
	1.5f,	level_bounds[4] + 11.0f,
	2.3f,	level_bounds[4] + 10.0f,
	-3.0f,	level_bounds[4] + 12.0f,
	-0.5f,	level_bounds[4] + 11.5f,
	-1.5f,	level_bounds[4] + 10.5f,
	2.5f,	level_bounds[4] + 11.5f,
	1.5f,	level_bounds[4] + 10.5f,
	0.5f,	level_bounds[4] + 10.0f,
	-1.5f,	level_bounds[4] + 11.0f,
	3.3f,	level_bounds[4] + 10.0f,
	-3.0f,	level_bounds[4] + 12.0f,
};
float* level_en_pos[] = {
	level_en_pos_0,
	level_en_pos_1,
	level_en_pos_2,
	level_en_pos_3,
	level_en_pos_4,
};

int* load_level_enemies( float* ens, int num );
void load_level_tiles( ef_video_scene_t* scn, ef_video_layer_t* lay, unsigned width, unsigned height, char* lvl );

void level_test( ef_video_element_t** lvl_es ){
	if( player.y > level_bounds[level_index] ){
		unload_level_enemies( level_enemies );
		if( level_index == last_level ){
			finished = 1;
			return;
		}
		lvl_es[0]->y = player.y + 10.0f;
		lvl_es[1]->y = lvl_es[0]->y + 10.0f;
		lvl_es[2]->y = lvl_es[1]->y + 10.0f;
		level_enemies = load_level_enemies( level_en_pos[level_index], level_en_num[level_index] );
		if( player.health + 50 < 127 )
			player.health += 50;
		else
			player.health = 127;
		level_index++;
	}
}

void end_screen( void ){}

int* load_level_enemies( float* ens, int num ){
	int* output = (int*) malloc( num * sizeof( int ) );
	for( int i = 0; i < num * 2; i += 2 ){
		output[i / 2] = add_enemies( make_enemy( ens[i], ens[i + 1] ) );
	}
	return output;
}

void unload_level_enemies( int* ens ){
	if( ens == NULL )
		return;
	int num = sizeof( ens ) / sizeof( int );
	for( int i = 0; i < num; i++ ){
		break_enemy( remove_enemies( ens[i] ) );
	}
}

void load_level_tiles( ef_video_scene_t* scn, ef_video_layer_t* lay, unsigned width, unsigned height, char* lvl ){
	ef_video_element_t* e = ef_video_create_element( 0.0f, 0.0f, 0, NULL, NULL );
	ef_video_open_layer( scn, lay );
	ef_video_clear_color( 0.0f, 0.0f, 0.0f, 0.0f );
	ef_video_clear();
	for( int i = 0; i < height; i++ ){
		for( int j = 0; j < width; j++ ){
			e->x = (float) j;
			e->y = (float) i;
			e->texture = (unsigned) lvl[ i * width + j ];
			ef_video_draw_element( scn, e );
		}
	}
	ef_video_close_layer( scn, lay );
}
