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

#ifndef _KINDLING_PROTOCOL_H_
#define _KINDLING_PROTOCOL_H_

#include <glib-object.h>
#include <telepathy-glib/base-protocol.h>

#define CAMPFIRE_PROTOCOL_NAME "campfire"

G_BEGIN_DECLS

#define KINDLING_TYPE_PROTOCOL             (kindling_protocol_get_type ())
#define KINDLING_PROTOCOL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KINDLING_TYPE_PROTOCOL, KindlingProtocol))
#define KINDLING_PROTOCOL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KINDLING_TYPE_PROTOCOL, KindlingProtocolClass))
#define KINDLING_IS_PROTOCOL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KINDLING_TYPE_PROTOCOL))
#define KINDLING_IS_PROTOCOL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KINDLING_TYPE_PROTOCOL))
#define KINDLING_PROTOCOL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KINDLING_TYPE_PROTOCOL, KindlingProtocolClass))

typedef struct _KindlingProtocolClass KindlingProtocolClass;
typedef struct _KindlingProtocol KindlingProtocol;



struct _KindlingProtocolClass
{
	TpBaseProtocolClass parent_class;
};

struct _KindlingProtocol
{
	TpBaseProtocol parent_instance;

 
};

GType kindling_protocol_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _KINDLING_PROTOCOL_H_ */
