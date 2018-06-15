/* Simple Plugin API
 * Copyright (C) 2018 Wim Taymans <wim.taymans@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __SPA_BUFFER_ALLOC_H__
#define __SPA_BUFFER_ALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <spa/buffer/buffer.h>

struct spa_buffer_alloc_info {
#define SPA_BUFFER_ALLOC_FLAG_INLINE_META	(1<<0)	/**< add metadata data in the skeleton */
#define SPA_BUFFER_ALLOC_FLAG_INLINE_CHUNK	(1<<1)	/**< add chunk data in the skeleton */
#define SPA_BUFFER_ALLOC_FLAG_INLINE_DATA	(1<<2)	/**< add buffer data to the skeleton */
#define SPA_BUFFER_ALLOC_FLAG_INLINE_ALL	0b111
#define SPA_BUFFER_ALLOC_FLAG_NO_DATA		(1<<3)	/**< don't set data pointers */
	uint32_t flags;
	uint32_t n_metas;
	struct spa_meta *metas;
	uint32_t n_datas;
	struct spa_data *datas;
	uint32_t *data_aligns;
	size_t skel_size;	/**< size of the struct spa_buffer and inlined meta/chunk/data */
	size_t meta_size;	/**< size of the meta if not inlined */
	size_t chunk_size;	/**< size of the chunk if not inlined */
	size_t data_size;	/**< size of the data if not inlined */
};

static inline int spa_buffer_alloc_fill_info(struct spa_buffer_alloc_info *info,
					     uint32_t n_metas, struct spa_meta metas[n_metas],
					     uint32_t n_datas, struct spa_data datas[n_datas],
					     uint32_t data_aligns[n_datas])
{
	size_t size;
	int i;

	info->n_metas = n_metas;
	info->metas = metas;
	info->n_datas = n_datas;
	info->datas = datas;
	info->data_aligns = data_aligns;

	info->skel_size = sizeof(struct spa_buffer);
        info->skel_size += n_metas * sizeof(struct spa_meta);
        info->skel_size += n_datas * sizeof(struct spa_data);

	for (i = 0, size = 0; i < n_metas; i++)
		size += metas[i].size;
	info->meta_size = size;

	if (SPA_FLAG_CHECK(info->flags, SPA_BUFFER_ALLOC_FLAG_INLINE_META))
		info->skel_size += info->meta_size;

	info->chunk_size = n_datas * sizeof(struct spa_chunk);
	if (SPA_FLAG_CHECK(info->flags, SPA_BUFFER_ALLOC_FLAG_INLINE_CHUNK))
	        info->skel_size += info->chunk_size;

	for (i = 0, size = 0; i < n_datas; i++)
		size += datas[i].maxsize;
	info->data_size = size;

	if (!SPA_FLAG_CHECK(info->flags, SPA_BUFFER_ALLOC_FLAG_NO_DATA) &&
	    SPA_FLAG_CHECK(info->flags, SPA_BUFFER_ALLOC_FLAG_INLINE_DATA))
		info->skel_size += size;

	return 0;
}

static inline struct spa_buffer *
spa_buffer_alloc_layout(struct spa_buffer_alloc_info *info,
			void *skel_mem, void *data_mem, uint32_t id)
{
	struct spa_buffer *b = skel_mem;
	size_t size;
	int i;
	void **dp, *skel, *data;
	struct spa_chunk *cp;

	b->id = id;
	b->n_metas = info->n_metas;
	b->metas = SPA_MEMBER(b, sizeof(struct spa_buffer), struct spa_meta);
	b->n_datas = info->n_datas;
	b->datas = SPA_MEMBER(b->metas, info->n_metas * sizeof(struct spa_meta), struct spa_data);

	skel = SPA_MEMBER(b->datas, info->n_datas * sizeof(struct spa_data), void);
	data = data_mem;

	if (SPA_FLAG_CHECK(info->flags, SPA_BUFFER_ALLOC_FLAG_INLINE_META))
		dp = &skel;
	else
		dp = &data;

	for (i = 0; i < info->n_metas; i++) {
		struct spa_meta *m = &b->metas[i];
		*m = info->metas[i];
		m->data = *dp;
		*dp += m->size;
	}

	size = info->n_datas * sizeof(struct spa_chunk);
	if (SPA_FLAG_CHECK(info->flags, SPA_BUFFER_ALLOC_FLAG_INLINE_CHUNK)) {
		cp = skel;
		skel += size;
	}
	else {
		cp = data;
		data += size;
	}

	if (SPA_FLAG_CHECK(info->flags, SPA_BUFFER_ALLOC_FLAG_INLINE_DATA))
		dp = &skel;
	else
		dp = &data;

	for (i = 0; i < info->n_datas; i++) {
		struct spa_data *d = &b->datas[i];

		*d = info->datas[i];
		d->chunk = &cp[i];
		if (!SPA_FLAG_CHECK(info->flags, SPA_BUFFER_ALLOC_FLAG_NO_DATA)) {
			d->data = *dp;
			*dp += d->maxsize;
		}
	}
	return b;
}

static inline int
spa_buffer_alloc_layout_array(struct spa_buffer_alloc_info *info,
			      uint32_t n_buffers, struct spa_buffer *buffers[n_buffers],
			      void *skel_mem, void *data_mem)
{
	int i;
	size_t data_size = info->data_size + info->meta_size + info->chunk_size;
	for (i = 0; i < n_buffers; i++) {
		buffers[i] = spa_buffer_alloc_layout(info, skel_mem, data_mem, i);
                skel_mem = SPA_MEMBER(skel_mem, info->skel_size, void);
                data_mem = SPA_MEMBER(data_mem, data_size, void);
        }
	return 0;
}

static inline struct spa_buffer **
spa_buffer_alloc_array(uint32_t n_buffers, uint32_t flags,
		       uint32_t n_metas, struct spa_meta metas[n_metas],
		       uint32_t n_datas, struct spa_data datas[n_datas],
		       uint32_t data_aligns[n_datas])
{

        struct spa_buffer **buffers;
        struct spa_buffer_alloc_info info = { flags | SPA_BUFFER_ALLOC_FLAG_INLINE_ALL, };
        void *skel;

        spa_buffer_alloc_fill_info(&info, n_metas, metas, n_datas, datas, data_aligns);

	info.skel_size = SPA_ROUND_UP_N(info.skel_size, 16);

        buffers = calloc(n_buffers, sizeof(struct spa_buffer *) + info.skel_size);

        skel = SPA_MEMBER(buffers, sizeof(struct spa_buffer *) * n_buffers, void);

        spa_buffer_alloc_layout_array(&info, n_buffers, buffers, skel, NULL);

        return buffers;
}


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __SPA_BUFFER_ALLOC_H__ */
