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
#include "kindling-connection.h"
#include "kindling-muc-manager.h"
#include <telepathy-glib/interfaces.h>
#include <telepathy-glib/handle-repo-dynamic.h>
#include <telepathy-glib/simple-password-manager.h>
#include <telepathy-glib/telepathy-glib.h>
#include <libsoup/soup.h>
#include <json-glib/json-glib.h>

G_DEFINE_TYPE (KindlingConnection, kindling_connection, TP_TYPE_BASE_CONNECTION);

static const gchar * interfaces_always_present[] = {
        TP_IFACE_CONNECTION_INTERFACE_ALIASING,
        TP_IFACE_CONNECTION_INTERFACE_CONTACT_INFO,
        TP_IFACE_CONNECTION_INTERFACE_REQUESTS,
        TP_IFACE_CONNECTION_INTERFACE_CONTACTS,
        NULL};

const gchar * const *kindling_connection_get_implemented_interfaces (void) {
        /* we don't have any conditionally-implemented interfaces yet */
        return interfaces_always_present;
}

enum {
	PROP_SERVER = 1,
	PROP_USERNAME,
	PROP_PASSWORD,
	LAST_PROPERTY_ENUM
};

typedef struct _KindlingConnectionPrivate KindlingConnectionPrivate;
struct _KindlingConnectionPrivate {
	char *server;
	char *username;
	char *password;
	char *api_token;

	//for actual auth
	TpSimplePasswordManager *password_manager;
	SoupSession *soup_session;
	gulong soup_auth_cb_handle;
};

#define KINDLING_CONNECTION_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), KINDLING_TYPE_CONNECTION, KindlingConnectionPrivate))

static void kindling_connection_set_property(GObject *obj, guint prop_id, const GValue *value, GParamSpec *pspec) {
		g_printf("connection set prop %d\n", prop_id);
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(obj);
	switch (prop_id) {
		case PROP_SERVER:
			g_free(priv->server);
			priv->server = g_value_dup_string(value);
			break;
		case PROP_USERNAME:
			g_free(priv->username);
			priv->username = g_value_dup_string(value);
			break;
		case PROP_PASSWORD:
			g_free(priv->password);
			priv->password = g_value_dup_string(value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
			break;
	}
}

static void kindling_connection_get_property(GObject *obj, guint prop_id, GValue *value, GParamSpec *pspec) {
		g_printf("connection get prop %d\n", prop_id);
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(obj);
	switch (prop_id) {
		case PROP_SERVER:
			g_value_set_string(value, priv->server);
			break;
		case PROP_USERNAME:
			g_value_set_string(value, priv->username);
			break;
		case PROP_PASSWORD:
			g_value_set_string(value, priv->password);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
            break;
	}
			
}

static void
kindling_connection_init (KindlingConnection *kindling_connection) {
		g_printf("connection init\n");
}

static void _soup_authenticate_cb(SoupSession *session,
                                  SoupMessage *msg,
                                  SoupAuth *auth,
                                  gboolean retrying,
                                  gpointer user_data) {
	g_printf("soup authenitcate\n");
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(user_data);
	if (!retrying) {
		soup_auth_authenticate(auth, priv->username, priv->password);
	}
									  
}

static void kindling_connection_constructed (GObject *object) {
		g_printf("connection constructed\n");
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(object);
	priv->soup_session = soup_session_async_new();

	priv->soup_auth_cb_handle = g_signal_connect(priv->soup_session,
	                 "authenticate",
	                 _soup_authenticate_cb,
	                 object);
	/* TODO: Add initialization code here */
}

static void
kindling_connection_finalize (GObject *object) {
	/* TODO: Add deinitalization code here */
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(object);
	g_free(priv->server);
	g_free(priv->username);
	g_free(priv->password);
	g_free(priv->api_token);
	
	g_signal_handler_disconnect(priv->soup_session, priv->soup_auth_cb_handle);
	g_object_unref(priv->soup_session);
		g_printf("finalize kindling connection object\n");
	G_OBJECT_CLASS (kindling_connection_parent_class)->finalize (object);
}
static void
kindling_connection_dispose (GObject *object) {
	g_printf("disposing connection\n");
}


static void _iface_create_handle_repos(TpBaseConnection *self, TpHandleRepoIface **repos) {
		g_printf("connection create handle repos\n");
	int i;
	for (i = 0; i < NUM_TP_HANDLE_TYPES; i++) {
		repos[i] = NULL;
	}
	repos[TP_HANDLE_TYPE_CONTACT] = (TpHandleRepoIface *) g_object_new(TP_TYPE_DYNAMIC_HANDLE_REPO,
                        "handle-type", TP_HANDLE_TYPE_CONTACT,
	                     NULL);
	repos[TP_HANDLE_TYPE_ROOM] = (TpHandleRepoIface *) g_object_new(TP_TYPE_DYNAMIC_HANDLE_REPO,
                        "handle-type", TP_HANDLE_TYPE_ROOM,
	                     NULL);
}

static GPtrArray *_iface_create_channel_managers(TpBaseConnection *self) {
		g_printf("connection create chan manager\n");
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(self);
	GPtrArray *managers = g_ptr_array_sized_new (1);
	g_ptr_array_add (managers, g_object_new (KINDLING_TYPE_MUC_MANAGER,
	                                      //   "connection", self,
	                                         NULL));
	priv->password_manager = tp_simple_password_manager_new(self);
	g_ptr_array_add(managers, priv->password_manager);
	return managers;
}

static void _connection_disconnect_with_gerror(KindlingConnection *conn, TpConnectionStatusReason reason, const gchar *key, const GError *error) {
	g_printf("disconnect w/ error\n");
    GHashTable *details = tp_asv_new(key, G_TYPE_STRING, error->message, NULL);

    tp_base_connection_disconnect_with_dbus_error(TP_BASE_CONNECTION(conn),
                                                  tp_error_get_dbus_name(error->code),
                                                  details,
                                                  reason);
    g_hash_table_unref(details);
}
static void
_soup_get_user_cb (SoupSession *session, SoupMessage *msg, gpointer user_data) {
	g_printf("got user req soup callback %d : %s\n", msg->status_code , msg->response_body->data);
	KindlingConnection *conn = KINDLING_CONNECTION(user_data);
	TpBaseConnection *base_conn = TP_BASE_CONNECTION(conn);
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(conn);
	JsonParser *parser = json_parser_new();
	GError *error = NULL;
	json_parser_load_from_data(parser, msg->response_body->data, -1, &error);
	if (error != NULL) {
		g_printf("error %s", error->message);
		if (base_conn->status != TP_CONNECTION_STATUS_DISCONNECTED) {
			_connection_disconnect_with_gerror(conn,
			                                   TP_CONNECTION_STATUS_REASON_AUTHENTICATION_FAILED,
			                                   "debug-message",
			                                   error);
		}
		g_error_free(error);
		g_object_unref (parser);
		return;
	}
	JsonReader *reader = json_reader_new(json_parser_get_root (parser));
	json_reader_read_member (reader, "user");
	json_reader_read_member (reader, "api_auth_token");
	g_free(priv->api_token);
	priv->api_token = g_strdup(json_reader_get_string_value(reader));
	g_object_unref (reader);
	g_object_unref (parser);

	if (priv->api_token == NULL) {
		if (base_conn->status != TP_CONNECTION_STATUS_DISCONNECTED) {
	        GError *error = g_error_new_literal(TP_ERRORS, 1, "no api token");

			_connection_disconnect_with_gerror(conn,
			                                   TP_CONNECTION_STATUS_REASON_AUTHENTICATION_FAILED,
			                                   "debug-message",
			                                   error);
	        g_error_free(error);

		}
	}

	tp_base_connection_change_status (base_conn, TP_CONNECTION_STATUS_CONNECTED, TP_CONNECTION_STATUS_REASON_REQUESTED);
}

static void _start_connecting_continue(KindlingConnection *self) {
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(self);
	g_printf("continue connecting to %s with user name %s\n", priv->server, priv->username);

	
	gchar *message = g_strdup_printf("https://%s.campfirenow.com/users/me.json", priv->server);
	g_printf("continue connecting to %s\n", message);
	SoupMessage *msg = soup_message_new("GET", message);
	g_free(message);
	//tp_base_connection_change_status (TP_BASE_CONNECTION(self), TP_CONNECTION_STATUS_CONNECTED, TP_CONNECTION_STATUS_REASON_REQUESTED);
	soup_session_queue_message(priv->soup_session, msg, _soup_get_user_cb, self);
}

static void _password_prompt_cb (GObject *source, GAsyncResult *result, gpointer user_data) {
	g_printf("prompt cb\n");
	KindlingConnection *conn = KINDLING_CONNECTION(user_data);
	TpBaseConnection *base_conn = TP_BASE_CONNECTION(conn);
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(conn);
	const GString *password;
	GError *error = NULL;

	password = tp_simple_password_manager_prompt_finish (TP_SIMPLE_PASSWORD_MANAGER(source),
	                                                     result, &error);
	if (error != NULL) {
		g_printf("error %s", error->message);
		if (base_conn->status != TP_CONNECTION_STATUS_DISCONNECTED) {
			_connection_disconnect_with_gerror(conn,
			                                   TP_CONNECTION_STATUS_REASON_AUTHENTICATION_FAILED,
			                                   "debug-message",
			                                   error);
		}
		g_error_free(error);
		return;
	}

	g_free(priv->password);
	priv->password = g_strdup(password->str);

	_start_connecting_continue(conn);
}

static gboolean _iface_start_connecting(TpBaseConnection *self, GError **error) {
		g_printf("connection connect \n");
	KindlingConnectionPrivate *priv = KINDLING_CONNECTION_GET_PRIVATE(self);
	TpHandleRepoIface *contact_handles = tp_base_connection_get_handles (self, TP_HANDLE_TYPE_CONTACT);
	self->self_handle = tp_handle_ensure(contact_handles, priv->username, NULL, error);
	if (!self->self_handle) {
		return FALSE;
	}
	tp_base_connection_change_status(self, TP_CONNECTION_STATUS_CONNECTING,
                                     TP_CONNECTION_STATUS_REASON_REQUESTED);


	tp_simple_password_manager_prompt_async (priv->password_manager,
	                                         _password_prompt_cb, self);
	return TRUE;
}

static void _iface_shut_down(TpBaseConnection *self) {
		g_printf("connection shut down\n");
}

static void _print_status_cb (TpBaseConnection *self) {
	g_printf("Status is : %d\n", self->status);
}

static void
kindling_connection_class_init (KindlingConnectionClass *klass)
{
	GParamSpec *param_spec;
		g_printf("inited kindling connection class\n");
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	TpBaseConnectionClass* parent_class = TP_BASE_CONNECTION_CLASS (klass);

	g_type_class_add_private (klass, sizeof(KindlingConnectionPrivate));
	object_class->set_property = kindling_connection_set_property;
	object_class->get_property = kindling_connection_get_property;
	object_class->constructed = kindling_connection_constructed;
	object_class->finalize = kindling_connection_finalize;
	object_class->dispose = kindling_connection_dispose;

	parent_class->create_handle_repos = _iface_create_handle_repos;
	parent_class->create_channel_managers = _iface_create_channel_managers;
	parent_class->start_connecting = _iface_start_connecting;
	parent_class->shut_down = _iface_shut_down;
	parent_class->interfaces_always_present = interfaces_always_present;
	parent_class->connecting = _print_status_cb;
	parent_class->connected = _print_status_cb;
	parent_class->disconnected = _print_status_cb;

	param_spec = g_param_spec_string ("server", "server", "campfire server to connect to",NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	g_object_class_install_property (object_class, PROP_SERVER, param_spec);
	param_spec = g_param_spec_string ("username", "username", "campfire user to auth with",NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	g_object_class_install_property (object_class, PROP_USERNAME, param_spec);
	param_spec = g_param_spec_string ("password", "password", "campfire password to auth with",NULL, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
	g_object_class_install_property (object_class, PROP_PASSWORD, param_spec);
}

