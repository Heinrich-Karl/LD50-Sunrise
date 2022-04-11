#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cef.h>

__asm__( ".symver realpath,realpath@GLIBC_2.25" );

struct{
	unsigned int up : 1;
	unsigned int down : 1;
	unsigned int left : 1;
	unsigned int right : 1;
	unsigned int attack : 1;
	unsigned int restart : 1;
	float cursor_x;
	float cursor_y;
}input_buffer = {0,0,0,0,0,0};

const float left_bound = -5.0f;
const float right_bound = 5.0f;
const float bottom_bound = -5.0f;

#include "texture.h"
#include "util.h"
#include "weapon.h"
int finished = 0;//mutable
void hurt_player( float damage );
int level_index = 0;//mutable
int* level_enemies = NULL;//mutable
void unload_level_enemies( int* ens );
#include "enemy.h"
#include "player.h"
#include "level.h"

void key_callback( ef_window_t* window, int key, int scancode, int action, int mods );
void cursor_callback( ef_window_t* window, double x, double y );
void mouse_callback( ef_window_t* window, int button, int action, int mods );

int main(){
	/*
	enum CFG {
		CFG_CTRL_UP,
		CFG_CTRL_DOWN,
		CFG_CTRL_LEFT,
		CFG_CTRL_RIGHT,
		CFG_SIZE
	};
	*/
	ef_init( __FILE__ );
	GLuint s_program = ef_init_shader_source( ef_shader_vertex_source, ef_shader_fragment_source );
	ef_set_key_callback( key_callback );
	ef_set_cursor_callback( cursor_callback );
	ef_set_mouse_callback( mouse_callback );
	ef_video_scene_t* scn = ef_video_create_scene( s_program, ef_config_read( ef_system->cfg, EF_CFG_WIDTH ), ef_config_read( ef_system->cfg, EF_CFG_HEIGHT ) );
	load_textures( scn );
	//ef_video_update_scene_view_scl( scn, 0.33f );
	ef_video_layer_t* lay = ef_video_create_layer( 0.0f, 0.0f, 500, 500 );
	int a = TEX_DARK_TILE;
	int b = TEX_LIGHT_TILE;
	char test_lvl_tile[] = {
		a, b, a, b, a, b, a, b, a, b,
		b, a, b, a, b, a, b, a, b, a,
		a, b, a, b, a, b, a, b, a, b,
		b, a, b, a, b, a, b, a, b, a,
		a, b, a, b, a, b, a, b, a, b,
		b, a, b, a, b, a, b, a, b, a,
		a, b, a, b, a, b, a, b, a, b,
		b, a, b, a, b, a, b, a, b, a,
		a, b, a, b, a, b, a, b, a, b,
		b, a, b, a, b, a, b, a, b, a
	};
	float test_lvl_enemies[] = {
		-5.0f,	5.0f,
		5.0f,	5.0f,
		0.0f,	10.0f,
		-5.0f,	-5.0f,
		5.0f,	5.0f,
	};
	ef_video_update_scene_view_scl( scn, 0.1f );
	load_level_tiles( scn, lay, 10, 10, test_lvl_tile );
	ef_video_element_t* lvl_e = ef_video_create_element( -4.5f, 4.5f, lay->index, NULL, NULL );
	ef_video_element_t* lvl_e_2 = ef_video_create_element( -4.5f, 14.5f, lay->index, NULL, NULL );
	ef_video_element_t* lvl_e_3 = ef_video_create_element( -4.5f, 24.5f, lay->index, NULL, NULL );
	lvl_e->sx = 20.0f;
	lvl_e->sy = 20.0f;
	lvl_e_2->sx = 20.0f;
	lvl_e_2->sy = 20.0f;
	lvl_e_3->sx = 20.0f;
	lvl_e_3->sy = 20.0f;
	ef_video_element_t* lvl_es[] = {
		lvl_e,
		lvl_e_2,
		lvl_e_3,
	};

	ef_video_element_t* end_e = ef_video_create_element( 0.0f, 0.0f, TEX_END, NULL, NULL );
	end_e->sx = 8.0f;
	end_e->sy = 2.0f;

	////////
	//INIT//
	////////
	init_enemy();
	init_player();

	//int* lvl_enemies = load_level_enemies( test_lvl_enemies, 4 );
	ef_video_update_scene_view_scl( scn, 0.1f );
	ef_video_clear_color( 0.0, 0.01, 0.02, 1.0 );
	do{
		ef_video_clear();
		ef_wait_events_timeout( 0.0001 );
		ef_update_delta_time();
		if( finished ){
			player.f_dead = 1;
			end_e->y = player.y + 7.5f;
			ef_video_draw_element( scn, end_e );

		}
		update_enemies( (entity*) &player );
		update_player( scn );
		update_camera( scn );
		level_test( lvl_es );
		ef_video_draw_element( scn, lvl_e );
		ef_video_draw_element( scn, lvl_e_2 );
		ef_video_draw_element( scn, lvl_e_3 );
		draw_enemies( scn );
		draw_player( scn );
		ef_swap();
	}while( !ef_should_close() );
	//unload_level_enemies( lvl_enemies );

	///////
	//END//
	///////
	end_enemy();
	end_player();

	ef_video_delete_element( &end_e );
	ef_video_delete_element( &lvl_e );
	ef_video_delete_element( &lvl_e_2 );
	ef_video_delete_element( &lvl_e_3 );
	ef_video_delete_layer( &lay );
	ef_video_delete_scene( &scn );

	ef_end();
	return 0;
}

void key_callback( ef_window_t* window, int key, int scancode, int action, int mods ){
	int a = 1;
	switch( action ){
		case GLFW_PRESS: a = 1;break;
		case GLFW_RELEASE: a = 0;break;
		default: return;
	}
	if( key == GLFW_KEY_W )
		( a ) ? ( input_buffer.up = 1 ) : ( input_buffer.up = 0 );
	if( key == GLFW_KEY_S )
		( a ) ? ( input_buffer.down = 1 ) : ( input_buffer.down = 0 );
	if( key == GLFW_KEY_A )
		( a ) ? ( input_buffer.left = 1 ) : ( input_buffer.left = 0 );
	if( key == GLFW_KEY_D )
		( a ) ? ( input_buffer.right = 1 ) : ( input_buffer.right = 0 );
	if( key == GLFW_KEY_R )
		( a ) ? ( input_buffer.restart = 1 ) : ( input_buffer.restart = 0 );
}

void cursor_callback( ef_window_t* window, double x, double y ){
	input_buffer.cursor_x = x;
	input_buffer.cursor_y = y;
}

void mouse_callback( ef_window_t* window, int button, int action, int mods ){
	int a;
	switch( action ){
		case GLFW_PRESS: a = 1;break;
		case GLFW_RELEASE: a = 0;break;
		default: return;
	}
	if( button == GLFW_MOUSE_BUTTON_LEFT )
		( a ) ? ( input_buffer.attack = 1 ) : ( input_buffer.attack = 0 );
}
