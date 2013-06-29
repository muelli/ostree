/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * Copyright (C) 2013 Colin Walters <walters@verbum.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Colin Walters <walters@verbum.org>
 */

#include "config.h"

#include "ot-builtins.h"
#include "ostree.h"
#include "ostree-repo-file.h"

static GOptionEntry options[] = {
  { NULL }
};

gboolean
ostree_builtin_refs (int argc, char **argv, GFile *repo_path, GError **error)
{
  gboolean ret = FALSE;
  GCancellable *cancellable = NULL;
  GOptionContext *context;
  const char *refspec_prefix = NULL;
  gs_unref_object OstreeRepo *repo = NULL;
  gs_unref_hashtable GHashTable *refs = NULL;
  GHashTableIter hashiter;
  gpointer hashkey, hashvalue;
  guint i;

  context = g_option_context_new ("[PREFIX...] - List refs");
  g_option_context_add_main_entries (context, options, NULL);

  if (!g_option_context_parse (context, &argc, &argv, error))
    goto out;

  repo = ostree_repo_new (repo_path);
  if (!ostree_repo_check (repo, error))
    goto out;

  if (argc >= 2)
    refspec_prefix = argv[1];

  if (!ostree_repo_list_refs (repo, refspec_prefix, &refs,
                              cancellable, error))
    goto out;

  g_hash_table_iter_init (&hashiter, refs);
  while (g_hash_table_iter_next (&hashiter, &hashkey, &hashvalue))
    {
      const char *ref = hashkey;
      g_print ("%s\n", ref);
    }
 
  ret = TRUE;
 out:
  if (context)
    g_option_context_free (context);
  return ret;
}
