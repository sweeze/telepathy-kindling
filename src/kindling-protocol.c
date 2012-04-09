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
#include "kindling-protocol.h"
#include "kindling-connection.h"
#include "kindling-muc-manager.h"


#include <dbus/dbus-glib.h>
#include <dbus/dbus-protocol.h>
#include <telepathy-glib/base-connection-manager.h>
#include <telepathy-glib/telepathy-glib.h>

#define ICON_NAME "im-irc"
#define ENGLISH_NAME "Campfire"
#define VCARD_FIELD_NAME "x-" CAMPFIRE_PROTOCOL_NAME


G_DEFINE_TYPE (KindlingProtocol, kindling_protocol, TP_TYPE_BASE_PROTOCOL);

static void
kindling_protocol_init (KindlingProtocol *kindling_protocol)
{

		g_printf("inited kindling protocol\n");

	/* TODO: Add initialization code here */
}

static void
kindling_protocol_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
		g_printf("finalize kindling protocol object\n");

	G_OBJECT_CLASS (kindling_protocol_parent_class)->finalize (object);
}

static const TpCMParamSpec kindling_params[] = {
	{"server", DBUS_TYPE_STRING_AS_STRING, G_TYPE_STRING, TP_CONN_MGR_PARAM_FLAG_REQUIRED, NULL, 0, NULL, NULL, (gpointer)"server", NULL},
	{"username", DBUS_TYPE_STRING_AS_STRING, G_TYPE_STRING, TP_CONN_MGR_PARAM_FLAG_REQUIRED, NULL, 0, NULL, NULL, (gpointer)"username", NULL},
	{"password", DBUS_TYPE_STRING_AS_STRING, G_TYPE_STRING, TP_CONN_MGR_PARAM_FLAG_SECRET, NULL, 0, NULL, NULL, (gpointer)"password", NULL},
	{NULL,NULL,0,0,NULL,0, NULL, NULL,NULL,NULL}
};

static const TpCMParamSpec * get_parameters(TpBaseProtocol *self) {
		g_printf("protocol get parameters\n");
	return kindling_params;
}

static TpBaseConnection *
new_connection (TpBaseProtocol *protocol, GHashTable *params, GError **error) {
		g_printf("protocol get new connection %s\n", tp_asv_get_string(params, "abc"));
	return g_object_new (KINDLING_TYPE_CONNECTION,
	                     "protocol", CAMPFIRE_PROTOCOL_NAME,
	                     "server", tp_asv_get_string(params, "server"),
	                     "username", tp_asv_get_string(params, "username"),
	                     "password", tp_asv_get_string(params, "password"),
	                     NULL);
}

static void get_connection_details(TpBaseProtocol *self,
                                   GStrv *connection_interfaces,
                                   GType **channel_managers,
                                   gchar **icon_name,
                                   char **english_name,
                                   gchar **vcard_field) {
	g_printf("called get details\n");
	if (connection_interfaces != NULL) {
		*connection_interfaces = g_strdupv((GStrv) kindling_connection_get_implemented_interfaces());
	}
	if (channel_managers != NULL) {
		GType types[] = {
			KINDLING_TYPE_MUC_MANAGER,
			G_TYPE_INVALID };
		*channel_managers = g_memdup(types, sizeof(types));
	}
    if (icon_name != NULL) {
		*icon_name = g_strdup (ICON_NAME);
	}
    if (vcard_field != NULL) {
		*vcard_field = g_strdup (VCARD_FIELD_NAME);
	}
    if (english_name != NULL) {
		*english_name = g_strdup (ENGLISH_NAME);
	}
				
}

static GStrv
dup_authentication_types (TpBaseProtocol *base) {
	g_printf("duping auth types\n");
	const gchar * const types [] = {
		TP_IFACE_CHANNEL_INTERFACE_SASL_AUTHENTICATION, NULL, };
	return g_strdupv((GStrv)types);
}

static gchar *normalize_contact(TpBaseProtocol *self, const gchar *contact, GError **error) {
	g_printf("normalizing contact\n");
	return g_strdupv(contact);
}

static char *identify_account(TpBaseProtocol *self, GHashTable *asv, GError **error) {
		g_printf("identing account\n");
	gchar *nick = g_strdupv(tp_asv_get_string(asv, "username"));
	gchar *server = g_strdupv(tp_asv_get_string(asv, "server"));
	gchar *nick_at_server = g_strdup_printf("%s@%s", nick, server);
	g_free(server);
	g_free(nick);
	return nick_at_server;
}

static GStrv
get_interfaces (TpBaseProtocol *self)
{
	g_printf("get interfaces\n");
  return g_new0 (gchar *, 1);
}


static void
kindling_protocol_class_init (KindlingProtocolClass *klass)
{
		g_printf("inited kindling protocol class\n");
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	TpBaseProtocolClass* parent_class = TP_BASE_PROTOCOL_CLASS (klass);
	parent_class->new_connection = new_connection;
	parent_class->get_parameters = get_parameters;
	parent_class->get_connection_details = get_connection_details;
	parent_class->normalize_contact = normalize_contact;
	parent_class->identify_account = identify_account;
	parent_class->dup_authentication_types = dup_authentication_types;
	parent_class->get_interfaces = get_interfaces;
	
	object_class->finalize = kindling_protocol_finalize;
}

