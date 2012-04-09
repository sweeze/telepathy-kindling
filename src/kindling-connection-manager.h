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
#ifndef __KINDLING_CONNECTION_MANAGER_H__
#define __KINDLING_CONNECTION_MANAGER_H__

#include <glib-object.h>
#include <telepathy-glib/base-connection-manager.h>

G_BEGIN_DECLS

typedef struct _KindlingConnectionManager KindlingConnectionManager;
typedef struct _KindlingConnectionManagerClass KindlingConnectionManagerClass;

struct _KindlingConnectionManagerClass {
        TpBaseConnectionManagerClass parent_class;
};

struct _KindlingConnectionManager {
        TpBaseConnection parent;
};

GType kindling_connection_manager_get_type(void);

#define KINDLING_TYPE_CONNECTION_MANAGER \
        (kindling_connection_manager_get_type())
#define KINDLING_CONNECTION_MANAGER(obj) \
        (G_TYPE_CHECK_INSTANCE_CAST((obj), KINDLING_TYPE_CONNECTION_MANAGER, KindlingConnectionManager))
#define KINDLING_CONNECTION_MANAGER_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST((klass), KINDLING_TYPE_CONNECTION_MANAGER, KindlingConnectionManagerClass))
#define KINDLING_IS_CONNECTION_MANAGER(obj) \
        (G_TYPE_CHECK_INSTANCE_TYPE((obj), KINDLING_TYPE_CONNECTION_MANAGER))
#define KINDLING_IS_CONNECTION_MANAGER_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE((klass), KINDLING_TYPE_CONNECTION_MANAGER))
#define KINDLING_CONNECTION_MANAGER_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS((obj), KINDLING_TYPE_CONNECTION_MANAGER, KindlingConnectionManagerClass))




G_END_DECLS

#endif