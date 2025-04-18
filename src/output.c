/*
 * Copyright (c) 2019 - 2022 Andri Yngvason
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

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <wayland-client.h>

#include "output.h"

static void output_handle_geometry(void* data, struct wl_output* wl_output,
                int32_t x, int32_t y, int32_t phys_width, int32_t phys_height,
                int32_t subpixel, const char* make, const char* model,
                int32_t transform)
{
}

static void output_handle_mode(void* data, struct wl_output* wl_output,
                uint32_t flags, int32_t width, int32_t height, int32_t refresh)
{
}

static void output_handle_done(void* data, struct wl_output* wl_output)
{
}

static void output_handle_scale(void* data, struct wl_output* wl_output,
                int32_t factor)
{
	struct output* output = data;
	output->scale = factor;
}

static const struct wl_output_listener output_listener = {
	.geometry = output_handle_geometry,
	.mode = output_handle_mode,
	.done = output_handle_done,
	.scale = output_handle_scale,
};

void output_destroy(struct output* output)
{
	wl_output_destroy(output->wl_output);
	free(output);
}

void output_list_destroy(struct wl_list* list)
{
	struct output* output;
	struct output* tmp;

	wl_list_for_each_safe(output, tmp, list, link) {
		wl_list_remove(&output->link);
		output_destroy(output);
	}
}

int32_t output_list_get_max_scale(const struct wl_list* list)
{
	struct output* output;

	int32_t scale = 1;

	wl_list_for_each(output, list, link)
		if (output->has_surface && output->scale > scale)
			scale = output->scale;

	return scale;
}

struct output* output_new(struct wl_output* wl_output, uint32_t id)
{
	struct output* output = calloc(1, sizeof(*output));
	if (!output)
		return NULL;

	output->wl_output = wl_output;
	output->id = id;
	output->has_surface = false;

	wl_output_add_listener(output->wl_output, &output_listener,
			output);

	return output;
}

struct output* output_find_by_id(struct wl_list* list, uint32_t id)
{
	struct output* output;

	wl_list_for_each(output, list, link)
		if (output->id == id)
			return output;

	return NULL;
}

struct output* output_find_by_wl_output(struct wl_list* list, struct wl_output* wl_output)
{
	struct output* output;

	wl_list_for_each(output, list, link)
		if (output->wl_output == wl_output)
			return output;

	return NULL;
}
