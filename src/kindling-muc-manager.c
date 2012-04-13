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
#include <telepathy-glib/interfaces.h>
#include <telepathy-glib/util.h>
#include <telepathy-glib/handle.h>
#include "kindling-connection.h"

static void _muc_manager_iface_init(gpointer, gpointer);


G_DEFINE_TYPE_WITH_CODE (KindlingMUCManager, kindling_muc_manager, G_TYPE_OBJECT,
               G_IMPLEMENT_INTERFACE(TP_TYPE_CHANNEL_MANAGER, _muc_manager_iface_init));


enum {
	PROP_CONNECTION = 1,
	LAST_PROPERTY_ENUM
};

typedef struct _KindlingMUCManagerPrivate KindlingMUCManagerPrivate;
struct _KindlingMUCManagerPrivate {
        KindlingConnection *conn;
        GHashTable *channels;
};

#define KINDLING_MUC_MANAGER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), KINDLING_TYPE_MUC_MANAGER, KindlingMUCManagerPrivate))

static void kindling_muc_manager_set_property(GObject *obj, guint prop_id, const GValue *value, GParamSpec *pspec) {
		g_printf("muc-manager set prop %d\n", prop_id);
	KindlingMUCManagerPrivate *priv = KINDLING_MUC_MANAGER_GET_PRIVATE(obj);
	switch (prop_id) {
		case PROP_CONNECTION:
			priv->conn = g_value_get_object(value);
			break;
	}
}

static void kindling_muc_manager_get_property(GObject *obj, guint prop_id, GValue *value, GParamSpec *pspec) {
		g_printf("muc-manager get prop %d\n", prop_id);
	KindlingMUCManagerPrivate *priv = KINDLING_MUC_MANAGER_GET_PRIVATE(obj);
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
kindling_muc_manager_init (KindlingMUCManager *kindling_muc_manager)
{
	g_printf("init kindling muc manager\n");

	/* TODO: Add initialization code here */
}


static void
kindling_muc_manager_finalize (GObject *object)
{
	g_printf("finalize kindling muc manager\n");
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (kindling_muc_manager_parent_class)->finalize (object);
}


static void kindling_muc_manager_foreach_channel(TpChannelManager *manager, TpExportableChannelFunc func, gpointer data) {
	g_printf("foreach channel\n");
}

static const gchar * const muc_channel_allowed_properties[] = {
    TP_IFACE_CHANNEL ".TargetHandle",
    TP_IFACE_CHANNEL ".TargetID",
    NULL
};

static void kindling_muc_manager_type_foreach_channel_class(GType type, TpChannelManagerTypeChannelClassFunc func, gpointer data) {
	g_printf("foreach type channel class\n");
	GHashTable *table = g_hash_table_new_full (g_str_hash, g_str_equal, NULL, (GDestroyNotify)tp_g_value_slice_free);
	GValue *channel_type_value, *handle_type_value;

	channel_type_value = tp_g_value_slice_new (G_TYPE_STRING);
	g_value_set_static_string (channel_type_value, TP_IFACE_CHANNEL_TYPE_TEXT);
	g_hash_table_insert (table, TP_IFACE_CHANNEL ".ChannelType", channel_type_value);

	handle_type_value = tp_g_value_slice_new (G_TYPE_UINT);
	g_value_set_uint(handle_type_value, TP_HANDLE_TYPE_ROOM);
	g_hash_table_insert(table, TP_IFACE_CHANNEL ".TargetHandleType", handle_type_value);

	func (type, table, muc_channel_allowed_properties, data);
	g_hash_table_destroy(table);
}

static void kindling_muc_manager_ensure_channel(TpChannelManager *manager,
                                                         gpointer request_token,
                                                         GHashTable *request_properties) {
	g_printf("ensure channel\n");
}

static void kindling_muc_manager_request_channel(TpChannelManager *manager,
                                                         gpointer request_token,
                                                         GHashTable *request_properties) {
	g_printf("request channel\n");
}

static void kindling_muc_manager_create_channel(TpChannelManager *manager,
                                                         gpointer request_token,
                                                         GHashTable *request_properties) {
	g_printf("create channel\n");
}

static void
kindling_muc_manager_class_init (KindlingMUCManagerClass *klass)
{
	g_printf("class init kindling muc manager\n");
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GObjectClass* parent_class = G_OBJECT_CLASS (klass);
	GParamSpec *param_spec;

	g_type_class_add_private (klass, sizeof(KindlingMUCManagerPrivate));
	
	object_class->set_property = kindling_muc_manager_set_property;
	object_class->get_property = kindling_muc_manager_get_property;

	object_class->finalize = kindling_muc_manager_finalize;
	param_spec = g_param_spec_string ("connection", "connection", "kindling connection",NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	g_object_class_install_property (object_class, PROP_CONNECTION, param_spec);
}
static void _muc_manager_iface_init(gpointer g_iface, gpointer iface_data) {
	g_printf("iface init kindling muc manager\n");
	TpChannelManagerIface *iface = g_iface;
	iface->foreach_channel = kindling_muc_manager_foreach_channel;
	iface->type_foreach_channel_class = kindling_muc_manager_type_foreach_channel_class;
	iface->ensure_channel = kindling_muc_manager_ensure_channel;
	iface->request_channel = kindling_muc_manager_request_channel;
	iface->create_channel = kindling_muc_manager_create_channel;
}
