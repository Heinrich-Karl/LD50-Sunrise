enum TEXTURES {
	TEX_SC_IDLE_0,
	TEX_SC_IDLE_1,
	TEX_SC_IDLE_2,
	TEX_SC_IDLE_3,
	TEX_SC_IDLE_4,
	TEX_SC_IDLE_5,
	TEX_SC_IDLE_6,
	TEX_SC_IDLE_7,
	TEX_SC_IDLE_8,
	TEX_SC_IDLE_9,
	TEX_SC_DEAD_0,
	TEX_SWORD,
	TEX_MC_IDLE_0,
	TEX_MC_DEAD_0,
	TEX_SPELL_0,
	TEX_SPELL_1,
	TEX_SPELL_2,
	TEX_SPELL_3,
	TEX_SPELL_4,
	TEX_SPELL_5,
	TEX_DARK_TILE,
	TEX_LIGHT_TILE,
	TEX_END,
	TEX_COUNT
};

char* texture_s[] = {
	"media/img/sun_champion_idle_0.png",
	"media/img/sun_champion_idle_1.png",
	"media/img/sun_champion_idle_2.png",
	"media/img/sun_champion_idle_3.png",
	"media/img/sun_champion_idle_4.png",
	"media/img/sun_champion_idle_5.png",
	"media/img/sun_champion_idle_6.png",
	"media/img/sun_champion_idle_7.png",
	"media/img/sun_champion_idle_8.png",
	"media/img/sun_champion_idle_9.png",
	"media/img/sun_champion_dead_0.png",
	"media/img/sword.png",
	"media/img/moon_cultist_idle_0.png",
	"media/img/moon_cultist_dead_0.png",
	"media/img/spell_0.png",
	"media/img/spell_1.png",
	"media/img/spell_2.png",
	"media/img/spell_3.png",
	"media/img/spell_4.png",
	"media/img/spell_5.png",
	"media/img/dark_tile.png",
	"media/img/light_tile.png",
	"media/img/end.png"
};

void load_textures( ef_video_scene_t* scn ){
	ef_video_create_tex_table( scn );
	for( int i = 0; i < TEX_COUNT; i++ ){
		ef_video_update_tex_table_png( scn, texture_s[i] );
	}
}
