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

#include "kindling-roomlist-channel.h"
#include "kindling-connection.h"
#include <telepathy-glib/svc-channel.h>
#include <telepathy-glib/interfaces.h>


static void roomlist_iface_init (gpointer, gpointer);

G_DEFINE_TYPE_WITH_CODE (KindlingRoomlistChannel, kindling_roomlist_channel, 
                         TP_TYPE_BASE_CHANNEL,
                         G_IMPLEMENT_INTERFACE(TP_TYPE_SVC_CHANNEL_TYPE_ROOM_LIST, roomlist_iface_init));

enum {
	PROP_CONNECTION = 1,
	LAST_PROPERTY_ENUM
};

typedef struct _KindlingRoomlistChannelPrivate KindlingRoomlistChannelPrivate;
struct _KindlingRoomlistChannelPrivate {
        KindlingConnection *conn;
	
};
#define KINDLING_ROOMLIST_CHANNEL_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), KINDLING_TYPE_ROOMLIST_CHANNEL, KindlingRoomlistChannelPrivate))

static void kindling_roomlist_channel_set_property(GObject *obj, guint prop_id, const GValue *value, GParamSpec *pspec) {
		g_printf("roomlist-channel set prop %d\n", prop_id);
	KindlingRoomlistChannelPrivate *priv = KINDLING_ROOMLIST_CHANNEL_GET_PRIVATE(obj);
	switch (prop_id) {
		case PROP_CONNECTION:
			priv->conn = g_value_get_object(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
            break;
	}
}

static void kindling_roomlist_channel_get_property(GObject *obj, guint prop_id, GValue *value, GParamSpec *pspec) {
		g_printf("roomlist-channel get prop %d\n", prop_id);
	KindlingRoomlistChannelPrivate *priv = KINDLING_ROOMLIST_CHANNEL_GET_PRIVATE(obj);
	switch (prop_id) {
		case PROP_CONNECTION:
			g_value_set_object(value, priv->conn);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
            break;
	}
			
}
static void
kindling_roomlist_channel_init (KindlingRoomlistChannel *kindling_roomlist_channel)
{

    g_printf("roomlist init\n");

	/* TODO: Add initialization code here */
}

static void
kindling_roomlist_channel_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
    g_printf("roomlist finalize\n");

	G_OBJECT_CLASS (kindling_roomlist_channel_parent_class)->finalize (object);
}
static const gchar *kindling_roomlist_channel_interfaces[] = {
    NULL
};

static void
kindling_roomlist_channel_class_init (KindlingRoomlistChannelClass *klass)
{
    g_printf("roomlist class init\n");
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	TpBaseChannelClass* parent_class = TP_BASE_CHANNEL_CLASS (klass);
	GParamSpec *param_spec;
	g_type_class_add_private (klass, sizeof(KindlingRoomlistChannelPrivate));
	
	object_class->set_property = kindling_roomlist_channel_set_property;
	object_class->get_property = kindling_roomlist_channel_get_property;
	object_class->finalize = kindling_roomlist_channel_finalize;
	parent_class->channel_type = TP_IFACE_CHANNEL_TYPE_ROOM_LIST;
	parent_class->interfaces = kindling_roomlist_channel_interfaces;
	param_spec = g_param_spec_object
		("connection", "connection", "connection", KINDLING_TYPE_CONNECTION, G_PARAM_READWRITE);
	g_object_class_install_property (object_class, PROP_CONNECTION, param_spec);
}

static void
kindling_roomlist_channel_stop_listing (TpSvcChannelTypeRoomList *iface,
                                      DBusGMethodInvocation *context) {
    g_printf("roomlist stop listing\n");
}

static void
kindling_roomlist_channel_list_rooms (TpSvcChannelTypeRoomList *iface,
                                      DBusGMethodInvocation *context) {
    g_printf("roomlist list rooms\n");
}

static void
kindling_roomlist_channel_get_listing_rooms (TpSvcChannelTypeRoomList *iface,
                                      DBusGMethodInvocation *context) {
    g_printf("roomlist is listing?\n");
}

static void roomlist_iface_init (gpointer g_iface, gpointer iface_data) {
	TpSvcChannelTypeRoomListClass *klass = (TpSvcChannelTypeRoomListClass *) g_iface;
	tp_svc_channel_type_room_list_implement_get_listing_rooms (klass, kindling_roomlist_channel_get_listing_rooms);
	tp_svc_channel_type_room_list_implement_list_rooms (klass, kindling_roomlist_channel_list_rooms);
	tp_svc_channel_type_room_list_implement_stop_listing (klass, kindling_roomlist_channel_stop_listing);
}

