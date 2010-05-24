/*
 * Copyright (c) 2003-2004 E. Will et al.
 * Copyright (c) 2006-2010 Atheme Development Group
 * Rights to this code are documented in doc/LICENSE.
 *
 * This file contains routines to handle the CService SET SECURE command.
 *
 * $Id$
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"chanserv/set_secure", false, _modinit, _moddeinit,
	"$Id$",
	"Atheme Development Group <http://www.atheme.org>"
);

static void cs_cmd_set_secure(sourceinfo_t *si, int parc, char *parv[]);

command_t cs_set_secure = { "SECURE", N_("Prevents unauthorized users from gaining operator status."), AC_NONE, 2, cs_cmd_set_secure };

list_t *cs_set_cmdtree;
list_t *cs_helptree;

void _modinit(module_t *m)
{
	MODULE_USE_SYMBOL(cs_set_cmdtree, "chanserv/set", "cs_set_cmdtree");
	MODULE_USE_SYMBOL(cs_helptree, "chanserv/main", "cs_helptree");

        command_add(&cs_set_secure, cs_set_cmdtree);

	help_addentry(cs_helptree, "SET SECURE", "help/cservice/set_secure", NULL);
}

void _moddeinit()
{
	command_delete(&cs_set_secure, cs_set_cmdtree);

	help_delentry(cs_helptree, "SET SECURE");
}

static void cs_cmd_set_secure(sourceinfo_t *si, int parc, char *parv[])
{
	mychan_t *mc;

	if (!(mc = mychan_find(parv[0])))
	{
		command_fail(si, fault_nosuch_target, _("Channel \2%s\2 is not registered."), parv[0]);
		return;
	}

	if (!chanacs_source_has_flag(mc, si, CA_SET))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to perform this command."));
		return;
	}

	if (!strcasecmp("ON", parv[1]))
	{
		if (MC_SECURE & mc->flags)
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is already set for channel \2%s\2."), "SECURE", mc->name);
			return;
		}

		logcommand(si, CMDLOG_SET, "SET:SECURE:ON: \2%s\2", mc->name);

		mc->flags |= MC_SECURE;

		command_success_nodata(si, _("The \2%s\2 flag has been set for channel \2%s\2."), "SECURE", mc->name);
		return;
	}
	else if (!strcasecmp("OFF", parv[1]))
	{
		if (!(MC_SECURE & mc->flags))
		{
			command_fail(si, fault_nochange, _("The \2%s\2 flag is not set for channel \2%s\2."), "SECURE", mc->name);
			return;
		}

		logcommand(si, CMDLOG_SET, "SET:SECURE:OFF: \2%s\2", mc->name);

		mc->flags &= ~MC_SECURE;

		command_success_nodata(si, _("The \2%s\2 flag has been removed for channel \2%s\2."), "SECURE", mc->name);
		return;
	}
	else
	{
		command_fail(si, fault_badparams, STR_INVALID_PARAMS, "SECURE");
		return;
	}
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */