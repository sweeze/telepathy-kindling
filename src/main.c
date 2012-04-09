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
#include <config.h>
#include <telepathy-glib/run.h>

#include "kindling-connection-manager.h"

static TpBaseConnectionManager *_construct_cm() {
	TpBaseConnectionManager *base_cm = TP_BASE_CONNECTION_MANAGER(g_object_new(KINDLING_TYPE_CONNECTION_MANAGER, NULL));
	return base_cm;
}


int
main (int argc, char *argv[])
{
	int result;
	g_type_init();
	result = tp_run_connection_manager("telepathy-kindling", VERSION, _construct_cm, argc, argv);
	return result;
}
