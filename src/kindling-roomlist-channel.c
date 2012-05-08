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
#include <telepathy-glib/gtypes.h>
#include <json-glib/json-glib.h>


static void roomlist_iface_init (gpointer, gpointer);

G_DEFINE_TYPE_WITH_CODE (KindlingRoomlistChannel, kindling_roomlist_channel, 
                         TP_TYPE_BASE_CHANNEL,
                         G_IMPLEMENT_INTERFACE(TP_TYPE_SVC_CHANNEL_TYPE_ROOM_LIST, roomlist_iface_init));


typedef struct _KindlingRoomlistChannelPrivate KindlingRoomlistChannelPrivate;
struct _KindlingRoomlistChannelPrivate {
	gboolean is_listing;
	GPtrArray *rooms;
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
	if (parent_class->constructed != NULL) {
		parent_class->constructed(obj);
	}
	KindlingRoomlistChannelPrivate *priv = KINDLING_ROOMLIST_CHANNEL_GET_PRIVATE(self);
	priv->is_listing = FALSE;
	priv->rooms = g_ptr_array_new ();
	
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
	KindlingRoomlistChannelPrivate *priv = KINDLING_ROOMLIST_CHANNEL_GET_PRIVATE(self);
	tp_svc_channel_type_room_list_emit_listing_rooms (self, FALSE);
	TpBaseConnection *base_connection = tp_base_channel_get_connection (TP_BASE_CHANNEL(self));
	TpHandleRepoIface *room_repo = tp_base_connection_get_handles (base_connection, TP_HANDLE_TYPE_ROOM);
	JsonParser *parser = json_parser_new();
	GError *error = NULL;
	int i;
	json_parser_load_from_data (parser, msg->response_body->data, -1, &error);
	if (error == NULL) {
		JsonReader *reader = json_reader_new(json_parser_get_root (parser));
		json_reader_read_member (reader, "rooms");
		gint num_rooms = json_reader_count_elements (reader);
		g_printf("should be %d # rooms\n", num_rooms);
		for (i = 0; i < num_rooms; i++) {
			GValue room = {0,};
			GValue handle_name = {0,};
			GHashTable *keys;
			TpHandle handle;
			const gchar *room_name;
			gint64 room_id;
			json_reader_read_element(reader, i);
			
			json_reader_read_member (reader, "name");
			room_name = json_reader_get_string_value (reader);
			json_reader_end_member (reader);
			
			json_reader_read_member (reader, "id");
			room_id = json_reader_get_int_value (reader);
			json_reader_end_member (reader);
			
			json_reader_end_element(reader);
			g_printf("Found room %s with id %d\n", room_name, room_id);
			handle = tp_handle_ensure(room_repo, room_name, NULL, NULL);
			if (handle != 0) {
				keys = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);

				g_value_init (&handle_name, G_TYPE_STRING);
				g_value_take_string(&handle_name, room_name);
				g_hash_table_insert(keys, "handle-name", &handle_name);

				g_value_init(&room, TP_STRUCT_TYPE_ROOM_INFO);
				g_value_take_boxed(&room, 
				                   dbus_g_type_specialized_construct (TP_STRUCT_TYPE_ROOM_INFO));
				dbus_g_type_struct_set(&room,
				                       0, handle,
				                       1, "org.freedesktop.Telepathy.Channel.Type.Text",
				                       2, keys,
				                       G_MAXUINT);
				g_ptr_array_add(priv->rooms, g_value_get_boxed(&room));
				g_hash_table_unref(keys);
			}
		}
		json_reader_end_member (reader);
		g_object_unref (reader);
	}
	tp_svc_channel_type_room_list_emit_got_rooms (self, priv->rooms);
	while (priv->rooms->len > 0) {
		g_boxed_free(TP_STRUCT_TYPE_ROOM_INFO, g_ptr_array_index(priv->rooms, 0));
		g_ptr_array_remove_index_fast (priv->rooms, 0);
	}
	priv->is_listing = FALSE;
	tp_svc_channel_type_room_list_emit_listing_rooms (self, FALSE);
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

