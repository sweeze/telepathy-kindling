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
#include "kindling-connection-manager.h"
#include "kindling-protocol.h"
#include <glib-object.h>

G_DEFINE_TYPE(KindlingConnectionManager, kindling_connection_manager, TP_TYPE_BASE_CONNECTION_MANAGER)

static void kindling_connection_manager_init(KindlingConnectionManager *obj) {
		g_printf("inited kindling cm\n");
}

static void kindling_connection_manager_finalize (GObject *object) {
	g_printf("finalized kindling cm object\n");
	G_OBJECT_CLASS (kindling_connection_manager_parent_class)->finalize (object);
}

static void kindling_connection_manager_constructed (GObject *object) {
	g_printf("constructed kindling cm object\n");
	TpBaseConnectionManager *base = (TpBaseConnectionManager *) object;
    TpBaseProtocol *p;
    void (*constructed) (GObject *) = ((GObjectClass *) kindling_connection_manager_parent_class)->constructed;

    if (constructed != NULL)
            constructed (object);

    p = g_object_new(KINDLING_TYPE_PROTOCOL, "name", "campfire", NULL);
    tp_base_connection_manager_add_protocol (base, p);
    g_object_unref (p);

}

static void kindling_connection_manager_class_init(KindlingConnectionManagerClass *klass) {
        GObjectClass *object_class = G_OBJECT_CLASS (klass);
        TpBaseConnectionManagerClass *parent_class = TP_BASE_CONNECTION_MANAGER_CLASS(klass);

        parent_class->cm_dbus_name = "kindling";

        object_class->finalize = kindling_connection_manager_finalize;
        object_class->constructed = kindling_connection_manager_constructed;
		g_printf("inited kindling cm class\n");
}
