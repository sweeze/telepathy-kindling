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

#ifndef _KINDLING_ROOMLIST_MANAGER_H_
#define _KINDLING_ROOMLIST_MANAGER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define KINDLING_TYPE_ROOMLIST_MANAGER             (kindling_roomlist_manager_get_type ())
#define KINDLING_ROOMLIST_MANAGER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KINDLING_TYPE_ROOMLIST_MANAGER, KindlingRoomlistManager))
#define KINDLING_ROOMLIST_MANAGER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KINDLING_TYPE_ROOMLIST_MANAGER, KindlingRoomlistManagerClass))
#define KINDLING_IS_ROOMLIST_MANAGER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KINDLING_TYPE_ROOMLIST_MANAGER))
#define KINDLING_IS_ROOMLIST_MANAGER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KINDLING_TYPE_ROOMLIST_MANAGER))
#define KINDLING_ROOMLIST_MANAGER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KINDLING_TYPE_ROOMLIST_MANAGER, KindlingRoomlistManagerClass))

typedef struct _KindlingRoomlistManagerClass KindlingRoomlistManagerClass;
typedef struct _KindlingRoomlistManager KindlingRoomlistManager;



struct _KindlingRoomlistManagerClass
{
	GObjectClass parent_class;
};

struct _KindlingRoomlistManager
{
	GObject parent_instance;

 
};

GType kindling_roomlist_manager_get_type (void) G_GNUC_CONST;

void kindling_roomlist_manager_connected(KindlingRoomlistManager *manager);
void kindling_roomlist_manager_disconnected(KindlingRoomlistManager *manager);

G_END_DECLS

#endif /* _KINDLING_ROOMLIST_MANAGER_H_ */
