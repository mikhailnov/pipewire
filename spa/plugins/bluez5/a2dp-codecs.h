/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2006-2010  Nokia Corporation
 *  Copyright (C) 2004-2010  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef BLUEALSA_A2DPCODECS_H_
#define BLUEALSA_A2DPCODECS_H_

#include <stdint.h>
#include <stddef.h>

#include <spa/param/audio/format.h>
#include <spa/param/bluetooth/audio.h>
#include <spa/pod/pod.h>
#include <spa/pod/builder.h>

#define A2DP_CODEC_SBC			0x00
#define A2DP_CODEC_MPEG12		0x01
#define A2DP_CODEC_MPEG24		0x02
#define A2DP_CODEC_ATRAC		0x03
#define A2DP_CODEC_VENDOR		0xFF

#define A2DP_MAX_CAPS_SIZE		254

/* customized 16-bit vendor extension */
#define A2DP_CODEC_VENDOR_APTX		0x4FFF
#define A2DP_CODEC_VENDOR_LDAC		0x2DFF

#define SBC_SAMPLING_FREQ_48000		(1 << 0)
#define SBC_SAMPLING_FREQ_44100		(1 << 1)
#define SBC_SAMPLING_FREQ_32000		(1 << 2)
#define SBC_SAMPLING_FREQ_16000		(1 << 3)

#define SBC_CHANNEL_MODE_JOINT_STEREO	(1 << 0)
#define SBC_CHANNEL_MODE_STEREO		(1 << 1)
#define SBC_CHANNEL_MODE_DUAL_CHANNEL	(1 << 2)
#define SBC_CHANNEL_MODE_MONO		(1 << 3)

#define SBC_BLOCK_LENGTH_16		(1 << 0)
#define SBC_BLOCK_LENGTH_12		(1 << 1)
#define SBC_BLOCK_LENGTH_8		(1 << 2)
#define SBC_BLOCK_LENGTH_4		(1 << 3)

#define SBC_SUBBANDS_8			(1 << 0)
#define SBC_SUBBANDS_4			(1 << 1)

#define SBC_ALLOCATION_LOUDNESS		(1 << 0)
#define SBC_ALLOCATION_SNR		(1 << 1)

#define SBC_MIN_BITPOOL 2
#define SBC_MAX_BITPOOL 64

#define MPEG_CHANNEL_MODE_JOINT_STEREO	(1 << 0)
#define MPEG_CHANNEL_MODE_STEREO	(1 << 1)
#define MPEG_CHANNEL_MODE_DUAL_CHANNEL	(1 << 2)
#define MPEG_CHANNEL_MODE_MONO		(1 << 3)

#define MPEG_LAYER_MP3			(1 << 0)
#define MPEG_LAYER_MP2			(1 << 1)
#define MPEG_LAYER_MP1			(1 << 2)

#define MPEG_SAMPLING_FREQ_48000	(1 << 0)
#define MPEG_SAMPLING_FREQ_44100	(1 << 1)
#define MPEG_SAMPLING_FREQ_32000	(1 << 2)
#define MPEG_SAMPLING_FREQ_24000	(1 << 3)
#define MPEG_SAMPLING_FREQ_22050	(1 << 4)
#define MPEG_SAMPLING_FREQ_16000	(1 << 5)

#define MPEG_BIT_RATE_VBR		0x8000
#define MPEG_BIT_RATE_320000		0x4000
#define MPEG_BIT_RATE_256000		0x2000
#define MPEG_BIT_RATE_224000		0x1000
#define MPEG_BIT_RATE_192000		0x0800
#define MPEG_BIT_RATE_160000		0x0400
#define MPEG_BIT_RATE_128000		0x0200
#define MPEG_BIT_RATE_112000		0x0100
#define MPEG_BIT_RATE_96000		0x0080
#define MPEG_BIT_RATE_80000		0x0040
#define MPEG_BIT_RATE_64000		0x0020
#define MPEG_BIT_RATE_56000		0x0010
#define MPEG_BIT_RATE_48000		0x0008
#define MPEG_BIT_RATE_40000		0x0004
#define MPEG_BIT_RATE_32000		0x0002
#define MPEG_BIT_RATE_FREE		0x0001

#define AAC_OBJECT_TYPE_MPEG2_AAC_LC	0x80
#define AAC_OBJECT_TYPE_MPEG4_AAC_LC	0x40
#define AAC_OBJECT_TYPE_MPEG4_AAC_LTP	0x20
#define AAC_OBJECT_TYPE_MPEG4_AAC_SCA	0x10

#define AAC_SAMPLING_FREQ_8000		0x0800
#define AAC_SAMPLING_FREQ_11025		0x0400
#define AAC_SAMPLING_FREQ_12000		0x0200
#define AAC_SAMPLING_FREQ_16000		0x0100
#define AAC_SAMPLING_FREQ_22050		0x0080
#define AAC_SAMPLING_FREQ_24000		0x0040
#define AAC_SAMPLING_FREQ_32000		0x0020
#define AAC_SAMPLING_FREQ_44100		0x0010
#define AAC_SAMPLING_FREQ_48000		0x0008
#define AAC_SAMPLING_FREQ_64000		0x0004
#define AAC_SAMPLING_FREQ_88200		0x0002
#define AAC_SAMPLING_FREQ_96000		0x0001

#define AAC_CHANNELS_1			0x02
#define AAC_CHANNELS_2			0x01

#define AAC_GET_BITRATE(a) ((a).bitrate1 << 16 | \
					(a).bitrate2 << 8 | (a).bitrate3)
#define AAC_GET_FREQUENCY(a) ((a).frequency1 << 4 | (a).frequency2)

#define AAC_SET_BITRATE(a, b) \
	do { \
		(a).bitrate1 = ((b) >> 16) & 0x7f; \
		(a).bitrate2 = ((b) >> 8) & 0xff; \
		(a).bitrate3 = (b) & 0xff; \
	} while (0)
#define AAC_SET_FREQUENCY(a, f) \
	do { \
		(a).frequency1 = ((f) >> 4) & 0xff; \
		(a).frequency2 = (f) & 0x0f; \
	} while (0)

#define AAC_INIT_BITRATE(b) \
	.bitrate1 = ((b) >> 16) & 0x7f, \
	.bitrate2 = ((b) >> 8) & 0xff, \
	.bitrate3 = (b) & 0xff,
#define AAC_INIT_FREQUENCY(f) \
	.frequency1 = ((f) >> 4) & 0xff, \
	.frequency2 = (f) & 0x0f,

#define APTX_VENDOR_ID			0x0000004f
#define APTX_CODEC_ID			0x0001

#define APTX_CHANNEL_MODE_MONO		0x01
#define APTX_CHANNEL_MODE_STEREO	0x02

#define APTX_SAMPLING_FREQ_16000	0x08
#define APTX_SAMPLING_FREQ_32000	0x04
#define APTX_SAMPLING_FREQ_44100	0x02
#define APTX_SAMPLING_FREQ_48000	0x01

#define APTX_HD_VENDOR_ID               0x000000D7
#define APTX_HD_CODEC_ID                0x0024

#define APTX_HD_CHANNEL_MODE_MONO       0x1
#define APTX_HD_CHANNEL_MODE_STEREO     0x2

#define APTX_HD_SAMPLING_FREQ_16000     0x8
#define APTX_HD_SAMPLING_FREQ_32000     0x4
#define APTX_HD_SAMPLING_FREQ_44100     0x2
#define APTX_HD_SAMPLING_FREQ_48000     0x1

#define LDAC_VENDOR_ID			0x0000012d
#define LDAC_CODEC_ID			0x00aa

#define LDAC_CHANNEL_MODE_MONO		0x04
#define LDAC_CHANNEL_MODE_DUAL_CHANNEL	0x02
#define LDAC_CHANNEL_MODE_STEREO	0x01

#define LDAC_SAMPLING_FREQ_44100	0x20
#define LDAC_SAMPLING_FREQ_48000	0x10
#define LDAC_SAMPLING_FREQ_88200	0x08
#define LDAC_SAMPLING_FREQ_96000	0x04
#define LDAC_SAMPLING_FREQ_176400	0x02
#define LDAC_SAMPLING_FREQ_192000	0x01

typedef struct {
	uint32_t vendor_id;
	uint16_t codec_id;
} __attribute__ ((packed)) a2dp_vendor_codec_t;

typedef struct {
	a2dp_vendor_codec_t info;
	uint8_t frequency;
	uint8_t channel_mode;
} __attribute__ ((packed)) a2dp_ldac_t;

#if __BYTE_ORDER == __LITTLE_ENDIAN

typedef struct {
	uint8_t channel_mode:4;
	uint8_t frequency:4;
	uint8_t allocation_method:2;
	uint8_t subbands:2;
	uint8_t block_length:4;
	uint8_t min_bitpool;
	uint8_t max_bitpool;
} __attribute__ ((packed)) a2dp_sbc_t;

typedef struct {
	uint8_t channel_mode:4;
	uint8_t crc:1;
	uint8_t layer:3;
	uint8_t frequency:6;
	uint8_t mpf:1;
	uint8_t rfa:1;
	uint16_t bitrate;
} __attribute__ ((packed)) a2dp_mpeg_t;

typedef struct {
	uint8_t object_type;
	uint8_t frequency1;
	uint8_t rfa:2;
	uint8_t channels:2;
	uint8_t frequency2:4;
	uint8_t bitrate1:7;
	uint8_t vbr:1;
	uint8_t bitrate2;
	uint8_t bitrate3;
} __attribute__ ((packed)) a2dp_aac_t;

typedef struct {
	a2dp_vendor_codec_t info;
	uint8_t channel_mode:4;
	uint8_t frequency:4;
} __attribute__ ((packed)) a2dp_aptx_t;

typedef struct {
	a2dp_vendor_codec_t info;
	uint8_t channel_mode:4;
	uint8_t frequency:4;
	uint32_t rfa;
} __attribute__ ((packed)) a2dp_aptx_hd_t;

#elif __BYTE_ORDER == __BIG_ENDIAN

typedef struct {
	uint8_t frequency:4;
	uint8_t channel_mode:4;
	uint8_t block_length:4;
	uint8_t subbands:2;
	uint8_t allocation_method:2;
	uint8_t min_bitpool;
	uint8_t max_bitpool;
} __attribute__ ((packed)) a2dp_sbc_t;

typedef struct {
	uint8_t layer:3;
	uint8_t crc:1;
	uint8_t channel_mode:4;
	uint8_t rfa:1;
	uint8_t mpf:1;
	uint8_t frequency:6;
	uint16_t bitrate;
} __attribute__ ((packed)) a2dp_mpeg_t;

typedef struct {
	uint8_t object_type;
	uint8_t frequency1;
	uint8_t frequency2:4;
	uint8_t channels:2;
	uint8_t rfa:2;
	uint8_t vbr:1;
	uint8_t bitrate1:7;
	uint8_t bitrate2;
	uint8_t bitrate3;
} __attribute__ ((packed)) a2dp_aac_t;

typedef struct {
	a2dp_vendor_codec_t info;
	uint8_t frequency:4;
	uint8_t channel_mode:4;
} __attribute__ ((packed)) a2dp_aptx_t;

typedef struct {
	a2dp_vendor_codec_t info;
	uint8_t frequency:4;
	uint8_t channel_mode:4;
	uint32_t rfa;
} __attribute__ ((packed)) a2dp_aptx_hd_t;


#else
#error "Unknown byte order"
#endif

#define A2DP_CODEC_DEFAULT_RATE		48000
#define A2DP_CODEC_DEFAULT_CHANNELS	2

struct a2dp_codec_audio_info {
	uint32_t rate;
	uint32_t channels;
};

struct a2dp_codec_handle;

struct a2dp_codec {
	enum spa_bluetooth_audio_codec id;
	uint8_t codec_id;
	a2dp_vendor_codec_t vendor;

	const char *name;
	const char *description;
	const struct spa_dict *info;

	const size_t send_buf_size;

	const char *feature_flag;

	int (*fill_caps) (const struct a2dp_codec *codec, uint32_t flags,
			uint8_t caps[A2DP_MAX_CAPS_SIZE]);
	int (*select_config) (const struct a2dp_codec *codec, uint32_t flags,
			const void *caps, size_t caps_size,
			const struct a2dp_codec_audio_info *info,
			const struct spa_dict *settings, uint8_t config[A2DP_MAX_CAPS_SIZE]);
	int (*enum_config) (const struct a2dp_codec *codec,
			const void *caps, size_t caps_size, uint32_t id, uint32_t idx,
			struct spa_pod_builder *builder, struct spa_pod **param);
	int (*validate_config) (const struct a2dp_codec *codec, uint32_t flags,
			const void *caps, size_t caps_size,
			struct spa_audio_info *info);

	/** qsort comparison sorting caps in order of preference for the codec.
	 * Used in codec switching to select best remote endpoints.
	 * The caps handed in correspond to this codec_id, but are
	 * otherwise not checked beforehand.
	 */
	int (*caps_preference_cmp) (const struct a2dp_codec *codec, const void *caps1, size_t caps1_size,
			const void *caps2, size_t caps2_size, const struct a2dp_codec_audio_info *info);

	void *(*init_props) (const struct a2dp_codec *codec, const struct spa_dict *settings);
	void (*clear_props) (void *);
	int (*enum_props) (void *props, const struct spa_dict *settings, uint32_t id, uint32_t idx,
			struct spa_pod_builder *builder, struct spa_pod **param);
	int (*set_props) (void *props, const struct spa_pod *param);

	void *(*init) (const struct a2dp_codec *codec, uint32_t flags, void *config, size_t config_size,
			const struct spa_audio_info *info, void *props, size_t mtu);
	void (*deinit) (void *data);

	int (*update_props) (void *data, void *props);

	int (*get_block_size) (void *data);

	int (*abr_process) (void *data, size_t unsent);

	int (*start_encode) (void *data,
		void *dst, size_t dst_size, uint16_t seqnum, uint32_t timestamp);
	int (*encode) (void *data,
		const void *src, size_t src_size,
		void *dst, size_t dst_size,
		size_t *dst_out, int *need_flush);

	int (*start_decode) (void *data,
		const void *src, size_t src_size, uint16_t *seqnum, uint32_t *timestamp);
	int (*decode) (void *data,
		const void *src, size_t src_size,
		void *dst, size_t dst_size,
		size_t *dst_out);

	int (*reduce_bitpool) (void *data);
	int (*increase_bitpool) (void *data);
};

extern const struct a2dp_codec **a2dp_codecs;

struct a2dp_codec_config {
	uint32_t config;
	int value;
	unsigned int priority;
};

int a2dp_codec_select_config(const struct a2dp_codec_config configs[], size_t n,
	uint32_t cap, int preferred_value);

bool a2dp_codec_check_caps(const struct a2dp_codec *codec, unsigned int codec_id,
	const void *caps, size_t caps_size, const struct a2dp_codec_audio_info *info);

#endif
