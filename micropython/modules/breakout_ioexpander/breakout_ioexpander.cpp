#include "libraries/breakout_ioexpander/breakout_ioexpander.hpp"
#include <cstdio>

#define MP_OBJ_TO_PTR2(o, t) ((t *)(uintptr_t)(o))

// SDA/SCL on even/odd pins, I2C0/I2C1 on even/odd pairs of pins.
#define IS_VALID_SCL(i2c, pin) (((pin) & 1) == 1 && (((pin) & 2) >> 1) == (i2c))
#define IS_VALID_SDA(i2c, pin) (((pin) & 1) == 0 && (((pin) & 2) >> 1) == (i2c))


using namespace pimoroni;

extern "C" {
#include "breakout_ioexpander.h"

/***** Variables Struct *****/
typedef struct _breakout_ioexpander_BreakoutIOExpander_obj_t {
    mp_obj_base_t base;
    BreakoutIOExpander *breakout;
} breakout_ioexpander_BreakoutIOExpander_obj_t;

/***** Print *****/
void BreakoutIOExpander_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind; //Unused input parameter
    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(self_in, breakout_ioexpander_BreakoutIOExpander_obj_t);
    BreakoutIOExpander* breakout = self->breakout;
    mp_print_str(print, "BreakoutIOExpander(");

    mp_print_str(print, "i2c = ");
    mp_obj_print_helper(print, mp_obj_new_int((breakout->get_i2c() == i2c0) ? 0 : 1), PRINT_REPR);

    mp_print_str(print, ", address = 0x");
    char buf[3];
    sprintf(buf, "%02X", breakout->get_address());
    mp_print_str(print, buf);

    mp_print_str(print, ", sda = ");
    mp_obj_print_helper(print, mp_obj_new_int(breakout->get_sda()), PRINT_REPR);

    mp_print_str(print, ", scl = ");
    mp_obj_print_helper(print, mp_obj_new_int(breakout->get_scl()), PRINT_REPR);

    mp_print_str(print, ", int = ");
    mp_obj_print_helper(print, mp_obj_new_int(breakout->get_int()), PRINT_REPR);

    mp_print_str(print, ")");
}

/***** Constructor *****/
mp_obj_t BreakoutIOExpander_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    breakout_ioexpander_BreakoutIOExpander_obj_t *self = nullptr;

    if(n_args + n_kw == 0) {
        mp_arg_check_num(n_args, n_kw, 0, 0, true);
        self = m_new_obj(breakout_ioexpander_BreakoutIOExpander_obj_t);
        self->base.type = &breakout_ioexpander_BreakoutIOExpander_type;
        self->breakout = new BreakoutIOExpander();
    }
    else if(n_args + n_kw == 1) {
        enum { ARG_address };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_address, MP_ARG_REQUIRED | MP_ARG_INT },
        };

        // Parse args.
        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        self = m_new_obj(breakout_ioexpander_BreakoutIOExpander_obj_t);
        self->base.type = &breakout_ioexpander_BreakoutIOExpander_type;

        self->breakout = new BreakoutIOExpander(args[ARG_address].u_int);
    }
    else {
        enum { ARG_i2c, ARG_address, ARG_sda, ARG_scl, ARG_interrupt };
        static const mp_arg_t allowed_args[] = {
            { MP_QSTR_i2c, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_address, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_sda, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_scl, MP_ARG_REQUIRED | MP_ARG_INT },
            { MP_QSTR_interrupt, MP_ARG_INT, {.u_int = BreakoutIOExpander::PIN_UNUSED} },
        };

        // Parse args.
        mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
        mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

        // Get I2C bus.
        int i2c_id = args[ARG_i2c].u_int;
        if(i2c_id < 0 || i2c_id > 1) {
            mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("I2C(%d) doesn't exist"), i2c_id);
        }

        int sda = args[ARG_sda].u_int;
        if (!IS_VALID_SDA(i2c_id, sda)) {
            mp_raise_ValueError(MP_ERROR_TEXT("bad SDA pin"));
        }

        int scl = args[ARG_scl].u_int;
        if (!IS_VALID_SCL(i2c_id, scl)) {
            mp_raise_ValueError(MP_ERROR_TEXT("bad SCL pin"));
        }

        self = m_new_obj(breakout_ioexpander_BreakoutIOExpander_obj_t);
        self->base.type = &breakout_ioexpander_BreakoutIOExpander_type;

        i2c_inst_t *i2c = (i2c_id == 0) ? i2c0 : i2c1;
        self->breakout = new BreakoutIOExpander(i2c, args[ARG_address].u_int, sda, scl, args[ARG_interrupt].u_int);
    }

    if(!self->breakout->init()) {
        mp_raise_msg(&mp_type_RuntimeError, "IOExpander breakout not found when initialising");
    }

    return MP_OBJ_FROM_PTR(self);
}

/***** Methods *****/
mp_obj_t BreakoutIOExpander_get_chip_id(mp_obj_t self_in) {
    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(self_in, breakout_ioexpander_BreakoutIOExpander_obj_t);
    return mp_obj_new_int(self->breakout->get_chip_id());
}

mp_obj_t BreakoutIOExpander_set_address(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_address };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_address, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    self->breakout->set_address(args[ARG_address].u_int);

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_get_adc_vref(mp_obj_t self_in) {
    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(self_in, breakout_ioexpander_BreakoutIOExpander_obj_t);
    return mp_obj_new_float(self->breakout->get_adc_vref());
}

mp_obj_t BreakoutIOExpander_set_adc_vref(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_vref };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_vref, MP_ARG_REQUIRED | MP_ARG_OBJ },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    float vref = mp_obj_get_float(args[ARG_vref].u_obj);
    self->breakout->set_adc_vref(vref);

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_enable_interrupt_out(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin_swap };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_pin_swap, MP_ARG_BOOL, {.u_bool = false} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    bool pin_swap = args[ARG_pin_swap].u_bool;
    self->breakout->enable_interrupt_out(pin_swap);

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_disable_interrupt_out(mp_obj_t self_in) {
    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(self_in, breakout_ioexpander_BreakoutIOExpander_obj_t);
    self->breakout->disable_interrupt_out();

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_get_interrupt_flag(mp_obj_t self_in) {
    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(self_in, breakout_ioexpander_BreakoutIOExpander_obj_t);
    return mp_obj_new_bool(self->breakout->get_interrupt_flag());
}

mp_obj_t BreakoutIOExpander_clear_interrupt_flag(mp_obj_t self_in) {
    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(self_in, breakout_ioexpander_BreakoutIOExpander_obj_t);
    self->breakout->clear_interrupt_flag();

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_set_pin_interrupt(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin, ARG_enabled };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_exp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_enabled, MP_ARG_REQUIRED | MP_ARG_BOOL },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int pin = args[ARG_pin].u_int;
    bool enabled = args[ARG_enabled].u_bool;
    if(!self->breakout->set_pin_interrupt(pin, enabled)) {
        mp_raise_ValueError("pin out of range. Expected 1 to 14");
    }

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_pwm_load(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_wait_for_load };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_wait_for_load, MP_ARG_BOOL, {.u_bool = true} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    bool wait_for_load = args[ARG_wait_for_load].u_bool;
    self->breakout->pwm_load(wait_for_load);

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_pwm_loading(mp_obj_t self_in) {
    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(self_in, breakout_ioexpander_BreakoutIOExpander_obj_t);
    return mp_obj_new_bool(self->breakout->pwm_loading());
}

mp_obj_t BreakoutIOExpander_pwm_clear(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_wait_for_clear };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_wait_for_clear, MP_ARG_BOOL, {.u_bool = true} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    bool wait_for_clear = args[ARG_wait_for_clear].u_bool;
    self->breakout->pwm_clear(wait_for_clear);

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_pwm_clearing(mp_obj_t self_in) {
    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(self_in, breakout_ioexpander_BreakoutIOExpander_obj_t);
    return mp_obj_new_bool(self->breakout->pwm_clearing());
}

mp_obj_t BreakoutIOExpander_set_pwm_control(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_divider };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_divider, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int divider = args[ARG_divider].u_int;
    if(!self->breakout->set_pwm_control(divider)) {
        mp_raise_ValueError("divider not valid. Available options are: 1, 2, 4, 8, 16, 32, 64, 128");
    }

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_set_pwm_period(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_value, ARG_load };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_value, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_load, MP_ARG_BOOL, {.u_bool = true} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int value = args[ARG_value].u_int;
    bool load = args[ARG_load].u_bool;
    if(value < 0 || value > 65535)
        mp_raise_ValueError("value out of range. Expected 0 to 65535");
    else
        self->breakout->set_pwm_period(value, load);

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_get_mode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin, ARG_enabled };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_exp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int pin = args[ARG_pin].u_int;
    uint8_t mode = self->breakout->get_mode(pin);
    if(mode == UINT8_MAX)
        mp_raise_ValueError("pin out of range. Expected 1 to 14");

    return mp_obj_new_int(mode);
}

mp_obj_t BreakoutIOExpander_set_mode(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin, ARG_mode, ARG_schmitt_trigger, ARG_invert };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_exp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_mode, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_schmitt_trigger, MP_ARG_BOOL, {.u_bool = false} },
        { MP_QSTR_invert, MP_ARG_BOOL, {.u_bool = false} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int pin = args[ARG_pin].u_int;
    int mode = args[ARG_mode].u_int;
    bool schmitt_trigger = args[ARG_schmitt_trigger].u_bool;
    bool invert = args[ARG_invert].u_bool;

    if(pin < 1 || pin > IOExpander::NUM_PINS)
        mp_raise_ValueError("pin out of range. Expected 1 to 14");
    else
        self->breakout->set_mode(pin, mode, schmitt_trigger, invert);

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_input(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_exp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int pin = args[ARG_pin].u_int;

    if(pin < 1 || pin > IOExpander::NUM_PINS)
        mp_raise_ValueError("pin out of range. Expected 1 to 14");
    else
        return mp_obj_new_int(self->breakout->input(pin));

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_input_as_voltage(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_exp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int pin = args[ARG_pin].u_int;

    if(pin < 1 || pin > IOExpander::NUM_PINS)
        mp_raise_ValueError("pin out of range. Expected 1 to 14");
    else
        return mp_obj_new_float(self->breakout->input_as_voltage(pin));

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_output(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_pin, ARG_value, ARG_load };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_exp_pin, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_value, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_load, MP_ARG_BOOL, {.u_bool = true} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int pin = args[ARG_pin].u_int;
    int value = args[ARG_value].u_int;
    bool load = args[ARG_load].u_bool;

    if(pin < 1 || pin > IOExpander::NUM_PINS)
        mp_raise_ValueError("pin out of range. Expected 1 to 14");
    else if(value < 0 || value > 65535)
        mp_raise_ValueError("value out of range. Expected 0 to 65535");
    else
        self->breakout->output(pin, value, load);

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_setup_rotary_encoder(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_channel, ARG_pin_a, ARG_pin_b, ARG_pin_c, ARG_count_microsteps };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_channel, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_pin_a, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_pin_b, MP_ARG_REQUIRED | MP_ARG_INT },
        { MP_QSTR_pin_c, MP_ARG_INT, {.u_int = 0} },
        { MP_QSTR_count_microsteps, MP_ARG_BOOL, {.u_bool = false} },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int channel = args[ARG_channel].u_int;
    int pin_a = args[ARG_pin_a].u_int;
    int pin_b = args[ARG_pin_b].u_int;
    int pin_c = args[ARG_pin_c].u_int;
    bool count_microsteps = args[ARG_count_microsteps].u_bool;

    if(channel < 1 || channel > 4)
        mp_raise_ValueError("channel out of range. Expected 1 to 4");
    else if(pin_a < 1 || pin_a > 14)
        mp_raise_ValueError("pin_a out of range. Expected 1 to 14");
    else if(pin_b < 1 || pin_b > 14)
        mp_raise_ValueError("pin_b out of range. Expected 1 to 14");
    else if(pin_c < 1 || pin_c > 14)
        mp_raise_ValueError("pin_c out of range. Expected 1 to 14");
    else
        self->breakout->setup_rotary_encoder(channel, pin_a, pin_b, pin_c, count_microsteps);

    return mp_const_none;
}

mp_obj_t BreakoutIOExpander_read_rotary_encoder(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_self, ARG_channel, ARG_value, ARG_load };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_channel, MP_ARG_REQUIRED | MP_ARG_INT },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    breakout_ioexpander_BreakoutIOExpander_obj_t *self = MP_OBJ_TO_PTR2(args[ARG_self].u_obj, breakout_ioexpander_BreakoutIOExpander_obj_t);

    int channel = args[ARG_channel].u_int;
    if(channel < 1 || channel > 4)
        mp_raise_ValueError("channel out of range. Expected 1 to 4");
    else
        return mp_obj_new_int(self->breakout->read_rotary_encoder(channel));

    return mp_const_none;
}
}