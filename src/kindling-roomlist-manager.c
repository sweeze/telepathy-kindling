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

#include "kindling-roomlist-manager.h"
#include "kindling-roomlist-channel.h"
#include "kindling-connection.h"
#include <telepathy-glib/channel-manager.h>
#include <telepathy-glib/interfaces.h>


static void _roomlist_manager_iface_init(gpointer, gpointer);


G_DEFINE_TYPE_WITH_CODE (KindlingRoomlistManager, kindling_roomlist_manager, G_TYPE_OBJECT,
               G_IMPLEMENT_INTERFACE(TP_TYPE_CHANNEL_MANAGER, _roomlist_manager_iface_init));

enum {
	PROP_CONNECTION = 1,
	LAST_PROPERTY_ENUM
};

typedef struct _KindlingRoomlistManagerPrivate KindlingRoomlistManagerPrivate;
struct _KindlingRoomlistManagerPrivate {
        KindlingConnection *conn;
		GPtrArray *channels;
	    guint timeout_id;
	
};
#define KINDLING_ROOMLIST_MANAGER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), KINDLING_TYPE_ROOMLIST_MANAGER, KindlingRoomlistManagerPrivate))

static void kindling_roomlist_manager_set_property(GObject *obj, guint prop_id, const GValue *value, GParamSpec *pspec) {
		g_printf("roomlist-manager set prop %d\n", prop_id);
	KindlingRoomlistManagerPrivate *priv = KINDLING_ROOMLIST_MANAGER_GET_PRIVATE(obj);
	switch (prop_id) {
		case PROP_CONNECTION:
			priv->conn = g_value_get_object(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
            break;
	}
}

static void kindling_roomlist_manager_get_property(GObject *obj, guint prop_id, GValue *value, GParamSpec *pspec) {
		g_printf("roomlist-manager get prop %d\n", prop_id);
	KindlingRoomlistManagerPrivate *priv = KINDLING_ROOMLIST_MANAGER_GET_PRIVATE(obj);
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
kindling_roomlist_manager_init (KindlingRoomlistManager *self)
{
    KindlingRoomlistManagerPrivate *priv = KINDLING_ROOMLIST_MANAGER_GET_PRIVATE(self);
	priv->channels = g_ptr_array_sized_new (1);
}

static void
kindling_roomlist_manager_finalize (GObject *object)
{
	KindlingRoomlistManagerPrivate *priv = KINDLING_ROOMLIST_MANAGER_GET_PRIVATE(object);
	g_ptr_array_unref (priv->channels);

	G_OBJECT_CLASS (kindling_roomlist_manager_parent_class)->finalize (object);
}

static void
kindling_roomlist_manager_class_init (KindlingRoomlistManagerClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GObjectClass* parent_class = G_OBJECT_CLASS (klass);
	GParamSpec *param_spec;

	g_type_class_add_private (klass, sizeof(KindlingRoomlistManagerPrivate));
	
	object_class->set_property = kindling_roomlist_manager_set_property;
	object_class->get_property = kindling_roomlist_manager_get_property;
	object_class->finalize = kindling_roomlist_manager_finalize;
	param_spec = g_param_spec_object ("connection", "connection", "kindling connection", KINDLING_TYPE_CONNECTION, G_PARAM_READWRITE);
	g_object_class_install_property (object_class, PROP_CONNECTION, param_spec);
}

static void kindling_roomlist_manager_foreach_channel (TpChannelManager *manager,
                                         TpExportableChannelFunc foreach,
                                         gpointer user_data) {
    g_printf("foreach_channel\n");
    KindlingRoomlistManagerPrivate *priv = KINDLING_ROOMLIST_MANAGER_GET_PRIVATE(manager);
    guint i;
	for (i = 0; i < priv->channels->len; i++) {
		TpExportableChannel *channel = TP_EXPORTABLE_CHANNEL (
          g_ptr_array_index (priv->channels, i));
		foreach(channel, user_data);
	}
}

static const gchar * const roomlist_channel_fixed_properties[] = {
    TP_IFACE_CHANNEL ".ChannelType",
    TP_IFACE_CHANNEL ".TargetHandleType",
    NULL
};

static const gchar * const roomlist_channel_allowed_properties[] = {
    TP_IFACE_CHANNEL_TYPE_ROOM_LIST ".Server",
    NULL
};

static void kindling_roomlist_manager_type_foreach_channel_class (GType type,
																TpChannelManagerTypeChannelClassFunc func,
																gpointer user_data) {
    g_printf("type_foreach_channel_class\n");
    GHashTable *table = g_hash_table_new_full (g_str_hash, g_str_equal,
      NULL, (GDestroyNotify) tp_g_value_slice_free);
	GValue *value;
																	
    value = tp_g_value_slice_new (G_TYPE_STRING);
	g_value_set_static_string (value, TP_IFACE_CHANNEL_TYPE_ROOM_LIST);
	g_hash_table_insert (table, TP_IFACE_CHANNEL ".ChannelType", value);

	value = tp_g_value_slice_new (G_TYPE_UINT);
	g_value_set_uint (value, TP_HANDLE_TYPE_NONE);
	g_hash_table_insert (table, TP_IFACE_CHANNEL ".TargetHandleType", value);

	func (type, table, roomlist_channel_allowed_properties, user_data);
	g_hash_table_unref (table);
}

static void roomlist_channel_closed_cb (KindlingRoomlistChannel *channel, gpointer user_data) {
	KindlingRoomlistManager *self = KINDLING_ROOMLIST_MANAGER(user_data);
	KindlingRoomlistManagerPrivate *priv = KINDLING_ROOMLIST_MANAGER_GET_PRIVATE(self);
	tp_channel_manager_emit_channel_closed_for_object (self, TP_EXPORTABLE_CHANNEL(channel));
	if (priv->channels != NULL) {
		g_ptr_array_remove (priv->channels, channel);
		g_object_unref (channel);
	}
}

static gboolean kindling_roomlist_manager_get_channel(TpChannelManager *manager,
                                                         gpointer request_token,
                                                         GHashTable *request_properties,
                                                         gboolean require_new) {
    KindlingRoomlistManagerPrivate *priv = KINDLING_ROOMLIST_MANAGER_GET_PRIVATE(manager);
    TpExportableChannel *channel;
    g_printf("get channel %s\n", tp_asv_get_string (request_properties,TP_IFACE_CHANNEL ".ChannelType"));
    if (tp_strdiff (tp_asv_get_string (request_properties, TP_IFACE_CHANNEL".ChannelType"),
                    TP_IFACE_CHANNEL_TYPE_ROOM_LIST)) {
		return FALSE;
	}
	if (priv->channels->len > 0 && !require_new) {
		tp_channel_manager_emit_request_already_satisfied (manager, request_token, TP_EXPORTABLE_CHANNEL(g_ptr_array_index (priv->channels,0)));
        return TRUE;
	}
    channel = g_object_new(KINDLING_TYPE_ROOMLIST_CHANNEL, "connection", priv->conn, NULL);
    g_signal_connect (channel, "closed", (GCallback) roomlist_channel_closed_cb, manager);
    g_ptr_array_add(priv->channels, channel);
    GSList *request_tokens = g_slist_prepend (NULL, request_token);
    tp_channel_manager_emit_new_channel (manager,
                                         TP_EXPORTABLE_CHANNEL(channel),
                                         request_tokens);
    g_slist_free (request_tokens);
    return TRUE;
}

static gboolean kindling_roomlist_manager_ensure_channel(TpChannelManager *manager,
                                                         gpointer request_token,
                                                         GHashTable *request_properties) {
    return kindling_roomlist_manager_get_channel(manager, request_token, request_properties, FALSE);
}

static gboolean kindling_roomlist_manager_request_channel(TpChannelManager *manager,
                                                         gpointer request_token,
                                                         GHashTable *request_properties) {
    return kindling_roomlist_manager_get_channel(manager, request_token, request_properties, FALSE);
}


static gboolean kindling_roomlist_manager_create_channel(TpChannelManager *manager,
                                                         gpointer request_token,
                                                         GHashTable *request_properties) {
    return kindling_roomlist_manager_get_channel(manager, request_token, request_properties, TRUE);
}


static void _roomlist_manager_iface_init(gpointer g_iface, gpointer iface_data) {
	g_printf("iface init kindling roomlist manager\n");
	TpChannelManagerIface *iface = g_iface;
	iface->foreach_channel = kindling_roomlist_manager_foreach_channel;
	iface->type_foreach_channel_class = kindling_roomlist_manager_type_foreach_channel_class;
	iface->ensure_channel = kindling_roomlist_manager_ensure_channel;
	iface->request_channel = kindling_roomlist_manager_request_channel;
	iface->create_channel = kindling_roomlist_manager_create_channel;
}

