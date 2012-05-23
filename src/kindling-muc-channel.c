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

#include "kindling-muc-channel.h"
#include <telepathy-glib/message-mixin.h>
#include <dbus/dbus-glib.h>


static void subject_iface_init(gpointer, gpointer);

G_DEFINE_TYPE_WITH_CODE (KindlingMUCChannel, kindling_muc_channel, TP_TYPE_BASE_CHANNEL,
                         G_IMPLEMENT_INTERFACE(TP_TYPE_SVC_CHANNEL_TYPE_TEXT, tp_message_mixin_text_iface_init);
                         G_IMPLEMENT_INTERFACE(TP_TYPE_SVC_CHANNEL_INTERFACE_MESSAGES, tp_message_mixin_messages_iface_init);
                         G_IMPLEMENT_INTERFACE(TP_TYPE_SVC_CHANNEL_INTERFACE_SUBJECT, subject_iface_init);
                         );

static void
kindling_muc_channel_init (KindlingMUCChannel *kindling_muc_channel)
{


	/* TODO: Add initialization code here */
}

static void
kindling_muc_channel_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (kindling_muc_channel_parent_class)->finalize (object);
}

static void
kindling_muc_channel_class_init (KindlingMUCChannelClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = kindling_muc_channel_finalize;
}

static void kindling_muc_channel_set_subject (TpSvcChannelInterfaceSubject *iface, const gchar *subject, DBusGMethodInvocation *context) {
}

static void subject_iface_init(gpointer g_iface, gpointer iface_data) {
	TpSvcChannelInterfaceSubjectClass *klass = g_iface;
	tp_svc_channel_interface_subject_implement_set_subject (klass, kindling_muc_channel_set_subject);
}
