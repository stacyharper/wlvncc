/*
 * Copyright (c) 2020 - 2024 Andri Yngvason
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <wayland-client.h>

#define PRESSED_KEYS_MAX 256

struct xkb_context;
struct xkb_keymap;
struct xkb_state;

struct keyboard_collection;

struct keyboard {
	struct wl_keyboard* wl_keyboard;
	struct wl_list link;

	struct seat* seat;

	struct xkb_context* context;
	struct xkb_keymap* keymap;
	struct xkb_state* state;

	struct keyboard_collection* collection;
	uint32_t led_state;

	uint32_t pressed_keys[PRESSED_KEYS_MAX];

	bool waiting_for_modifiers;
};

struct keyboard_collection {
	struct wl_list keyboards;
	void (*on_event)(struct keyboard_collection*, struct keyboard*,
			uint32_t key, bool is_pressed);
	void* userdata;
};

struct keyboard_collection* keyboard_collection_new(void);
void keyboard_collection_destroy(struct keyboard_collection*);

int keyboard_collection_add_wl_keyboard(struct keyboard_collection* self,
		struct wl_keyboard* wl_keyboard, struct seat* seat);
void keyboard_collection_remove_wl_keyboard(struct keyboard_collection* self,
		struct wl_keyboard* wl_keyboard);
