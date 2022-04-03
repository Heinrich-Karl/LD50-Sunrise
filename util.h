#define PI 3.14159265
#define radian( A ) ( A / 180.0f * PI )

int is_collision( float a_x, float a_y, float a_w, float a_h, float b_x, float b_y, float b_w, float b_h );
void cursor_to_pos( ef_video_scene_t* scn, double cur_x, double cur_y, float* out_x, float* out_y );
float calc_target_rot( float x, float y, float target_x, float target_y, float current_rot );

int is_collision( float a_x, float a_y, float a_w, float a_h, float b_x, float b_y, float b_w, float b_h ){
	if( 
		a_x >= b_x - b_w && a_x <= b_x + b_w && 
		a_y >= b_y - b_h && a_y <= b_y + b_h
	)
		return 1;
	return 0;
}

void cursor_to_pos( ef_video_scene_t* scn, double cur_x, double cur_y, float* out_x, float* out_y ){
	if( out_x != NULL ){
		float x = cur_x / ef_config_read( ef_system->cfg, EF_CFG_WIDTH );
		x *= 2;
		x -= 1;
		x -= scn->view_x;
		x /= scn->view_scl;
		*out_x = x;
	}
	if( out_y != NULL ){
		float y = cur_y / ef_config_read( ef_system->cfg, EF_CFG_HEIGHT );
		y *= -2;
		y += 1;
		y -= scn->view_y;
		y /= scn->view_scl;
		*out_y = y;
	}
}

float calc_target_rot( float x, float y, float target_x, float target_y, float current_rot ){
	float rel_x = target_x - x;
	float rel_y = target_y - y;
	float h = sqrt( pow( rel_x, 2 ) + pow( rel_y, 2 ) );
	if( h != 0.0f )
		return ( rel_x > 0.0 ) ? asin( rel_y / h ) : PI - asin( rel_y / h );
	else
		return current_rot;
}
