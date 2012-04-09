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

#include "kindling-muc-manager.h"
#include <telepathy-glib/channel-manager.h>

static void _muc_manager_iface_init(gpointer, gpointer);


G_DEFINE_TYPE_WITH_CODE (KindlingMUCManager, kindling_muc_manager, G_TYPE_OBJECT,
               G_IMPLEMENT_INTERFACE(TP_TYPE_CHANNEL_MANAGER, _muc_manager_iface_init));

static void
kindling_muc_manager_init (KindlingMUCManager *kindling_muc_manager)
{


	/* TODO: Add initialization code here */
}

static void
kindling_muc_manager_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (kindling_muc_manager_parent_class)->finalize (object);
}

static void
kindling_muc_manager_class_init (KindlingMUCManagerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GObjectClass* parent_class = G_OBJECT_CLASS (klass);

	object_class->finalize = kindling_muc_manager_finalize;
}

static void _muc_manager_iface_init(gpointer g_iface, gpointer iface_data) {
	TpChannelManagerIface *iface = g_iface;
}
