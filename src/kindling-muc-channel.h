/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * telepathy-kindling
 * Copyright (C) Joe Barnett 2012 <jbarnett@taplop>
 * 
telepathy-kindling is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * telepathy-kindling is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#ifndef _KINDLING_MUC_CHANNEL_H_
#define _KINDLING_MUC_CHANNEL_H_

#include <glib-object.h>
#include <telepathy-glib/base-channel.h>

G_BEGIN_DECLS

#define KINDLING_TYPE_MUC_CHANNEL             (kindling_muc_channel_get_type ())
#define KINDLING_MUC_CHANNEL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KINDLING_TYPE_MUC_CHANNEL, KindlingMUCChannel))
#define KINDLING_MUC_CHANNEL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KINDLING_TYPE_MUC_CHANNEL, KindlingMUCChannelClass))
#define KINDLING_IS_MUC_CHANNEL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KINDLING_TYPE_MUC_CHANNEL))
#define KINDLING_IS_MUC_CHANNEL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KINDLING_TYPE_MUC_CHANNEL))
#define KINDLING_MUC_CHANNEL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KINDLING_TYPE_MUC_CHANNEL, KindlingMUCChannelClass))

typedef struct _KindlingMUCChannelClass KindlingMUCChannelClass;
typedef struct _KindlingMUCChannel KindlingMUCChannel;



struct _KindlingMUCChannelClass
{
	TpBaseChannelClass parent_class;
};

struct _KindlingMUCChannel
{
	TpBaseChannel parent_instance;

 
};

GType kindling_muc_channel_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _KINDLING_MUC_CHANNEL_H_ */
