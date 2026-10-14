/* MotoCross Engine: synthetic continuous motocross engine sound.
 * SD88 MPC/Force port-kit engine interface. No recordings or copyrighted samples.
 */
#include "engine.h"
#include "params.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    float throttle;
    int engine;       /* 0 = 2-stroke, 1 = 4-stroke */
    int cc;           /* 0 = 125, 1 = 250, 2 = 450 */
    float volume;
    double phase;
    double pulse_phase;
    uint32_t noise;
    float rpm;
} Moto;

static float clampf(float x, float lo, float hi) {
    return x < lo ? lo : (x > hi ? hi : x);
}

static void *moto_create(const char *data_dir) {
    (void)data_dir;
    Moto *m = (Moto *)calloc(1, sizeof(*m));
    if (!m) return NULL;
    m->throttle = 18.0f;
    m->engine = 0;
    m->cc = 1;
    m->volume = 35.0f;
    m->noise = 0x13579BDFu;
    m->rpm = 1800.0f;
    return m;
}

static void moto_destroy(void *inst) {
    free(inst);
}

static void moto_midi(void *inst, const uint8_t *msg, int len) {
    (void)inst; (void)msg; (void)len;
}

static void moto_set_param(void *inst, const char *key, const char *val) {
    Moto *m = (Moto *)inst;
    if (!m || !key || !val) return;
    float v = strtof(val, NULL);
    if (!strcmp(key, "throttle")) m->throttle = clampf(v, 0.0f, 100.0f);
    else if (!strcmp(key, "engine")) m->engine = (int)clampf(v, 0.0f, 1.0f);
    else if (!strcmp(key, "cc")) m->cc = (int)clampf(v, 0.0f, 2.0f);
    else if (!strcmp(key, "volume")) m->volume = clampf(v, 0.0f, 100.0f);
}

static int moto_get_param(void *inst, const char *key, char *buf, int n) {
    Moto *m = (Moto *)inst;
    if (!m || !key || !buf || n < 2) return 0;
    if (!strcmp(key, "throttle")) snprintf(buf, (size_t)n, "%.0f", m->throttle);
    else if (!strcmp(key, "engine")) snprintf(buf, (size_t)n, "%d", m->engine);
    else if (!strcmp(key, "cc")) snprintf(buf, (size_t)n, "%d", m->cc);
    else if (!strcmp(key, "volume")) snprintf(buf, (size_t)n, "%.0f", m->volume);
    else return 0;
    return 1;
}

static void moto_render(void *inst, int16_t *out_lr, int frames) {
    Moto *m = (Moto *)inst;
    if (!out_lr || frames <= 0) return;
    if (!m) {
        memset(out_lr, 0, (size_t)frames * 2u * sizeof(*out_lr));
        return;
    }

    static const float cc_factor[3] = {1.18f, 1.0f, 0.78f};
    const float sr = 44100.0f;
    const float target_rpm = (1450.0f + m->throttle * 93.0f) * cc_factor[m->cc];
    const float amplitude = (m->volume / 100.0f) * 0.28f;

    for (int i = 0; i < frames; ++i) {
        m->rpm += (target_rpm - m->rpm) * 0.0018f;
        const float rpm = clampf(m->rpm, 900.0f, 11500.0f);
        const float firing_rate = (rpm / 60.0f) * (m->engine == 0 ? 1.0f : 0.5f);
        m->phase += firing_rate / sr;
        if (m->phase >= 1.0) m->phase -= floor(m->phase);

        /* A short, asymmetric exhaust pulse with harmonics. */
        const float p = (float)m->phase;
        const float pulse = expf(-p * (m->engine == 0 ? 17.0f : 11.0f))
                          * (0.68f + 0.32f * cosf(6.2831853f * p));
        const float rasp = sinf(6.2831853f * p * (m->engine == 0 ? 5.0f : 3.0f)) * 0.24f;
        m->noise ^= m->noise << 13;
        m->noise ^= m->noise >> 17;
        m->noise ^= m->noise << 5;
        const float noise = ((float)(m->noise & 0xffffu) / 32767.5f - 1.0f);
        const float body = sinf(6.2831853f * p) * 0.42f
                         + sinf(12.5663706f * p) * 0.23f
                         + sinf(18.8495559f * p) * 0.12f;
        const float s = clampf((body + pulse * 0.40f + rasp + noise * 0.055f) * amplitude,
                               -0.95f, 0.95f);
        const int16_t sample = (int16_t)(s * 32767.0f);
        out_lr[i * 2] = sample;
        out_lr[i * 2 + 1] = sample;
    }
}

static const mpc_engine_t ENGINE = {
    moto_create, moto_destroy, moto_midi, moto_set_param,
    moto_get_param, moto_render, NULL, NULL
};

const mpc_engine_t *mpc_engine(void) {
    return &ENGINE;
}
