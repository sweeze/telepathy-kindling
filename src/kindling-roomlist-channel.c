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
#include <json-glib/json-glib.h>


static void roomlist_iface_init (gpointer, gpointer);

G_DEFINE_TYPE_WITH_CODE (KindlingRoomlistChannel, kindling_roomlist_channel, 
                         TP_TYPE_BASE_CHANNEL,
                         G_IMPLEMENT_INTERFACE(TP_TYPE_SVC_CHANNEL_TYPE_ROOM_LIST, roomlist_iface_init));


typedef struct _KindlingRoomlistChannelPrivate KindlingRoomlistChannelPrivate;
struct _KindlingRoomlistChannelPrivate {
	gboolean is_listing;
};

#define KINDLING_ROOMLIST_CHANNEL_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), KINDLING_TYPE_ROOMLIST_CHANNEL, KindlingRoomlistChannelPrivate))

static void
kindling_roomlist_channel_init (KindlingRoomlistChannel *kindling_roomlist_channel)
{

    g_printf("roomlist init\n");

	/* TODO: Add initialization code here */
}
static void
kindling_roomlist_channel_constructed (GObject *obj) {
    g_printf("roomlist constructed\n");
	GObjectClass *parent_class = kindling_roomlist_channel_parent_class;
	KindlingRoomlistChannel *self = KINDLING_ROOMLIST_CHANNEL(obj);
	TpBaseChannel *base_chan = (TpBaseChannel *)self;
	TpBaseConnection *conn = tp_base_channel_get_connection (base_chan);
	TpHandleRepoIface *room_handles;
	if (parent_class->constructed != NULL) {
		parent_class->constructed(obj);
	}
	KindlingRoomlistChannelPrivate *priv = KINDLING_ROOMLIST_CHANNEL_GET_PRIVATE(self);
	priv->is_listing = FALSE;
	room_handles = tp_base_connection_get_handles(conn, TP_HANDLE_TYPE_ROOM);
	tp_base_channel_register(TP_BASE_CHANNEL(obj));

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
kindling_roomlist_channel_close(TpBaseChannel *base) {
	tp_base_channel_destroyed(base);
}

static void
kindling_roomlist_channel_class_init (KindlingRoomlistChannelClass *klass)
{
    g_printf("roomlist class init\n");
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	TpBaseChannelClass* parent_class = TP_BASE_CHANNEL_CLASS (klass);
	GParamSpec *param_spec;
	g_type_class_add_private (klass, sizeof(KindlingRoomlistChannelPrivate));
	
	object_class->finalize = kindling_roomlist_channel_finalize;
	object_class->constructed = kindling_roomlist_channel_constructed;
	
	parent_class->channel_type = TP_IFACE_CHANNEL_TYPE_ROOM_LIST;
	parent_class->interfaces = kindling_roomlist_channel_interfaces;
	parent_class->target_handle_type = TP_HANDLE_TYPE_NONE;
	parent_class->close = kindling_roomlist_channel_close;
}

static void
kindling_roomlist_channel_stop_listing (TpSvcChannelTypeRoomList *iface,
                                      DBusGMethodInvocation *context) {
    g_printf("roomlist stop listing\n");
    tp_svc_channel_type_room_list_return_from_stop_listing (context);
}

static void
_room_list_callback (SoupSession *session, SoupMessage *msg, gpointer user_data) {
	g_printf("got room list:\n\t%s\n",msg->response_body->data);
	KindlingRoomlistChannel *self = KINDLING_ROOMLIST_CHANNEL(user_data);
	tp_svc_channel_type_room_list_emit_listing_rooms (self, FALSE);

	JsonParser *parser = json_parser_new();
	GError *error = NULL;
	json_parser_load_from_data (parser, msg->response_body->data, -1, &error);
	if (error == NULL) {
		JsonReader *reader = json_reader_new(json_parser_get_root (parser));
		g_object_unref (reader);
	}
	g_object_unref (parser);
}

static void
kindling_roomlist_channel_list_rooms (TpSvcChannelTypeRoomList *iface,
                                      DBusGMethodInvocation *context) {
    g_printf("roomlist list rooms\n");
	KindlingRoomlistChannel *self = KINDLING_ROOMLIST_CHANNEL(iface);
    KindlingRoomlistChannelPrivate *priv = KINDLING_ROOMLIST_CHANNEL_GET_PRIVATE(self);
    TpBaseChannel *base = TP_BASE_CHANNEL(iface);
    KindlingConnection *conn = KINDLING_CONNECTION(tp_base_channel_get_connection (base));
    priv->is_listing = TRUE;
    tp_svc_channel_type_room_list_emit_listing_rooms (iface, TRUE);

    kindling_connection_list_rooms (conn, _room_list_callback, self);

    tp_svc_channel_type_room_list_return_from_list_rooms (context);
}

static void
kindling_roomlist_channel_get_listing_rooms (TpSvcChannelTypeRoomList *iface,
                                      DBusGMethodInvocation *context) {
    KindlingRoomlistChannelPrivate *priv = KINDLING_ROOMLIST_CHANNEL_GET_PRIVATE(iface); 
	tp_svc_channel_type_room_list_return_from_get_listing_rooms (
      context, priv->is_listing);  
}

static void roomlist_iface_init (gpointer g_iface, gpointer iface_data) {
	TpSvcChannelTypeRoomListClass *klass = (TpSvcChannelTypeRoomListClass *) g_iface;
	tp_svc_channel_type_room_list_implement_get_listing_rooms (klass, kindling_roomlist_channel_get_listing_rooms);
	tp_svc_channel_type_room_list_implement_list_rooms (klass, kindling_roomlist_channel_list_rooms);
	tp_svc_channel_type_room_list_implement_stop_listing (klass, kindling_roomlist_channel_stop_listing);
}

