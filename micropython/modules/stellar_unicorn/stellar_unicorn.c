#include "stellar_unicorn.h"


/***** Methods *****/
MP_DEFINE_CONST_FUN_OBJ_1(Channel___del___obj, Channel___del__);
MP_DEFINE_CONST_FUN_OBJ_KW(Channel_configure_obj, 1, Channel_configure);
MP_DEFINE_CONST_FUN_OBJ_1(Channel_restore_obj, Channel_restore);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(Channel_waveforms_obj, 1, 2, Channel_waveforms);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(Channel_frequency_obj, 1, 2, Channel_frequency);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(Channel_volume_obj, 1, 2, Channel_volume);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(Channel_attack_duration_obj, 1, 2, Channel_attack_duration);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(Channel_decay_duration_obj, 1, 2, Channel_decay_duration);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(Channel_sustain_level_obj, 1, 2, Channel_sustain_level);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(Channel_release_duration_obj, 1, 2, Channel_release_duration);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(Channel_pulse_width_obj, 1, 2, Channel_pulse_width);
MP_DEFINE_CONST_FUN_OBJ_1(Channel_trigger_attack_obj, Channel_trigger_attack);
MP_DEFINE_CONST_FUN_OBJ_1(Channel_trigger_release_obj, Channel_trigger_release);
MP_DEFINE_CONST_FUN_OBJ_KW(Channel_play_tone_obj, 2, Channel_play_tone);
//MP_DEFINE_CONST_FUN_OBJ_1(Channel_stop_playing_obj, Channel_stop_playing);

MP_DEFINE_CONST_FUN_OBJ_1(StellarUnicorn___del___obj, StellarUnicorn___del__);
MP_DEFINE_CONST_FUN_OBJ_1(StellarUnicorn_clear_obj, StellarUnicorn_clear);
MP_DEFINE_CONST_FUN_OBJ_2(StellarUnicorn_update_obj, StellarUnicorn_update);
MP_DEFINE_CONST_FUN_OBJ_2(StellarUnicorn_set_brightness_obj, StellarUnicorn_set_brightness);
MP_DEFINE_CONST_FUN_OBJ_1(StellarUnicorn_get_brightness_obj, StellarUnicorn_get_brightness);
MP_DEFINE_CONST_FUN_OBJ_2(StellarUnicorn_adjust_brightness_obj, StellarUnicorn_adjust_brightness);
MP_DEFINE_CONST_FUN_OBJ_2(StellarUnicorn_set_volume_obj, StellarUnicorn_set_volume);
MP_DEFINE_CONST_FUN_OBJ_1(StellarUnicorn_get_volume_obj, StellarUnicorn_get_volume);
MP_DEFINE_CONST_FUN_OBJ_2(StellarUnicorn_adjust_volume_obj, StellarUnicorn_adjust_volume);
MP_DEFINE_CONST_FUN_OBJ_1(StellarUnicorn_light_obj, StellarUnicorn_light);
MP_DEFINE_CONST_FUN_OBJ_2(StellarUnicorn_is_pressed_obj, StellarUnicorn_is_pressed);
MP_DEFINE_CONST_FUN_OBJ_2(StellarUnicorn_play_sample_obj, StellarUnicorn_play_sample);
MP_DEFINE_CONST_FUN_OBJ_1(StellarUnicorn_play_synth_obj, StellarUnicorn_play_synth);
MP_DEFINE_CONST_FUN_OBJ_1(StellarUnicorn_stop_playing_obj, StellarUnicorn_stop_playing);
MP_DEFINE_CONST_FUN_OBJ_2(StellarUnicorn_synth_channel_obj, StellarUnicorn_synth_channel);

/***** Binding of Methods *****/
static const mp_rom_map_elem_t Channel_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&Channel___del___obj) },
    { MP_ROM_QSTR(MP_QSTR_configure), MP_ROM_PTR(&Channel_configure_obj) },
    { MP_ROM_QSTR(MP_QSTR_restore), MP_ROM_PTR(&Channel_restore_obj) },
    { MP_ROM_QSTR(MP_QSTR_waveforms), MP_ROM_PTR(&Channel_waveforms_obj) },
    { MP_ROM_QSTR(MP_QSTR_frequency), MP_ROM_PTR(&Channel_frequency_obj) },
    { MP_ROM_QSTR(MP_QSTR_volume), MP_ROM_PTR(&Channel_volume_obj) },
    { MP_ROM_QSTR(MP_QSTR_attack_duration), MP_ROM_PTR(&Channel_attack_duration_obj) },
    { MP_ROM_QSTR(MP_QSTR_decay_duration), MP_ROM_PTR(&Channel_decay_duration_obj) },
    { MP_ROM_QSTR(MP_QSTR_sustain_level), MP_ROM_PTR(&Channel_sustain_level_obj) },
    { MP_ROM_QSTR(MP_QSTR_release_duration), MP_ROM_PTR(&Channel_release_duration_obj) },
    { MP_ROM_QSTR(MP_QSTR_pulse_width), MP_ROM_PTR(&Channel_pulse_width_obj) },
    { MP_ROM_QSTR(MP_QSTR_trigger_attack), MP_ROM_PTR(&Channel_trigger_attack_obj) },
    { MP_ROM_QSTR(MP_QSTR_trigger_release), MP_ROM_PTR(&Channel_trigger_release_obj) },
    { MP_ROM_QSTR(MP_QSTR_play_tone), MP_ROM_PTR(&Channel_play_tone_obj) },

    { MP_ROM_QSTR(MP_QSTR_NOISE), MP_ROM_INT(128) },
    { MP_ROM_QSTR(MP_QSTR_SQUARE), MP_ROM_INT(64) },
    { MP_ROM_QSTR(MP_QSTR_SAW), MP_ROM_INT(32) },
    { MP_ROM_QSTR(MP_QSTR_TRIANGLE), MP_ROM_INT(16) },
    { MP_ROM_QSTR(MP_QSTR_SINE), MP_ROM_INT(8) },
    { MP_ROM_QSTR(MP_QSTR_WAVE), MP_ROM_INT(1) },
};

static const mp_rom_map_elem_t StellarUnicorn_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&StellarUnicorn___del___obj) },
    { MP_ROM_QSTR(MP_QSTR_clear), MP_ROM_PTR(&StellarUnicorn_clear_obj) },
    { MP_ROM_QSTR(MP_QSTR_update), MP_ROM_PTR(&StellarUnicorn_update_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_brightness), MP_ROM_PTR(&StellarUnicorn_set_brightness_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_brightness), MP_ROM_PTR(&StellarUnicorn_get_brightness_obj) },
    { MP_ROM_QSTR(MP_QSTR_adjust_brightness), MP_ROM_PTR(&StellarUnicorn_adjust_brightness_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_volume), MP_ROM_PTR(&StellarUnicorn_set_volume_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_volume), MP_ROM_PTR(&StellarUnicorn_get_volume_obj) },
    { MP_ROM_QSTR(MP_QSTR_adjust_volume), MP_ROM_PTR(&StellarUnicorn_adjust_volume_obj) },
    { MP_ROM_QSTR(MP_QSTR_light), MP_ROM_PTR(&StellarUnicorn_light_obj) },
    { MP_ROM_QSTR(MP_QSTR_is_pressed), MP_ROM_PTR(&StellarUnicorn_is_pressed_obj) },
    { MP_ROM_QSTR(MP_QSTR_play_sample), MP_ROM_PTR(&StellarUnicorn_play_sample_obj) },
    { MP_ROM_QSTR(MP_QSTR_play_synth), MP_ROM_PTR(&StellarUnicorn_play_synth_obj) },
    { MP_ROM_QSTR(MP_QSTR_stop_playing), MP_ROM_PTR(&StellarUnicorn_stop_playing_obj) },
    { MP_ROM_QSTR(MP_QSTR_synth_channel), MP_ROM_PTR(&StellarUnicorn_synth_channel_obj) },

    { MP_ROM_QSTR(MP_QSTR_WIDTH), MP_ROM_INT(16) },
    { MP_ROM_QSTR(MP_QSTR_HEIGHT), MP_ROM_INT(16) },

    { MP_ROM_QSTR(MP_QSTR_SWITCH_A), MP_ROM_INT(0) },
    { MP_ROM_QSTR(MP_QSTR_SWITCH_B), MP_ROM_INT(1) },
    { MP_ROM_QSTR(MP_QSTR_SWITCH_C), MP_ROM_INT(3) },
    { MP_ROM_QSTR(MP_QSTR_SWITCH_D), MP_ROM_INT(6) },
    { MP_ROM_QSTR(MP_QSTR_SWITCH_SLEEP), MP_ROM_INT(27) },
    { MP_ROM_QSTR(MP_QSTR_SWITCH_VOLUME_UP), MP_ROM_INT(7) },
    { MP_ROM_QSTR(MP_QSTR_SWITCH_VOLUME_DOWN), MP_ROM_INT(8) },
    { MP_ROM_QSTR(MP_QSTR_SWITCH_BRIGHTNESS_UP), MP_ROM_INT(21) },
    { MP_ROM_QSTR(MP_QSTR_SWITCH_BRIGHTNESS_DOWN), MP_ROM_INT(26) },
};

static MP_DEFINE_CONST_DICT(Channel_locals_dict, Channel_locals_dict_table);
static MP_DEFINE_CONST_DICT(StellarUnicorn_locals_dict, StellarUnicorn_locals_dict_table);

/***** Class Definition *****/
#ifdef MP_DEFINE_CONST_OBJ_TYPE
MP_DEFINE_CONST_OBJ_TYPE(
    Channel_type,
    MP_QSTR_Channel,
    MP_TYPE_FLAG_NONE,
    make_new, Channel_make_new,
    print, Channel_print,
    locals_dict, (mp_obj_dict_t*)&Channel_locals_dict
);

MP_DEFINE_CONST_OBJ_TYPE(
    StellarUnicorn_type,
    MP_QSTR_StellarUnicorn,
    MP_TYPE_FLAG_NONE,
    make_new, StellarUnicorn_make_new,
    print, StellarUnicorn_print,
    locals_dict, (mp_obj_dict_t*)&StellarUnicorn_locals_dict
);
#else
const mp_obj_type_t Channel_type = {
    { &mp_type_type },
    .name = MP_QSTR_Channel,
    .print = Channel_print,
    .make_new = Channel_make_new,
    .locals_dict = (mp_obj_dict_t*)&Channel_locals_dict,
};

const mp_obj_type_t StellarUnicorn_type = {
    { &mp_type_type },
    .name = MP_QSTR_StellarUnicorn,
    .print = StellarUnicorn_print,
    .make_new = StellarUnicorn_make_new,
    .locals_dict = (mp_obj_dict_t*)&StellarUnicorn_locals_dict,
};
#endif

/***** Globals Table *****/
static const mp_map_elem_t stellar_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_stellar) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Channel), (mp_obj_t)&Channel_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_StellarUnicorn), (mp_obj_t)&StellarUnicorn_type },
};
static MP_DEFINE_CONST_DICT(mp_module_stellar_globals, stellar_globals_table);

/***** Module Definition *****/
const mp_obj_module_t stellar_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_stellar_globals,
};
#if MICROPY_VERSION <= 70144
MP_REGISTER_MODULE(MP_QSTR_stellar, stellar_user_cmodule, MODULE_STELLAR_ENABLED);
#else
MP_REGISTER_MODULE(MP_QSTR_stellar, stellar_user_cmodule);
#endif
